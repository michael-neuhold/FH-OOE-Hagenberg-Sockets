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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/build

# Include any dependencies generated for this target.
include CMakeFiles/src.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/src.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/src.dir/flags.make

CMakeFiles/src.dir/src/WordCheck.c.o: CMakeFiles/src.dir/flags.make
CMakeFiles/src.dir/src/WordCheck.c.o: ../src/WordCheck.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/src.dir/src/WordCheck.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/src.dir/src/WordCheck.c.o   -c /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/src/WordCheck.c

CMakeFiles/src.dir/src/WordCheck.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/src.dir/src/WordCheck.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/src/WordCheck.c > CMakeFiles/src.dir/src/WordCheck.c.i

CMakeFiles/src.dir/src/WordCheck.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/src.dir/src/WordCheck.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/src/WordCheck.c -o CMakeFiles/src.dir/src/WordCheck.c.s

CMakeFiles/src.dir/src/main.c.o: CMakeFiles/src.dir/flags.make
CMakeFiles/src.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/src.dir/src/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/src.dir/src/main.c.o   -c /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/src/main.c

CMakeFiles/src.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/src.dir/src/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/src/main.c > CMakeFiles/src.dir/src/main.c.i

CMakeFiles/src.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/src.dir/src/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/src/main.c -o CMakeFiles/src.dir/src/main.c.s

# Object files for target src
src_OBJECTS = \
"CMakeFiles/src.dir/src/WordCheck.c.o" \
"CMakeFiles/src.dir/src/main.c.o"

# External object files for target src
src_EXTERNAL_OBJECTS =

bin/src: CMakeFiles/src.dir/src/WordCheck.c.o
bin/src: CMakeFiles/src.dir/src/main.c.o
bin/src: CMakeFiles/src.dir/build.make
bin/src: CMakeFiles/src.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable bin/src"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/src.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/src.dir/build: bin/src

.PHONY : CMakeFiles/src.dir/build

CMakeFiles/src.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/src.dir/cmake_clean.cmake
.PHONY : CMakeFiles/src.dir/clean

CMakeFiles/src.dir/depend:
	cd /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01 /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01 /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/build /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/build /Users/michaelneuhold/Documents/FH/Semester/03_Semester/01_NET_UE/Socket_Programming_01/build/CMakeFiles/src.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/src.dir/depend

