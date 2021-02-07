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
include libraries/modules/system-timer/CMakeFiles/system-timer.dir/depend.make

# Include the progress variables for this target.
include libraries/modules/system-timer/CMakeFiles/system-timer.dir/progress.make

# Include the compile flags for this target's objects.
include libraries/modules/system-timer/CMakeFiles/system-timer.dir/flags.make

libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj: libraries/modules/system-timer/CMakeFiles/system-timer.dir/flags.make
libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj: ../libraries/modules/system-timer/src/system-timer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/system-timer && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/system-timer.dir/src/system-timer.c.obj   -c /home/ame/cleanProject/automatic-farm/software/libraries/modules/system-timer/src/system-timer.c

libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/system-timer.dir/src/system-timer.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/system-timer && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/libraries/modules/system-timer/src/system-timer.c > CMakeFiles/system-timer.dir/src/system-timer.c.i

libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/system-timer.dir/src/system-timer.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/system-timer && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/libraries/modules/system-timer/src/system-timer.c -o CMakeFiles/system-timer.dir/src/system-timer.c.s

libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj.requires:

.PHONY : libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj.requires

libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj.provides: libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj.requires
	$(MAKE) -f libraries/modules/system-timer/CMakeFiles/system-timer.dir/build.make libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj.provides.build
.PHONY : libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj.provides

libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj.provides.build: libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj


# Object files for target system-timer
system__timer_OBJECTS = \
"CMakeFiles/system-timer.dir/src/system-timer.c.obj"

# External object files for target system-timer
system__timer_EXTERNAL_OBJECTS =

libraries/modules/system-timer/libsystem-timer.a: libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj
libraries/modules/system-timer/libsystem-timer.a: libraries/modules/system-timer/CMakeFiles/system-timer.dir/build.make
libraries/modules/system-timer/libsystem-timer.a: libraries/modules/system-timer/CMakeFiles/system-timer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libsystem-timer.a"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/system-timer && $(CMAKE_COMMAND) -P CMakeFiles/system-timer.dir/cmake_clean_target.cmake
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/system-timer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/system-timer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libraries/modules/system-timer/CMakeFiles/system-timer.dir/build: libraries/modules/system-timer/libsystem-timer.a

.PHONY : libraries/modules/system-timer/CMakeFiles/system-timer.dir/build

libraries/modules/system-timer/CMakeFiles/system-timer.dir/requires: libraries/modules/system-timer/CMakeFiles/system-timer.dir/src/system-timer.c.obj.requires

.PHONY : libraries/modules/system-timer/CMakeFiles/system-timer.dir/requires

libraries/modules/system-timer/CMakeFiles/system-timer.dir/clean:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/system-timer && $(CMAKE_COMMAND) -P CMakeFiles/system-timer.dir/cmake_clean.cmake
.PHONY : libraries/modules/system-timer/CMakeFiles/system-timer.dir/clean

libraries/modules/system-timer/CMakeFiles/system-timer.dir/depend:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ame/cleanProject/automatic-farm/software /home/ame/cleanProject/automatic-farm/software/libraries/modules/system-timer /home/ame/cleanProject/automatic-farm/software/buildDir /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/system-timer /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/modules/system-timer/CMakeFiles/system-timer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libraries/modules/system-timer/CMakeFiles/system-timer.dir/depend
