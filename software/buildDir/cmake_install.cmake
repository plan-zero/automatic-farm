# Install script for directory: /home/ame/cleanProject/automatic-farm/software

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/external/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/libraries/hardware/atmega328p/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/spi/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/uart/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/e2p/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/flash/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/timer/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/wdg/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/nrf24/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/system-timer/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/firmware/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/programmer/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/application/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/testing/timer_testbench/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/testing/system_timer_testbench/cmake_install.cmake")
  include("/home/ame/cleanProject/automatic-farm/software/buildDir/testing/oneWireDev_testbench/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/ame/cleanProject/automatic-farm/software/buildDir/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
