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
include testing/timer_testbench/CMakeFiles/timer_test.elf.dir/depend.make

# Include the progress variables for this target.
include testing/timer_testbench/CMakeFiles/timer_test.elf.dir/progress.make

# Include the compile flags for this target's objects.
include testing/timer_testbench/CMakeFiles/timer_test.elf.dir/flags.make

testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj: testing/timer_testbench/CMakeFiles/timer_test.elf.dir/flags.make
testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj: ../testing/timer_testbench/timer_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/testing/timer_testbench && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/timer_test.elf.dir/timer_test.c.obj   -c /home/ame/cleanProject/automatic-farm/software/testing/timer_testbench/timer_test.c

testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/timer_test.elf.dir/timer_test.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/testing/timer_testbench && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/testing/timer_testbench/timer_test.c > CMakeFiles/timer_test.elf.dir/timer_test.c.i

testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/timer_test.elf.dir/timer_test.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/testing/timer_testbench && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/testing/timer_testbench/timer_test.c -o CMakeFiles/timer_test.elf.dir/timer_test.c.s

testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj.requires:

.PHONY : testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj.requires

testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj.provides: testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj.requires
	$(MAKE) -f testing/timer_testbench/CMakeFiles/timer_test.elf.dir/build.make testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj.provides.build
.PHONY : testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj.provides

testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj.provides.build: testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj


# Object files for target timer_test.elf
timer_test_elf_OBJECTS = \
"CMakeFiles/timer_test.elf.dir/timer_test.c.obj"

# External object files for target timer_test.elf
timer_test_elf_EXTERNAL_OBJECTS =

testing/timer_testbench/timer_test.elf: testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj
testing/timer_testbench/timer_test.elf: testing/timer_testbench/CMakeFiles/timer_test.elf.dir/build.make
testing/timer_testbench/timer_test.elf: libraries/drivers/uart/libuart.a
testing/timer_testbench/timer_test.elf: libraries/drivers/timer/libtimer.a
testing/timer_testbench/timer_test.elf: testing/timer_testbench/CMakeFiles/timer_test.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable timer_test.elf"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/testing/timer_testbench && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/timer_test.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
testing/timer_testbench/CMakeFiles/timer_test.elf.dir/build: testing/timer_testbench/timer_test.elf

.PHONY : testing/timer_testbench/CMakeFiles/timer_test.elf.dir/build

testing/timer_testbench/CMakeFiles/timer_test.elf.dir/requires: testing/timer_testbench/CMakeFiles/timer_test.elf.dir/timer_test.c.obj.requires

.PHONY : testing/timer_testbench/CMakeFiles/timer_test.elf.dir/requires

testing/timer_testbench/CMakeFiles/timer_test.elf.dir/clean:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/testing/timer_testbench && $(CMAKE_COMMAND) -P CMakeFiles/timer_test.elf.dir/cmake_clean.cmake
.PHONY : testing/timer_testbench/CMakeFiles/timer_test.elf.dir/clean

testing/timer_testbench/CMakeFiles/timer_test.elf.dir/depend:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ame/cleanProject/automatic-farm/software /home/ame/cleanProject/automatic-farm/software/testing/timer_testbench /home/ame/cleanProject/automatic-farm/software/buildDir /home/ame/cleanProject/automatic-farm/software/buildDir/testing/timer_testbench /home/ame/cleanProject/automatic-farm/software/buildDir/testing/timer_testbench/CMakeFiles/timer_test.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : testing/timer_testbench/CMakeFiles/timer_test.elf.dir/depend
