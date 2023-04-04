# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

cmake_minimum_required(VERSION 3.15)

# Default to a release build
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build ot produce")


project(libperm,
	VERSION "1.2.0"
	LANGUAGES "CXX"
)

set(CMAKE_MODULE_PATH
	"${PROJECT_SOURCE_DIR}/cmake"
)


# Ensure that the NEW behavior for LTO is used - GoogleTest is not quite up-to-date when
# it comes to cmake versions and policies
set(CMAKE_POLICY_DEFAULT_CMP0069 "NEW" CACHE BOOL "")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(dependencies)
include(compiler)
include(CTest)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_library(libperm STATIC)
set_target_properties(libperm PROPERTIES PREFIX "")

add_library(libperm::libperm ALIAS libperm)

set(SYSTEM_KEY "")
if (LIBPERM_DISABLE_WARNINGS)
	set(SYSTEM_KEY "SYSTEM")
endif()

# Make sure that outside projects have to prefix all includes with libperm, but don't burden
# ourselves with this.
target_include_directories(libperm ${SYSTEM_KEY} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(libperm PRIVATE "${PROJECT_SOURCE_DIR}/include/libperm/")

target_link_libraries(libperm PUBLIC polymorphic_variant)

enable_testing()

add_subdirectory(src)

add_subdirectory(tests)

add_subdirectory(examples)
