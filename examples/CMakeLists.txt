# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

option(LIBPERM_EXAMPLES "Whether to build the examples" OFF)

if (NOT LIBPERM_EXAMPLES)
	return()
endif()

function(add_example)
	set(options "")
	set(oneValueArgs TARGET)
	set(multiValueArgs SOURCES)

	cmake_parse_arguments(LIBPERM_EXAMPLE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	add_executable(${LIBPERM_EXAMPLE_TARGET} ${LIBPERM_EXAMPLE_SOURCES})
	target_link_libraries(${LIBPERM_EXAMPLE_TARGET} PRIVATE libperm)
	set_target_properties(${LIBPERM_EXAMPLE_TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endfunction()

add_example(TARGET permutations_basics SOURCES permutations_basics.cpp)
add_example(TARGET permutations_as_value_types SOURCES permutations_as_value_types.cpp)
add_example(TARGET permutation_group_basics SOURCES permutation_group_basics.cpp)
