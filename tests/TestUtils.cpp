// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/AbstractPermutation.hpp"
#include "libperm/ExplicitPermutation.hpp"
#include "libperm/Utils.hpp"

#include <gtest/gtest.h>

#include <vector>

TEST(Utils, computeSortPermutation) {
	std::vector< int > sequence = { -1, 5, 78, -34, 0 };

	ASSERT_EQ(perm::computeSortPermutation(sequence), perm::ExplicitPermutation({ 3, 0, 4, 1, 2 }));
	ASSERT_EQ(perm::computeSortPermutation(sequence, std::greater< int >{}),
			  perm::ExplicitPermutation({ 2, 1, 4, 0, 3 }));

	sequence = {};
	ASSERT_EQ(perm::computeSortPermutation(sequence), perm::ExplicitPermutation());

	sequence = { 42 };
	ASSERT_EQ(perm::computeSortPermutation(sequence), perm::ExplicitPermutation());
}

struct UtilsTest : testing::TestWithParam< std::vector< perm::AbstractPermutation::value_type > > {
	using ParamPack = std::vector< perm::AbstractPermutation::value_type >;
};

TEST_P(UtilsTest, applyPermutation) {
	const std::vector< perm::AbstractPermutation::value_type > mainSequence = { 0, 1, 2, 3, 4, 5, 6 };

	const perm::ExplicitPermutation permutation(GetParam());
	const perm::AbstractPermutation &abstractPerm = permutation;

	std::vector< perm::AbstractPermutation::value_type > expectedSequence(mainSequence.size());
	for (std::size_t i = 0; i < mainSequence.size(); ++i) {
		expectedSequence[i] = mainSequence[permutation.image(static_cast< perm::AbstractPermutation::value_type >(i))];
	}

	auto sequence = mainSequence;
	perm::applyPermutation(sequence, permutation);
	ASSERT_EQ(sequence, expectedSequence);

	sequence = mainSequence;
	perm::applyPermutation(sequence, abstractPerm);
	ASSERT_EQ(sequence, expectedSequence);
}

INSTANTIATE_TEST_SUITE_P(Utils, UtilsTest,
						 ::testing::Values(UtilsTest::ParamPack{ 3, 1, 2, 0 }, UtilsTest::ParamPack{ 2, 0, 1 },
										   UtilsTest::ParamPack{ 3, 2, 5, 0, 1, 4 }, UtilsTest::ParamPack{ 0 },
										   UtilsTest::ParamPack{ 0, 1, 2, 3, 6, 5, 4 },
										   UtilsTest::ParamPack{ 3, 1, 0, 2 }));
