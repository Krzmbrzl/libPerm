# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

include(CheckIPOSupported)
include(CompilerFlags)

check_ipo_supported(RESULT LTO_DEFAULT)


option(LIBPERM_LTO "Whether to use link-time optimizations (if available)" ${LTO_DEFAULT})
option(LIBPERM_DISABLE_WARNINGS "Whether to disable compiler warnings" OFF)
option(LIBPERM_WARNINGS_AS_ERRORS "Whether to disable compiler warnings" OFF)


# Use cpp17 and error if that is not available
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ${LIBPERM_LTO})
cmake_policy(SET CMP0069 NEW)


# Configure warning-related options
set(FLAGS_TO_ENABLE "ENABLE_MOST_WARNINGS")
if (LIBPERM_WARNINGS_AS_ERRORS)
	list(APPEND FLAGS_TO_ENABLE "ENABLE_WARNINGS_AS_ERRORS")
endif()
if (LIBPERM_DISABLE_WARNINGS)
	list(APPEND FLAGS_TO_ENABLE "DISABLE_ALL_WARNINGS")
endif()

get_compiler_flags(
	${FLAGS_TO_ENABLE}
	OUTPUT_VARIABLE COMPILER_FLAGS
)

add_compile_options(${COMPILER_FLAGS})
