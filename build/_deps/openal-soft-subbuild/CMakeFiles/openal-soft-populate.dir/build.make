# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild

# Utility rule file for openal-soft-populate.

# Include any custom commands dependencies for this target.
include CMakeFiles/openal-soft-populate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/openal-soft-populate.dir/progress.make

CMakeFiles/openal-soft-populate: CMakeFiles/openal-soft-populate-complete

CMakeFiles/openal-soft-populate-complete: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-install
CMakeFiles/openal-soft-populate-complete: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-mkdir
CMakeFiles/openal-soft-populate-complete: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-download
CMakeFiles/openal-soft-populate-complete: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-update
CMakeFiles/openal-soft-populate-complete: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-patch
CMakeFiles/openal-soft-populate-complete: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-configure
CMakeFiles/openal-soft-populate-complete: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-build
CMakeFiles/openal-soft-populate-complete: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-install
CMakeFiles/openal-soft-populate-complete: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'openal-soft-populate'"
	/usr/bin/cmake -E make_directory /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles
	/usr/bin/cmake -E touch /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles/openal-soft-populate-complete
	/usr/bin/cmake -E touch /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-done

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-update:
.PHONY : openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-update

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-build: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'openal-soft-populate'"
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-build && /usr/bin/cmake -E echo_append
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-build && /usr/bin/cmake -E touch /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-build

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-configure: openal-soft-populate-prefix/tmp/openal-soft-populate-cfgcmd.txt
openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-configure: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'openal-soft-populate'"
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-build && /usr/bin/cmake -E echo_append
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-build && /usr/bin/cmake -E touch /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-configure

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-download: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-gitinfo.txt
openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-download: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'openal-soft-populate'"
	cd /home/lauri/Documents/inteplas/build/_deps && /usr/bin/cmake -P /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/tmp/openal-soft-populate-gitclone.cmake
	cd /home/lauri/Documents/inteplas/build/_deps && /usr/bin/cmake -E touch /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-download

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-install: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'openal-soft-populate'"
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-build && /usr/bin/cmake -E echo_append
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-build && /usr/bin/cmake -E touch /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-install

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'openal-soft-populate'"
	/usr/bin/cmake -E make_directory /home/lauri/Documents/inteplas/build/_deps/openal-soft-src
	/usr/bin/cmake -E make_directory /home/lauri/Documents/inteplas/build/_deps/openal-soft-build
	/usr/bin/cmake -E make_directory /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix
	/usr/bin/cmake -E make_directory /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/tmp
	/usr/bin/cmake -E make_directory /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp
	/usr/bin/cmake -E make_directory /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src
	/usr/bin/cmake -E make_directory /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp
	/usr/bin/cmake -E touch /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-mkdir

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-patch: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'openal-soft-populate'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-patch

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-update:
.PHONY : openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-update

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-test: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'openal-soft-populate'"
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-build && /usr/bin/cmake -E echo_append
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-build && /usr/bin/cmake -E touch /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-test

openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-update: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Performing update step for 'openal-soft-populate'"
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-src && /usr/bin/cmake -P /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/openal-soft-populate-prefix/tmp/openal-soft-populate-gitupdate.cmake

openal-soft-populate: CMakeFiles/openal-soft-populate
openal-soft-populate: CMakeFiles/openal-soft-populate-complete
openal-soft-populate: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-build
openal-soft-populate: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-configure
openal-soft-populate: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-download
openal-soft-populate: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-install
openal-soft-populate: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-mkdir
openal-soft-populate: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-patch
openal-soft-populate: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-test
openal-soft-populate: openal-soft-populate-prefix/src/openal-soft-populate-stamp/openal-soft-populate-update
openal-soft-populate: CMakeFiles/openal-soft-populate.dir/build.make
.PHONY : openal-soft-populate

# Rule to build all files generated by this target.
CMakeFiles/openal-soft-populate.dir/build: openal-soft-populate
.PHONY : CMakeFiles/openal-soft-populate.dir/build

CMakeFiles/openal-soft-populate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/openal-soft-populate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/openal-soft-populate.dir/clean

CMakeFiles/openal-soft-populate.dir/depend:
	cd /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild /home/lauri/Documents/inteplas/build/_deps/openal-soft-subbuild/CMakeFiles/openal-soft-populate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/openal-soft-populate.dir/depend

