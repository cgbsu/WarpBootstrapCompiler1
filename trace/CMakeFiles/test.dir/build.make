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
CMAKE_SOURCE_DIR = /root/workdir

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workdir/trace

# Include any dependencies generated for this target.
include CMakeFiles/test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test.dir/flags.make

CMakeFiles/test.dir/Test/BooleanExpressions.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Test/BooleanExpressions.cpp.o: ../Test/BooleanExpressions.cpp
CMakeFiles/test.dir/Test/BooleanExpressions.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test.dir/Test/BooleanExpressions.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Test/BooleanExpressions.cpp.o -MF CMakeFiles/test.dir/Test/BooleanExpressions.cpp.o.d -o CMakeFiles/test.dir/Test/BooleanExpressions.cpp.o -c /root/workdir/Test/BooleanExpressions.cpp

CMakeFiles/test.dir/Test/BooleanExpressions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Test/BooleanExpressions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Test/BooleanExpressions.cpp > CMakeFiles/test.dir/Test/BooleanExpressions.cpp.i

CMakeFiles/test.dir/Test/BooleanExpressions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Test/BooleanExpressions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Test/BooleanExpressions.cpp -o CMakeFiles/test.dir/Test/BooleanExpressions.cpp.s

CMakeFiles/test.dir/Test/Conversions.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Test/Conversions.cpp.o: ../Test/Conversions.cpp
CMakeFiles/test.dir/Test/Conversions.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test.dir/Test/Conversions.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Test/Conversions.cpp.o -MF CMakeFiles/test.dir/Test/Conversions.cpp.o.d -o CMakeFiles/test.dir/Test/Conversions.cpp.o -c /root/workdir/Test/Conversions.cpp

CMakeFiles/test.dir/Test/Conversions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Test/Conversions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Test/Conversions.cpp > CMakeFiles/test.dir/Test/Conversions.cpp.i

CMakeFiles/test.dir/Test/Conversions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Test/Conversions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Test/Conversions.cpp -o CMakeFiles/test.dir/Test/Conversions.cpp.s

CMakeFiles/test.dir/Test/Decimal.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Test/Decimal.cpp.o: ../Test/Decimal.cpp
CMakeFiles/test.dir/Test/Decimal.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test.dir/Test/Decimal.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Test/Decimal.cpp.o -MF CMakeFiles/test.dir/Test/Decimal.cpp.o.d -o CMakeFiles/test.dir/Test/Decimal.cpp.o -c /root/workdir/Test/Decimal.cpp

CMakeFiles/test.dir/Test/Decimal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Test/Decimal.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Test/Decimal.cpp > CMakeFiles/test.dir/Test/Decimal.cpp.i

CMakeFiles/test.dir/Test/Decimal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Test/Decimal.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Test/Decimal.cpp -o CMakeFiles/test.dir/Test/Decimal.cpp.s

CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.o: ../Test/FunctionDeclarations.cpp
CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.o -MF CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.o.d -o CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.o -c /root/workdir/Test/FunctionDeclarations.cpp

CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Test/FunctionDeclarations.cpp > CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.i

CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Test/FunctionDeclarations.cpp -o CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.s

CMakeFiles/test.dir/Test/Main.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Test/Main.cpp.o: ../Test/Main.cpp
CMakeFiles/test.dir/Test/Main.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test.dir/Test/Main.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Test/Main.cpp.o -MF CMakeFiles/test.dir/Test/Main.cpp.o.d -o CMakeFiles/test.dir/Test/Main.cpp.o -c /root/workdir/Test/Main.cpp

CMakeFiles/test.dir/Test/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Test/Main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Test/Main.cpp > CMakeFiles/test.dir/Test/Main.cpp.i

CMakeFiles/test.dir/Test/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Test/Main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Test/Main.cpp -o CMakeFiles/test.dir/Test/Main.cpp.s

CMakeFiles/test.dir/Test/Math.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Test/Math.cpp.o: ../Test/Math.cpp
CMakeFiles/test.dir/Test/Math.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/test.dir/Test/Math.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Test/Math.cpp.o -MF CMakeFiles/test.dir/Test/Math.cpp.o.d -o CMakeFiles/test.dir/Test/Math.cpp.o -c /root/workdir/Test/Math.cpp

CMakeFiles/test.dir/Test/Math.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Test/Math.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Test/Math.cpp > CMakeFiles/test.dir/Test/Math.cpp.i

CMakeFiles/test.dir/Test/Math.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Test/Math.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Test/Math.cpp -o CMakeFiles/test.dir/Test/Math.cpp.s

CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.o: ../Test/MathematicalExpressions.cpp
CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.o -MF CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.o.d -o CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.o -c /root/workdir/Test/MathematicalExpressions.cpp

CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Test/MathematicalExpressions.cpp > CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.i

CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Test/MathematicalExpressions.cpp -o CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.s

CMakeFiles/test.dir/Test/NumericLiterals.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Test/NumericLiterals.cpp.o: ../Test/NumericLiterals.cpp
CMakeFiles/test.dir/Test/NumericLiterals.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/test.dir/Test/NumericLiterals.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Test/NumericLiterals.cpp.o -MF CMakeFiles/test.dir/Test/NumericLiterals.cpp.o.d -o CMakeFiles/test.dir/Test/NumericLiterals.cpp.o -c /root/workdir/Test/NumericLiterals.cpp

CMakeFiles/test.dir/Test/NumericLiterals.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Test/NumericLiterals.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Test/NumericLiterals.cpp > CMakeFiles/test.dir/Test/NumericLiterals.cpp.i

CMakeFiles/test.dir/Test/NumericLiterals.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Test/NumericLiterals.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Test/NumericLiterals.cpp -o CMakeFiles/test.dir/Test/NumericLiterals.cpp.s

CMakeFiles/test.dir/Test/Terms.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Test/Terms.cpp.o: ../Test/Terms.cpp
CMakeFiles/test.dir/Test/Terms.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/test.dir/Test/Terms.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Test/Terms.cpp.o -MF CMakeFiles/test.dir/Test/Terms.cpp.o.d -o CMakeFiles/test.dir/Test/Terms.cpp.o -c /root/workdir/Test/Terms.cpp

CMakeFiles/test.dir/Test/Terms.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Test/Terms.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Test/Terms.cpp > CMakeFiles/test.dir/Test/Terms.cpp.i

CMakeFiles/test.dir/Test/Terms.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Test/Terms.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Test/Terms.cpp -o CMakeFiles/test.dir/Test/Terms.cpp.s

CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.o: ../Source/Cache/BooleanExpressionsCache.cpp
CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.o -MF CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.o.d -o CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.o -c /root/workdir/Source/Cache/BooleanExpressionsCache.cpp

CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/Cache/BooleanExpressionsCache.cpp > CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.i

CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/Cache/BooleanExpressionsCache.cpp -o CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.s

CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.o: ../Source/Cache/FunctionDeclarationsCache.cpp
CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.o -MF CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.o.d -o CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.o -c /root/workdir/Source/Cache/FunctionDeclarationsCache.cpp

CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/Cache/FunctionDeclarationsCache.cpp > CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.i

CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/Cache/FunctionDeclarationsCache.cpp -o CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.s

CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.o: ../Source/Cache/MathematicalExpressionsCache.cpp
CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.o -MF CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.o.d -o CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.o -c /root/workdir/Source/Cache/MathematicalExpressionsCache.cpp

CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/Cache/MathematicalExpressionsCache.cpp > CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.i

CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/Cache/MathematicalExpressionsCache.cpp -o CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.s

CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.o: ../Source/Cache/NumericLiteralsCache.cpp
CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.o -MF CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.o.d -o CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.o -c /root/workdir/Source/Cache/NumericLiteralsCache.cpp

CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/Cache/NumericLiteralsCache.cpp > CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.i

CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/Cache/NumericLiteralsCache.cpp -o CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.s

CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.o: ../Source/Cache/TypeTermsCache.cpp
CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.o -MF CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.o.d -o CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.o -c /root/workdir/Source/Cache/TypeTermsCache.cpp

CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/Cache/TypeTermsCache.cpp > CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.i

CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/Cache/TypeTermsCache.cpp -o CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.s

CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.o: ../Source/SyntaxNodes/Call.cpp
CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.o -MF CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.o.d -o CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.o -c /root/workdir/Source/SyntaxNodes/Call.cpp

CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/SyntaxNodes/Call.cpp > CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.i

CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/SyntaxNodes/Call.cpp -o CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.s

CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.o: ../Source/SyntaxNodes/LogicalExpression.cpp
CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.o -MF CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.o.d -o CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.o -c /root/workdir/Source/SyntaxNodes/LogicalExpression.cpp

CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/SyntaxNodes/LogicalExpression.cpp > CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.i

CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/SyntaxNodes/LogicalExpression.cpp -o CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.s

CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.o: ../Source/SyntaxNodes/MathematicalExpressions.cpp
CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.o -MF CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.o.d -o CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.o -c /root/workdir/Source/SyntaxNodes/MathematicalExpressions.cpp

CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/SyntaxNodes/MathematicalExpressions.cpp > CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.i

CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/SyntaxNodes/MathematicalExpressions.cpp -o CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.s

CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.o: ../Source/SyntaxNodes/NumericLiterals.cpp
CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.o -MF CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.o.d -o CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.o -c /root/workdir/Source/SyntaxNodes/NumericLiterals.cpp

CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/SyntaxNodes/NumericLiterals.cpp > CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.i

CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/SyntaxNodes/NumericLiterals.cpp -o CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.s

CMakeFiles/test.dir/Source/filler.cpp.o: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/Source/filler.cpp.o: ../Source/filler.cpp
CMakeFiles/test.dir/Source/filler.cpp.o: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Building CXX object CMakeFiles/test.dir/Source/filler.cpp.o"
	ccache /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/Source/filler.cpp.o -MF CMakeFiles/test.dir/Source/filler.cpp.o.d -o CMakeFiles/test.dir/Source/filler.cpp.o -c /root/workdir/Source/filler.cpp

CMakeFiles/test.dir/Source/filler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/Source/filler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workdir/Source/filler.cpp > CMakeFiles/test.dir/Source/filler.cpp.i

CMakeFiles/test.dir/Source/filler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/Source/filler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workdir/Source/filler.cpp -o CMakeFiles/test.dir/Source/filler.cpp.s

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/Test/BooleanExpressions.cpp.o" \
"CMakeFiles/test.dir/Test/Conversions.cpp.o" \
"CMakeFiles/test.dir/Test/Decimal.cpp.o" \
"CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.o" \
"CMakeFiles/test.dir/Test/Main.cpp.o" \
"CMakeFiles/test.dir/Test/Math.cpp.o" \
"CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.o" \
"CMakeFiles/test.dir/Test/NumericLiterals.cpp.o" \
"CMakeFiles/test.dir/Test/Terms.cpp.o" \
"CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.o" \
"CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.o" \
"CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.o" \
"CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.o" \
"CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.o" \
"CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.o" \
"CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.o" \
"CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.o" \
"CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.o" \
"CMakeFiles/test.dir/Source/filler.cpp.o"

# External object files for target test
test_EXTERNAL_OBJECTS =

bin/test: CMakeFiles/test.dir/Test/BooleanExpressions.cpp.o
bin/test: CMakeFiles/test.dir/Test/Conversions.cpp.o
bin/test: CMakeFiles/test.dir/Test/Decimal.cpp.o
bin/test: CMakeFiles/test.dir/Test/FunctionDeclarations.cpp.o
bin/test: CMakeFiles/test.dir/Test/Main.cpp.o
bin/test: CMakeFiles/test.dir/Test/Math.cpp.o
bin/test: CMakeFiles/test.dir/Test/MathematicalExpressions.cpp.o
bin/test: CMakeFiles/test.dir/Test/NumericLiterals.cpp.o
bin/test: CMakeFiles/test.dir/Test/Terms.cpp.o
bin/test: CMakeFiles/test.dir/Source/Cache/BooleanExpressionsCache.cpp.o
bin/test: CMakeFiles/test.dir/Source/Cache/FunctionDeclarationsCache.cpp.o
bin/test: CMakeFiles/test.dir/Source/Cache/MathematicalExpressionsCache.cpp.o
bin/test: CMakeFiles/test.dir/Source/Cache/NumericLiteralsCache.cpp.o
bin/test: CMakeFiles/test.dir/Source/Cache/TypeTermsCache.cpp.o
bin/test: CMakeFiles/test.dir/Source/SyntaxNodes/Call.cpp.o
bin/test: CMakeFiles/test.dir/Source/SyntaxNodes/LogicalExpression.cpp.o
bin/test: CMakeFiles/test.dir/Source/SyntaxNodes/MathematicalExpressions.cpp.o
bin/test: CMakeFiles/test.dir/Source/SyntaxNodes/NumericLiterals.cpp.o
bin/test: CMakeFiles/test.dir/Source/filler.cpp.o
bin/test: CMakeFiles/test.dir/build.make
bin/test: lib/liblibwarp.a
bin/test: CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workdir/trace/CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Linking CXX executable bin/test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test.dir/build: bin/test
.PHONY : CMakeFiles/test.dir/build

CMakeFiles/test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test.dir/clean

CMakeFiles/test.dir/depend:
	cd /root/workdir/trace && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workdir /root/workdir /root/workdir/trace /root/workdir/trace /root/workdir/trace/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test.dir/depend

