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
include firmware/CMakeFiles/firmware.elf.dir/depend.make

# Include the progress variables for this target.
include firmware/CMakeFiles/firmware.elf.dir/progress.make

# Include the compile flags for this target's objects.
include firmware/CMakeFiles/firmware.elf.dir/flags.make

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj: firmware/CMakeFiles/firmware.elf.dir/flags.make
firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj: ../firmware/bootloader/src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/firmware && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj   -c /home/ame/cleanProject/automatic-farm/software/firmware/bootloader/src/main.c

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/firmware.elf.dir/bootloader/src/main.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/firmware && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/firmware/bootloader/src/main.c > CMakeFiles/firmware.elf.dir/bootloader/src/main.c.i

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/firmware.elf.dir/bootloader/src/main.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/firmware && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/firmware/bootloader/src/main.c -o CMakeFiles/firmware.elf.dir/bootloader/src/main.c.s

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj.requires:

.PHONY : firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj.requires

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj.provides: firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj.requires
	$(MAKE) -f firmware/CMakeFiles/firmware.elf.dir/build.make firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj.provides.build
.PHONY : firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj.provides

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj.provides.build: firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj


firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj: firmware/CMakeFiles/firmware.elf.dir/flags.make
firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj: ../firmware/bootloader/src/WriteApp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/firmware && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj   -c /home/ame/cleanProject/automatic-farm/software/firmware/bootloader/src/WriteApp.c

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/firmware && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/firmware/bootloader/src/WriteApp.c > CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.i

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/firmware && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/firmware/bootloader/src/WriteApp.c -o CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.s

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj.requires:

.PHONY : firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj.requires

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj.provides: firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj.requires
	$(MAKE) -f firmware/CMakeFiles/firmware.elf.dir/build.make firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj.provides.build
.PHONY : firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj.provides

firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj.provides.build: firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj


# Object files for target firmware.elf
firmware_elf_OBJECTS = \
"CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj" \
"CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj"

# External object files for target firmware.elf
firmware_elf_EXTERNAL_OBJECTS =

firmware/firmware.elf: firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj
firmware/firmware.elf: firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj
firmware/firmware.elf: firmware/CMakeFiles/firmware.elf.dir/build.make
firmware/firmware.elf: libraries/modules/nrf24/libnrf24.a
firmware/firmware.elf: libraries/drivers/e2p/libe2p.a
firmware/firmware.elf: libraries/drivers/flash/libflash.a
firmware/firmware.elf: libraries/drivers/wdg/libwdg.a
firmware/firmware.elf: libraries/drivers/spi/libspi.a
firmware/firmware.elf: firmware/CMakeFiles/firmware.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable firmware.elf"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/firmware && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/firmware.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
firmware/CMakeFiles/firmware.elf.dir/build: firmware/firmware.elf

.PHONY : firmware/CMakeFiles/firmware.elf.dir/build

firmware/CMakeFiles/firmware.elf.dir/requires: firmware/CMakeFiles/firmware.elf.dir/bootloader/src/main.c.obj.requires
firmware/CMakeFiles/firmware.elf.dir/requires: firmware/CMakeFiles/firmware.elf.dir/bootloader/src/WriteApp.c.obj.requires

.PHONY : firmware/CMakeFiles/firmware.elf.dir/requires

firmware/CMakeFiles/firmware.elf.dir/clean:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/firmware && $(CMAKE_COMMAND) -P CMakeFiles/firmware.elf.dir/cmake_clean.cmake
.PHONY : firmware/CMakeFiles/firmware.elf.dir/clean

firmware/CMakeFiles/firmware.elf.dir/depend:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ame/cleanProject/automatic-farm/software /home/ame/cleanProject/automatic-farm/software/firmware /home/ame/cleanProject/automatic-farm/software/buildDir /home/ame/cleanProject/automatic-farm/software/buildDir/firmware /home/ame/cleanProject/automatic-farm/software/buildDir/firmware/CMakeFiles/firmware.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : firmware/CMakeFiles/firmware.elf.dir/depend
