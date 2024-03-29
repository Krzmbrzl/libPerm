# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

include(FetchContent)

FetchContent_Declare(
	cmake_compiler_flags
	GIT_REPOSITORY https://github.com/Krzmbrzl/cmake-compiler-flags.git
	GIT_TAG        v1.1.0
	GIT_SHALLOW    true
)

FetchContent_Declare(
	polymorphic_variant
	GIT_REPOSITORY https://github.com/Krzmbrzl/polymorphic_variant.git
	GIT_TAG        v1.3.0
	GIT_SHALLOW    true
)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest
	GIT_TAG        release-1.12.1
	GIT_SHALLOW    true
)

message(STATUS "Fetching and building dependencies...")
FetchContent_MakeAvailable(cmake_compiler_flags polymorphic_variant)
message(STATUS "Done fetching dependencies")

# Append the compiler flags CMake module to the module path
FetchContent_GetProperties(cmake_compiler_flags SOURCE_DIR COMPILER_FLAGS_SRC_DIR)
list(APPEND CMAKE_MODULE_PATH "${COMPILER_FLAGS_SRC_DIR}")
