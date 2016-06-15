# Copyright (C) 2016 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
{
  'targets': [
     {
       # This jar contains the library I need to use.
       # This library doesn't contain the resources needed for the library to work.
       'target_name': 'ambient_sdk_javalib_no_res',
       'type' : 'none',
       'variables': {
         'jar_path': 'ambient/lib/ambientsdk-1.5.11.jar',
         'proguard_preprocess': 1,
         'proguard_config': 'proguard.flags',
       },
       'includes': ['../../build/java_prebuilt.gypi'],
     },
     {
       # This target contains my library with the resources needed.
       'target_name': 'ambient_sdk_javalib',
       'type': 'none',
       'variables': {
         'java_in_dir': 'ambient/',
         'R_package': ['com.cyngn.ambientsdk'],
         'R_package_relpath': ['com/cyngn/ambientsdk'],
         'has_java_resources': 1,
         'res_v14_skip': 1,
         'run_findbugs': 0,
       },
       'dependencies': [
         'ambient_sdk_javalib_no_res',
       ],
       'includes': [ '../../build/java.gypi' ]
     },
}
