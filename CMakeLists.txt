cmake_minimum_required (VERSION 2.6)
project (reggie)

# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(TEST "Build all tests." ON) # Makes boolean 'test' available.

# Default build config
if (NOT CMAKE_BUILD_TYPE)
#  message(STATUS "No build type selected, default to Release")
#  set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_BUILD_TYPE "Debug")
endif()

# Some compiler flags
SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11" )
set( CMAKE_C_FLAGS_DEBUG "-O2 -Wall -Wextra -pedantic -g" )
set( CMAKE_C_FLAGS_RELEASE "-O2 -Wall -fomit-frame-pointer -DNDEBUG" )
set( CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} -g" )

################################
# Header Files
################################
include_directories(${reggie_SOURCE_DIR}/include/)

################################
# Source Files
################################
add_subdirectory(src/Collection)
add_subdirectory(src/Reggie)

################################
# Unit Tests
################################
# This has to be before any add_subdirectory,
# so we can pick up the add_test inside of them.

if(TEST)
	enable_testing()
	add_subdirectory(testsrc)
endif()


