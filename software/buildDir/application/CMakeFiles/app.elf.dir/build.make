# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ame/cleanProject/automatic-farm/software

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ame/cleanProject/automatic-farm/software/buildDir

# Include any dependencies generated for this target.
include application/CMakeFiles/app.elf.dir/depend.make

# Include the progress variables for this target.
include application/CMakeFiles/app.elf.dir/progress.make

# Include the compile flags for this target's objects.
include application/CMakeFiles/app.elf.dir/flags.make

application/CMakeFiles/app.elf.dir/App/Main.c.obj: application/CMakeFiles/app.elf.dir/flags.make
application/CMakeFiles/app.elf.dir/App/Main.c.obj: ../application/App/Main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object application/CMakeFiles/app.elf.dir/App/Main.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/app.elf.dir/App/Main.c.obj   -c /home/ame/cleanProject/automatic-farm/software/application/App/Main.c

application/CMakeFiles/app.elf.dir/App/Main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/app.elf.dir/App/Main.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/application/App/Main.c > CMakeFiles/app.elf.dir/App/Main.c.i

application/CMakeFiles/app.elf.dir/App/Main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/app.elf.dir/App/Main.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/application/App/Main.c -o CMakeFiles/app.elf.dir/App/Main.c.s

application/CMakeFiles/app.elf.dir/App/Main.c.obj.requires:

.PHONY : application/CMakeFiles/app.elf.dir/App/Main.c.obj.requires

application/CMakeFiles/app.elf.dir/App/Main.c.obj.provides: application/CMakeFiles/app.elf.dir/App/Main.c.obj.requires
	$(MAKE) -f application/CMakeFiles/app.elf.dir/build.make application/CMakeFiles/app.elf.dir/App/Main.c.obj.provides.build
.PHONY : application/CMakeFiles/app.elf.dir/App/Main.c.obj.provides

application/CMakeFiles/app.elf.dir/App/Main.c.obj.provides.build: application/CMakeFiles/app.elf.dir/App/Main.c.obj


application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj: application/CMakeFiles/app.elf.dir/flags.make
application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj: ../application/Scheduler/Scheduler.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj   -c /home/ame/cleanProject/automatic-farm/software/application/Scheduler/Scheduler.c

application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/application/Scheduler/Scheduler.c > CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.i

application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/application/Scheduler/Scheduler.c -o CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.s

application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj.requires:

.PHONY : application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj.requires

application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj.provides: application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj.requires
	$(MAKE) -f application/CMakeFiles/app.elf.dir/build.make application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj.provides.build
.PHONY : application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj.provides

application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj.provides.build: application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj


application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj: application/CMakeFiles/app.elf.dir/flags.make
application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj: ../application/Abstractization/Sensors/Humidity/Humidity.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj   -c /home/ame/cleanProject/automatic-farm/software/application/Abstractization/Sensors/Humidity/Humidity.c

application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/application/Abstractization/Sensors/Humidity/Humidity.c > CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.i

application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/application/Abstractization/Sensors/Humidity/Humidity.c -o CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.s

application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj.requires:

.PHONY : application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj.requires

application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj.provides: application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj.requires
	$(MAKE) -f application/CMakeFiles/app.elf.dir/build.make application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj.provides.build
.PHONY : application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj.provides

application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj.provides.build: application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj


application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj: application/CMakeFiles/app.elf.dir/flags.make
application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj: ../application/Communication/Communication.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/app.elf.dir/Communication/Communication.c.obj   -c /home/ame/cleanProject/automatic-farm/software/application/Communication/Communication.c

application/CMakeFiles/app.elf.dir/Communication/Communication.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/app.elf.dir/Communication/Communication.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/application/Communication/Communication.c > CMakeFiles/app.elf.dir/Communication/Communication.c.i

application/CMakeFiles/app.elf.dir/Communication/Communication.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/app.elf.dir/Communication/Communication.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/application/Communication/Communication.c -o CMakeFiles/app.elf.dir/Communication/Communication.c.s

application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj.requires:

.PHONY : application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj.requires

application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj.provides: application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj.requires
	$(MAKE) -f application/CMakeFiles/app.elf.dir/build.make application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj.provides.build
.PHONY : application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj.provides

application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj.provides.build: application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj


application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj: application/CMakeFiles/app.elf.dir/flags.make
application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj: ../application/Communication/radio_link.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/app.elf.dir/Communication/radio_link.c.obj   -c /home/ame/cleanProject/automatic-farm/software/application/Communication/radio_link.c

application/CMakeFiles/app.elf.dir/Communication/radio_link.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/app.elf.dir/Communication/radio_link.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/application/Communication/radio_link.c > CMakeFiles/app.elf.dir/Communication/radio_link.c.i

application/CMakeFiles/app.elf.dir/Communication/radio_link.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/app.elf.dir/Communication/radio_link.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/application/Communication/radio_link.c -o CMakeFiles/app.elf.dir/Communication/radio_link.c.s

application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj.requires:

.PHONY : application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj.requires

application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj.provides: application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj.requires
	$(MAKE) -f application/CMakeFiles/app.elf.dir/build.make application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj.provides.build
.PHONY : application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj.provides

application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj.provides.build: application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj


application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj: application/CMakeFiles/app.elf.dir/flags.make
application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj: ../application/Communication/network_def.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/app.elf.dir/Communication/network_def.c.obj   -c /home/ame/cleanProject/automatic-farm/software/application/Communication/network_def.c

application/CMakeFiles/app.elf.dir/Communication/network_def.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/app.elf.dir/Communication/network_def.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/application/Communication/network_def.c > CMakeFiles/app.elf.dir/Communication/network_def.c.i

application/CMakeFiles/app.elf.dir/Communication/network_def.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/app.elf.dir/Communication/network_def.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/application/Communication/network_def.c -o CMakeFiles/app.elf.dir/Communication/network_def.c.s

application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj.requires:

.PHONY : application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj.requires

application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj.provides: application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj.requires
	$(MAKE) -f application/CMakeFiles/app.elf.dir/build.make application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj.provides.build
.PHONY : application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj.provides

application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj.provides.build: application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj


application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj: application/CMakeFiles/app.elf.dir/flags.make
application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj: ../application/SwOta/ota.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/app.elf.dir/SwOta/ota.c.obj   -c /home/ame/cleanProject/automatic-farm/software/application/SwOta/ota.c

application/CMakeFiles/app.elf.dir/SwOta/ota.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/app.elf.dir/SwOta/ota.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/application/SwOta/ota.c > CMakeFiles/app.elf.dir/SwOta/ota.c.i

application/CMakeFiles/app.elf.dir/SwOta/ota.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/app.elf.dir/SwOta/ota.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/application/SwOta/ota.c -o CMakeFiles/app.elf.dir/SwOta/ota.c.s

application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj.requires:

.PHONY : application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj.requires

application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj.provides: application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj.requires
	$(MAKE) -f application/CMakeFiles/app.elf.dir/build.make application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj.provides.build
.PHONY : application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj.provides

application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj.provides.build: application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj


# Object files for target app.elf
app_elf_OBJECTS = \
"CMakeFiles/app.elf.dir/App/Main.c.obj" \
"CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj" \
"CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj" \
"CMakeFiles/app.elf.dir/Communication/Communication.c.obj" \
"CMakeFiles/app.elf.dir/Communication/radio_link.c.obj" \
"CMakeFiles/app.elf.dir/Communication/network_def.c.obj" \
"CMakeFiles/app.elf.dir/SwOta/ota.c.obj"

# External object files for target app.elf
app_elf_EXTERNAL_OBJECTS =

application/app.elf: application/CMakeFiles/app.elf.dir/App/Main.c.obj
application/app.elf: application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj
application/app.elf: application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj
application/app.elf: application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj
application/app.elf: application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj
application/app.elf: application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj
application/app.elf: application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj
application/app.elf: application/CMakeFiles/app.elf.dir/build.make
application/app.elf: libraries/modules/nrf24/libnrf24_api.a
application/app.elf: libraries/drivers/uart/libuart.a
application/app.elf: libraries/drivers/timer/libtimer.a
application/app.elf: libraries/drivers/wdg/libwdg.a
application/app.elf: libraries/modules/nrf24/libnrf24_api.a
application/app.elf: libraries/modules/system-timer/libsystem-timer.a
application/app.elf: libraries/drivers/timer/libtimer.a
application/app.elf: application/CMakeFiles/app.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable app.elf"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/app.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
application/CMakeFiles/app.elf.dir/build: application/app.elf

.PHONY : application/CMakeFiles/app.elf.dir/build

application/CMakeFiles/app.elf.dir/requires: application/CMakeFiles/app.elf.dir/App/Main.c.obj.requires
application/CMakeFiles/app.elf.dir/requires: application/CMakeFiles/app.elf.dir/Scheduler/Scheduler.c.obj.requires
application/CMakeFiles/app.elf.dir/requires: application/CMakeFiles/app.elf.dir/Abstractization/Sensors/Humidity/Humidity.c.obj.requires
application/CMakeFiles/app.elf.dir/requires: application/CMakeFiles/app.elf.dir/Communication/Communication.c.obj.requires
application/CMakeFiles/app.elf.dir/requires: application/CMakeFiles/app.elf.dir/Communication/radio_link.c.obj.requires
application/CMakeFiles/app.elf.dir/requires: application/CMakeFiles/app.elf.dir/Communication/network_def.c.obj.requires
application/CMakeFiles/app.elf.dir/requires: application/CMakeFiles/app.elf.dir/SwOta/ota.c.obj.requires

.PHONY : application/CMakeFiles/app.elf.dir/requires

application/CMakeFiles/app.elf.dir/clean:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/application && $(CMAKE_COMMAND) -P CMakeFiles/app.elf.dir/cmake_clean.cmake
.PHONY : application/CMakeFiles/app.elf.dir/clean

application/CMakeFiles/app.elf.dir/depend:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ame/cleanProject/automatic-farm/software /home/ame/cleanProject/automatic-farm/software/application /home/ame/cleanProject/automatic-farm/software/buildDir /home/ame/cleanProject/automatic-farm/software/buildDir/application /home/ame/cleanProject/automatic-farm/software/buildDir/application/CMakeFiles/app.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : application/CMakeFiles/app.elf.dir/depend
