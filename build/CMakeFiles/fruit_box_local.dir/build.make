# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eqzou/eqzou/fruitbox

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eqzou/eqzou/fruitbox/build

# Include any dependencies generated for this target.
include CMakeFiles/fruit_box_local.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/fruit_box_local.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/fruit_box_local.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fruit_box_local.dir/flags.make

CMakeFiles/fruit_box_local.dir/src/main.cpp.o: CMakeFiles/fruit_box_local.dir/flags.make
CMakeFiles/fruit_box_local.dir/src/main.cpp.o: /home/eqzou/eqzou/fruitbox/src/main.cpp
CMakeFiles/fruit_box_local.dir/src/main.cpp.o: CMakeFiles/fruit_box_local.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/eqzou/eqzou/fruitbox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fruit_box_local.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/fruit_box_local.dir/src/main.cpp.o -MF CMakeFiles/fruit_box_local.dir/src/main.cpp.o.d -o CMakeFiles/fruit_box_local.dir/src/main.cpp.o -c /home/eqzou/eqzou/fruitbox/src/main.cpp

CMakeFiles/fruit_box_local.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/fruit_box_local.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eqzou/eqzou/fruitbox/src/main.cpp > CMakeFiles/fruit_box_local.dir/src/main.cpp.i

CMakeFiles/fruit_box_local.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/fruit_box_local.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eqzou/eqzou/fruitbox/src/main.cpp -o CMakeFiles/fruit_box_local.dir/src/main.cpp.s

# Object files for target fruit_box_local
fruit_box_local_OBJECTS = \
"CMakeFiles/fruit_box_local.dir/src/main.cpp.o"

# External object files for target fruit_box_local
fruit_box_local_EXTERNAL_OBJECTS =

fruit_box_local: CMakeFiles/fruit_box_local.dir/src/main.cpp.o
fruit_box_local: CMakeFiles/fruit_box_local.dir/build.make
fruit_box_local: /usr/lib/x86_64-linux-gnu/libsfml-graphics.so.2.6.1
fruit_box_local: /usr/lib/x86_64-linux-gnu/libsfml-window.so.2.6.1
fruit_box_local: /usr/lib/x86_64-linux-gnu/libsfml-system.so.2.6.1
fruit_box_local: CMakeFiles/fruit_box_local.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/eqzou/eqzou/fruitbox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fruit_box_local"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fruit_box_local.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fruit_box_local.dir/build: fruit_box_local
.PHONY : CMakeFiles/fruit_box_local.dir/build

CMakeFiles/fruit_box_local.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fruit_box_local.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fruit_box_local.dir/clean

CMakeFiles/fruit_box_local.dir/depend:
	cd /home/eqzou/eqzou/fruitbox/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eqzou/eqzou/fruitbox /home/eqzou/eqzou/fruitbox /home/eqzou/eqzou/fruitbox/build /home/eqzou/eqzou/fruitbox/build /home/eqzou/eqzou/fruitbox/build/CMakeFiles/fruit_box_local.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/fruit_box_local.dir/depend

