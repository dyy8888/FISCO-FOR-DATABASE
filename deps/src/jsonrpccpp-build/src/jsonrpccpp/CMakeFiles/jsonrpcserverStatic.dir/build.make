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
CMAKE_SOURCE_DIR = /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build

# Include any dependencies generated for this target.
include src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/depend.make

# Include the progress variables for this target.
include src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/progress.make

# Include the compile flags for this target's objects.
include src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/flags.make

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.o: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/flags.make
src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.o: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractprotocolhandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.o"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.o -c /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractprotocolhandler.cpp

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.i"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractprotocolhandler.cpp > CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.i

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.s"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractprotocolhandler.cpp -o CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.s

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.o: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/flags.make
src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.o: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractserverconnector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.o"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.o -c /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractserverconnector.cpp

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.i"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractserverconnector.cpp > CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.i

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.s"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/abstractserverconnector.cpp -o CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.s

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.o: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/flags.make
src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.o: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/requesthandlerfactory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.o"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.o -c /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/requesthandlerfactory.cpp

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.i"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/requesthandlerfactory.cpp > CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.i

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.s"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/requesthandlerfactory.cpp -o CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.s

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.o: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/flags.make
src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.o: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserver12.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.o"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.o -c /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserver12.cpp

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.i"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserver12.cpp > CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.i

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.s"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserver12.cpp -o CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.s

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.o: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/flags.make
src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.o: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserverv1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.o"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.o -c /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserverv1.cpp

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.i"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserverv1.cpp > CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.i

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.s"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserverv1.cpp -o CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.s

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.o: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/flags.make
src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.o: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserverv2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.o"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.o -c /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserverv2.cpp

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.i"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserverv2.cpp > CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.i

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.s"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/rpcprotocolserverv2.cpp -o CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.s

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.o: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/flags.make
src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.o: /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/connectors/httpserver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.o"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.o -c /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/connectors/httpserver.cpp

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.i"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/connectors/httpserver.cpp > CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.i

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.s"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp/server/connectors/httpserver.cpp -o CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.s

# Object files for target jsonrpcserverStatic
jsonrpcserverStatic_OBJECTS = \
"CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.o" \
"CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.o" \
"CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.o" \
"CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.o" \
"CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.o" \
"CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.o" \
"CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.o"

# External object files for target jsonrpcserverStatic
jsonrpcserverStatic_EXTERNAL_OBJECTS =

lib/libjsonrpccpp-server.a: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractprotocolhandler.cpp.o
lib/libjsonrpccpp-server.a: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/abstractserverconnector.cpp.o
lib/libjsonrpccpp-server.a: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/requesthandlerfactory.cpp.o
lib/libjsonrpccpp-server.a: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserver12.cpp.o
lib/libjsonrpccpp-server.a: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv1.cpp.o
lib/libjsonrpccpp-server.a: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/rpcprotocolserverv2.cpp.o
lib/libjsonrpccpp-server.a: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/server/connectors/httpserver.cpp.o
lib/libjsonrpccpp-server.a: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/build.make
lib/libjsonrpccpp-server.a: src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library ../../lib/libjsonrpccpp-server.a"
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && $(CMAKE_COMMAND) -P CMakeFiles/jsonrpcserverStatic.dir/cmake_clean_target.cmake
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jsonrpcserverStatic.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/build: lib/libjsonrpccpp-server.a

.PHONY : src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/build

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/clean:
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp && $(CMAKE_COMMAND) -P CMakeFiles/jsonrpcserverStatic.dir/cmake_clean.cmake
.PHONY : src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/clean

src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/depend:
	cd /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp/src/jsonrpccpp /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp /home/dyy/FISCO-TIDY/FISCO-BCOS-2.9.1/deps/src/jsonrpccpp-build/src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/jsonrpccpp/CMakeFiles/jsonrpcserverStatic.dir/depend

