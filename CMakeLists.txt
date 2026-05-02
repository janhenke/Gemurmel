cmake_minimum_required(VERSION 3.27)
project(Gemurmel VERSION 0.0.1)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Use libc++ instead of libstdc++ (Clang only)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	option(USE_LIBCPP "Use libc++ as C++ standard library" OFF)
	if (USE_LIBCPP)
		add_compile_options(-stdlib=libc++)
	endif ()
endif ()

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CheckIPOSupported)
# check for LTO support in the compiler
check_ipo_supported(RESULT LTO_SUPPORTED)
find_package(PkgConfig REQUIRED)

option(GEMURMEL_BUILD_SERVER "Build the server library" ON)
option(GEMURMEL_BUILD_TEST "Build the test executables" ON)
option(GEMURMEL_USE_BOOST_ASIO "Use Boost.Asio instead of standalone Asio" OFF)

if (${GEMURMEL_BUILD_TEST})
	enable_testing()
	find_package(Catch2 3 REQUIRED)
	include(CTest)
	include(Catch)
endif ()

add_subdirectory(mumble_protocol)
add_subdirectory(gemurmel)

if (${GEMURMEL_BUILD_SERVER})
	add_subdirectory(server)
	add_subdirectory(GemurmelServer)
endif ()
