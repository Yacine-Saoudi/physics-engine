# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.25.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.25.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yacine/Documents/engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yacine/Documents/engine/build

# Include any dependencies generated for this target.
include CMakeFiles/PhysEng.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/PhysEng.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/PhysEng.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PhysEng.dir/flags.make

CMakeFiles/PhysEng.dir/source/main.cpp.o: CMakeFiles/PhysEng.dir/flags.make
CMakeFiles/PhysEng.dir/source/main.cpp.o: /Users/yacine/Documents/engine/source/main.cpp
CMakeFiles/PhysEng.dir/source/main.cpp.o: CMakeFiles/PhysEng.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yacine/Documents/engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PhysEng.dir/source/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PhysEng.dir/source/main.cpp.o -MF CMakeFiles/PhysEng.dir/source/main.cpp.o.d -o CMakeFiles/PhysEng.dir/source/main.cpp.o -c /Users/yacine/Documents/engine/source/main.cpp

CMakeFiles/PhysEng.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PhysEng.dir/source/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yacine/Documents/engine/source/main.cpp > CMakeFiles/PhysEng.dir/source/main.cpp.i

CMakeFiles/PhysEng.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PhysEng.dir/source/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yacine/Documents/engine/source/main.cpp -o CMakeFiles/PhysEng.dir/source/main.cpp.s

# Object files for target PhysEng
PhysEng_OBJECTS = \
"CMakeFiles/PhysEng.dir/source/main.cpp.o"

# External object files for target PhysEng
PhysEng_EXTERNAL_OBJECTS =

PhysEng: CMakeFiles/PhysEng.dir/source/main.cpp.o
PhysEng: CMakeFiles/PhysEng.dir/build.make
PhysEng: /opt/homebrew/lib/libSDL2.dylib
PhysEng: CMakeFiles/PhysEng.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yacine/Documents/engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable PhysEng"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PhysEng.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PhysEng.dir/build: PhysEng
.PHONY : CMakeFiles/PhysEng.dir/build

CMakeFiles/PhysEng.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PhysEng.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PhysEng.dir/clean

CMakeFiles/PhysEng.dir/depend:
	cd /Users/yacine/Documents/engine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yacine/Documents/engine /Users/yacine/Documents/engine /Users/yacine/Documents/engine/build /Users/yacine/Documents/engine/build /Users/yacine/Documents/engine/build/CMakeFiles/PhysEng.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PhysEng.dir/depend

