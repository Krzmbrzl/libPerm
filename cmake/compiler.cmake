# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

include(CheckIPOSupported)

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
if (MSVC)
	add_compile_options(/W4 /Wall)

	if (LIBPERM_DISABLE_WARNINGS)
		add_compile_options(/w)
	elseif (LIBPERM_WARNINGS_AS_ERRORS)
		add_compile_options(/WX)
	endif()
else()
	add_compile_options(-Wpedantic -Wall -Wextra)

	if (LIBPERM_DISABLE_WARNINGS)
		add_compile_options(-w)
	elseif (LIBPERM_WARNINGS_AS_ERRORS)
		add_compile_options(-Werror)
	endif()
endif()
