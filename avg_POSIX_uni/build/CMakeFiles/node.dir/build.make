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
CMAKE_SOURCE_DIR = /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/build

# Include any dependencies generated for this target.
include CMakeFiles/node.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/node.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/node.dir/flags.make

CMakeFiles/node.dir/node.c.o: CMakeFiles/node.dir/flags.make
CMakeFiles/node.dir/node.c.o: ../node.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/node.dir/node.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/node.dir/node.c.o   -c /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/node.c

CMakeFiles/node.dir/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/node.dir/node.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/node.c > CMakeFiles/node.dir/node.c.i

CMakeFiles/node.dir/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/node.dir/node.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/node.c -o CMakeFiles/node.dir/node.c.s

CMakeFiles/node.dir/node.c.o.requires:

.PHONY : CMakeFiles/node.dir/node.c.o.requires

CMakeFiles/node.dir/node.c.o.provides: CMakeFiles/node.dir/node.c.o.requires
	$(MAKE) -f CMakeFiles/node.dir/build.make CMakeFiles/node.dir/node.c.o.provides.build
.PHONY : CMakeFiles/node.dir/node.c.o.provides

CMakeFiles/node.dir/node.c.o.provides.build: CMakeFiles/node.dir/node.c.o


# Object files for target node
node_OBJECTS = \
"CMakeFiles/node.dir/node.c.o"

# External object files for target node
node_EXTERNAL_OBJECTS =

bin/node: CMakeFiles/node.dir/node.c.o
bin/node: CMakeFiles/node.dir/build.make
bin/node: CMakeFiles/node.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin/node"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/node.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/node.dir/build: bin/node

.PHONY : CMakeFiles/node.dir/build

CMakeFiles/node.dir/requires: CMakeFiles/node.dir/node.c.o.requires

.PHONY : CMakeFiles/node.dir/requires

CMakeFiles/node.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/node.dir/cmake_clean.cmake
.PHONY : CMakeFiles/node.dir/clean

CMakeFiles/node.dir/depend:
	cd /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/build /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/build /home/marcoantoniobustamante/Desktop/COMP362/MarcoBustamanteLab03/avg_POSIX_uni/build/CMakeFiles/node.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/node.dir/depend
