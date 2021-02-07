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
include libraries/drivers/wdg/CMakeFiles/wdg.dir/depend.make

# Include the progress variables for this target.
include libraries/drivers/wdg/CMakeFiles/wdg.dir/progress.make

# Include the compile flags for this target's objects.
include libraries/drivers/wdg/CMakeFiles/wdg.dir/flags.make

libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj: libraries/drivers/wdg/CMakeFiles/wdg.dir/flags.make
libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj: ../libraries/drivers/wdg/src/wdg.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/wdg && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/wdg.dir/src/wdg.c.obj   -c /home/ame/cleanProject/automatic-farm/software/libraries/drivers/wdg/src/wdg.c

libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/wdg.dir/src/wdg.c.i"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/wdg && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ame/cleanProject/automatic-farm/software/libraries/drivers/wdg/src/wdg.c > CMakeFiles/wdg.dir/src/wdg.c.i

libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/wdg.dir/src/wdg.c.s"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/wdg && /home/ame/git_projects/automatic-farm/tools/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ame/cleanProject/automatic-farm/software/libraries/drivers/wdg/src/wdg.c -o CMakeFiles/wdg.dir/src/wdg.c.s

libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj.requires:

.PHONY : libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj.requires

libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj.provides: libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj.requires
	$(MAKE) -f libraries/drivers/wdg/CMakeFiles/wdg.dir/build.make libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj.provides.build
.PHONY : libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj.provides

libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj.provides.build: libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj


# Object files for target wdg
wdg_OBJECTS = \
"CMakeFiles/wdg.dir/src/wdg.c.obj"

# External object files for target wdg
wdg_EXTERNAL_OBJECTS =

libraries/drivers/wdg/libwdg.a: libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj
libraries/drivers/wdg/libwdg.a: libraries/drivers/wdg/CMakeFiles/wdg.dir/build.make
libraries/drivers/wdg/libwdg.a: libraries/drivers/wdg/CMakeFiles/wdg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ame/cleanProject/automatic-farm/software/buildDir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libwdg.a"
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/wdg && $(CMAKE_COMMAND) -P CMakeFiles/wdg.dir/cmake_clean_target.cmake
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/wdg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wdg.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libraries/drivers/wdg/CMakeFiles/wdg.dir/build: libraries/drivers/wdg/libwdg.a

.PHONY : libraries/drivers/wdg/CMakeFiles/wdg.dir/build

libraries/drivers/wdg/CMakeFiles/wdg.dir/requires: libraries/drivers/wdg/CMakeFiles/wdg.dir/src/wdg.c.obj.requires

.PHONY : libraries/drivers/wdg/CMakeFiles/wdg.dir/requires

libraries/drivers/wdg/CMakeFiles/wdg.dir/clean:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/wdg && $(CMAKE_COMMAND) -P CMakeFiles/wdg.dir/cmake_clean.cmake
.PHONY : libraries/drivers/wdg/CMakeFiles/wdg.dir/clean

libraries/drivers/wdg/CMakeFiles/wdg.dir/depend:
	cd /home/ame/cleanProject/automatic-farm/software/buildDir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ame/cleanProject/automatic-farm/software /home/ame/cleanProject/automatic-farm/software/libraries/drivers/wdg /home/ame/cleanProject/automatic-farm/software/buildDir /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/wdg /home/ame/cleanProject/automatic-farm/software/buildDir/libraries/drivers/wdg/CMakeFiles/wdg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libraries/drivers/wdg/CMakeFiles/wdg.dir/depend
