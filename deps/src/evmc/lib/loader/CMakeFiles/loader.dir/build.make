# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc

# Include any dependencies generated for this target.
include lib/loader/CMakeFiles/loader.dir/depend.make

# Include the progress variables for this target.
include lib/loader/CMakeFiles/loader.dir/progress.make

# Include the compile flags for this target's objects.
include lib/loader/CMakeFiles/loader.dir/flags.make

lib/loader/CMakeFiles/loader.dir/loader.c.o: lib/loader/CMakeFiles/loader.dir/flags.make
lib/loader/CMakeFiles/loader.dir/loader.c.o: lib/loader/loader.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/loader/CMakeFiles/loader.dir/loader.c.o"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/loader.dir/loader.c.o   -c /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader/loader.c

lib/loader/CMakeFiles/loader.dir/loader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/loader.dir/loader.c.i"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader/loader.c > CMakeFiles/loader.dir/loader.c.i

lib/loader/CMakeFiles/loader.dir/loader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/loader.dir/loader.c.s"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader/loader.c -o CMakeFiles/loader.dir/loader.c.s

# Object files for target loader
loader_OBJECTS = \
"CMakeFiles/loader.dir/loader.c.o"

# External object files for target loader
loader_EXTERNAL_OBJECTS =

lib/loader/libevmc-loader.a: lib/loader/CMakeFiles/loader.dir/loader.c.o
lib/loader/libevmc-loader.a: lib/loader/CMakeFiles/loader.dir/build.make
lib/loader/libevmc-loader.a: lib/loader/CMakeFiles/loader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libevmc-loader.a"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader && $(CMAKE_COMMAND) -P CMakeFiles/loader.dir/cmake_clean_target.cmake
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/loader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/loader/CMakeFiles/loader.dir/build: lib/loader/libevmc-loader.a

.PHONY : lib/loader/CMakeFiles/loader.dir/build

lib/loader/CMakeFiles/loader.dir/clean:
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader && $(CMAKE_COMMAND) -P CMakeFiles/loader.dir/cmake_clean.cmake
.PHONY : lib/loader/CMakeFiles/loader.dir/clean

lib/loader/CMakeFiles/loader.dir/depend:
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/evmc/lib/loader/CMakeFiles/loader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/loader/CMakeFiles/loader.dir/depend

