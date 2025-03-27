# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

include(CheckIPOSupported)
include(CompilerFlags)

check_ipo_supported(RESULT LTO_DEFAULT)


option(LIBPERM_LTO "Whether to use link-time optimizations (if available)" ${LTO_DEFAULT})
if (PROJECT_IS_TOP_LEVEL)
	set(DEFAULT_DISABLE_WARNINGS OFF)
else()
	set(DEFAULT_DISABLE_WARNINGS ON)
endif()
option(LIBPERM_DISABLE_WARNINGS "Whether to disable compiler warnings" ${DEFAULT_DISABLE_WARNINGS})
option(LIBPERM_WARNINGS_AS_ERRORS "Whether to disable compiler warnings" OFF)


# Use cpp20 and error if that is not available
if (NOT DEFINED CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 20)
	set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${LIBPERM_LTO})
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)


function(set_internal_build_flags TARGET)
	set(FLAGS_TO_ENABLE "")

	if (LIBPERM_WARNINGS_AS_ERRORS)
		list(APPEND FLAGS_TO_ENABLE "ENABLE_WARNINGS_AS_ERRORS")
	endif()
	if (LIBPERM_DISABLE_WARNINGS)
		list(APPEND FLAGS_TO_ENABLE "DISABLE_ALL_WARNINGS")
	else()
		list(APPEND FLAGS_TO_ENABLE "ENABLE_MOST_WARNINGS")
	endif()

	set_compiler_flags(
		TARGET "${TARGET}"
		${FLAGS_TO_ENABLE}
	)
endfunction()
