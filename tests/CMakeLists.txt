# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

option(LIBPERM_TESTS "Whether to also build the libterm tests" ON)


# Helper function for registering test-cases
function(register_test)
	set(options "")
	set(oneValueArgs "")
	set(multiValueArgs TARGETS)

	cmake_parse_arguments(LIBPERM_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if(LIBPERM_TEST_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "Unrecognized arguments: ${LIBPERM_TEST_UNPARSED_ARGUMENTS}")
	endif()

	foreach(CURRENT IN LISTS LIBPERM_TEST_TARGETS)
		if(NOT TARGET "${CURRENT}")
			message(FATAL_ERROR "\"${CURRENT} is not a target")
		endif()

		target_link_libraries("${CURRENT}" PRIVATE gmock gtest_main)
		gtest_discover_tests("${CURRENT}")
	endforeach()
endfunction()


if(LIBPERM_TESTS)
	# Setup Google test
	include(FetchContent)
	FetchContent_Declare(
		GoogleTest
		GIT_REPOSITORY https://github.com/google/googletest
		GIT_TAG v1.12.0
	)
	FetchContent_MakeAvailable(GoogleTest)

	include(GoogleTest)

	add_subdirectory(ExplicitPermutation)
	add_subdirectory(Cycle)
endif()
