# Copyright (c) 2009 Google Inc. All rights reserved.
# Copyright (c) 2009 Apple Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# MultiCommandTool provides a framework for writing svn-like/git-like tools
# which are called with the following format:
# tool-name [global options] command-name [command options]

import optparse
import logging
import sys

from webkitpy.tool.commands.command import Command
from webkitpy.tool.commands.command import HelpPrintingOptionParser
from webkitpy.tool.commands.helpcommand import HelpCommand

_log = logging.getLogger(__name__)


class TryAgain(Exception):
    # This is an exception that can be raised in the execute method of a Command,
    # to make it retry from the start with the same arguments.
    # TODO(qyearsley): Remove this, as it appears unused.
    pass


class MultiCommandTool(object):

    global_options = None

    def __init__(self, name=None, commands=None):
        self._name = name or optparse.OptionParser(prog=name).get_prog_name()  # OptionParser has nice logic for fetching the name.
        # Allow the unit tests to disable command auto-discovery.
        self.commands = commands or [cls() for cls in self._find_all_commands() if cls.name]
        self.help_command = self.command_by_name(HelpCommand.name)
        # Require a help command, even if the manual test list doesn't include one.
        if not self.help_command:
            self.help_command = HelpCommand()
            self.commands.append(self.help_command)
        for command in self.commands:
            command.bind_to_tool(self)

    @classmethod
    def _add_all_subclasses(cls, class_to_crawl, seen_classes):
        for subclass in class_to_crawl.__subclasses__():
            if subclass not in seen_classes:
                seen_classes.add(subclass)
                cls._add_all_subclasses(subclass, seen_classes)

    @classmethod
    def _find_all_commands(cls):
        commands = set()
        cls._add_all_subclasses(Command, commands)
        return sorted(commands)

    def name(self):
        return self._name

    def _create_option_parser(self):
        usage = "Usage: %prog [options] COMMAND [ARGS]"
        return HelpPrintingOptionParser(epilog_method=self.help_command._help_epilog, prog=self.name(), usage=usage)

    @staticmethod
    def _split_command_name_from_args(args):
        # Assume the first argument which doesn't start with "-" is the command name.
        command_index = 0
        for arg in args:
            if arg[0] != "-":
                break
            command_index += 1
        else:
            return (None, args[:])

        command = args[command_index]
        return (command, args[:command_index] + args[command_index + 1:])

    def command_by_name(self, command_name):
        for command in self.commands:
            if command_name == command.name:
                return command
        return None

    def path(self):
        raise NotImplementedError, "subclasses must implement"

    def command_completed(self):
        pass

    def should_show_in_main_help(self, command):
        return command.show_in_main_help

    def should_execute_command(self, command):
        return True

    def _add_global_options(self, option_parser):
        global_options = self.global_options or []
        for option in global_options:
            option_parser.add_option(option)

    def handle_global_options(self, options):
        pass

    def main(self, argv=sys.argv):
        (command_name, args) = self._split_command_name_from_args(argv[1:])

        option_parser = self._create_option_parser()
        self._add_global_options(option_parser)

        command = self.command_by_name(command_name) or self.help_command
        if not command:
            option_parser.error("%s is not a recognized command" % command_name)

        command.set_option_parser(option_parser)
        (options, args) = command.parse_args(args)
        self.handle_global_options(options)

        (should_execute, failure_reason) = self.should_execute_command(command)
        if not should_execute:
            _log.error(failure_reason)
            return 0  # FIXME: Should this really be 0?

        while True:
            try:
                result = command.check_arguments_and_execute(options, args, self)
                break
            except TryAgain, e:
                pass

        self.command_completed()
        return result
