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
		"TestUtils.cpp"
	)

	target_link_libraries(libPermTest PRIVATE gmock gtest_main libperm::libperm)

	target_include_directories(libPermTest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

	gtest_discover_tests(libPermTest)
endif()
