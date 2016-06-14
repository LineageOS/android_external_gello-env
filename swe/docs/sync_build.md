# Sync & Build Instructions
## Recommended Build System
* 64-bit machine
* 4GB or higher RAM
* 8GB or higher swap space
* 10GB or higher free hard-disk space
* 64-bit Ubuntu 12.04

## Chromium Builds
Instructions provided in this page are similar to Chromium's build
instructions at:
<https://www.chromium.org/developers/how-tos/android-build-instructions>

## One-time Environment Setup
### Setup GIT

    $ sudo apt-get install git-core

### Setup depot_tools
1. Get depot_tools

        $ cd ~
        $ git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git

2. Add depot_tools to ~/.bashrc *(optional)*

        export ${PATH}=${PATH}:${HOME}/depot_tools

### Setup GCC
Ensure gcc is set to 4.6 or higher

    $ sudo update-alternatives --config gcc

Select 'gcc-4.6' from the list

If 'gcc-4.6' is not listed, please install with:

    $ sudo apt-get -y install build-essential

### Setup JDK
Ensure that JDK is set to JDK 7

    $ sudo update-alternatives --config java

It is enough to check if 'java-7-openjdk-amd64' is listed for now since the build process sets this automatically.

If 'java-7-openjdk-amd64' is not listed then install it:

    $ sudo apt-get -y install openjdk-7-jre openjdk-7-jdk

## For China *(optional)*: Sync from APAC mirrors
To speed up sync in China, you may sync the sources from a mirror located in China at:
<https://www.codeaurora.org/xwiki/bin/Support/CodeAuroraMirrors>

## Before Sync
* Ensure you are using the bash shell

* Create a folder to sync the branch into. This folder is referred to as *browser_root* in rest of this section.

* If depot_tools was not added to ~/.bashrc during [One-time Environment Setup](#one-time-environment-setup) then update PATH now:

        PATH=$PATH:$HOME/depot_tools

## Create .gclient file
In the folder *browser_root* create a file named '.gclient' with the following content:

        solutions = [
            {   "name"        : "src",
                "url"         : "git://codeaurora.org/quic/chrome4sdp/chromium/src.git@refs/remotes/origin/m46",
                "deps_file"   : "DEPS",
                "managed"     : False,
                "safesync_url": "",
            },
        ]
        target_os = ["android"]

## Sync Code
From *browser_root* folder:

    $ gclient sync -v -n --no-nag-max

## Build Code
* Install build dependencies from *browser_root* folder:

        $ cd src
        $ ./build/install-build-deps-android.sh

  The script 'install-build-depds-android.sh sets 'java-7-openjdk-amd64' as default

* Setup environment variables from *browser_root*/src

        $ . build/android/envsetup.sh

* Run hooks from *browser_root*/src

    * Default configuration

            $ GYP_DEFINES="$GYP_DEFINES clang=0 OS=android" gclient runhooks -v

      OR

    * Configuration overridden using Channels

            $ GYP_DEFINES="$GYP_DEFINES clang=0 OS=android swe_channels=<channel_name_1>,<channel_name_n>" gclient runhooks

      For more information about using Channels and overlay.xml, please refer to [Customizing using Channels](channels.md)

* Build APK
* APK

            $ ninja -C out/Release swe_browser_apk

    * System package

            $ ninja -C out/Release swe_system_package

  ##### Note
  * **System package** are used for integration with Android builds
