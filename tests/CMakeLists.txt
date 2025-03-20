# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

option(LIBPERM_TESTS "Whether to also build the libterm tests" ON)

if(LIBPERM_TESTS)
	# Setup Google test
	include(FetchContent)
	FetchContent_MakeAvailable(googletest)

	include(GoogleTest)

	add_executable(libPermTest
		"TestCycle.cpp"
		"TestDiminoAlgorithm.cpp"
		"TestExplicitPermutation.cpp"
		"TestPermutationInterface.cpp"
		"TestPermutationGroupInterface.cpp"
		"TestPrimitivePermutationGroup.cpp"
		"TestRank.cpp"
		"TestSpecialGroups.cpp"
		"TestUtils.cpp"
	)

	if (MSVC)
		# MSVC is buggy and gets confused about some template syntax when using
		# unity builds for the tests.
		set_target_properties(libPermTest PROPERTIES UNITY_BUILD OFF)
	endif()

	target_link_libraries(libPermTest PRIVATE gmock gtest_main libperm::libperm)

	target_include_directories(libPermTest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

	gtest_discover_tests(libPermTest TEST_PREFIX "libPerm/")
endif()
