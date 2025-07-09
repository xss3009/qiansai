# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/demo+demo")
  file(MAKE_DIRECTORY "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/demo+demo")
endif()
file(MAKE_DIRECTORY
  "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/1"
  "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/demo+demo"
  "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/demo+demo/tmp"
  "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/demo+demo/src/demo+demo-stamp"
  "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/demo+demo/src"
  "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/demo+demo/src/demo+demo-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/demo+demo/src/demo+demo-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "G:/QianSai/Modbus_f030/demo/MDK-ARM/tmp/demo+demo/src/demo+demo-stamp${cfgdir}") # cfgdir has leading slash
endif()
