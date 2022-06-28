# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

cmake_minimum_required(VERSION 3.15)

# Default to a release build
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build ot produce")


project(libperm,
	VERSION "1.0.0"
	LANGUAGES "CXX"
)

list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(compiler)
include(CTest)

add_library(libperm STATIC)

set_target_properties(libperm PROPERTIES OUTPUT_NAME "libperm")

# Make sure that outside projects have to prefix all includes with libperm, but don't burden
# ourselves with this.
target_include_directories(libperm PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(libperm PRIVATE "${CMAKE_SOURCE_DIR}/include/libperm/")

enable_testing()

add_subdirectory(src)

add_subdirectory(tests)
