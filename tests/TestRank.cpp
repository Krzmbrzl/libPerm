// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/ExplicitPermutation.hpp>
#include <libperm/Rank.hpp>
#include <libperm/details/Math.hpp>

#include <gtest/gtest.h>

#include <utility>

struct RankTest
	: ::testing::TestWithParam< std::pair< std::size_t, std::vector< perm::ExplicitPermutation::image_type > > > {
	using ParamPack = std::pair< std::size_t, std::vector< perm::ExplicitPermutation::image_type > >;
};

TEST_P(RankTest, rank) {
	auto [expected_rank, image] = GetParam();

	ASSERT_EQ(expected_rank, rank(perm::ExplicitPermutation(image), image.size()));
}

TEST_P(RankTest, unrank) {
	auto [rank, image] = GetParam();

	perm::ExplicitPermutation expected_perm(image);

	ASSERT_EQ(expected_perm, perm::unrank(rank, image.size()));
}

// Test cases from doi.org/10.1016/S0020-0190(01)00141-7
INSTANTIATE_TEST_SUITE_P(
	Rank, RankTest,
	::testing::Values(RankTest::ParamPack(0, { 1, 2, 3, 0 }), RankTest::ParamPack(1, { 3, 2, 0, 1 }),
					  RankTest::ParamPack(2, { 1, 3, 0, 2 }), RankTest::ParamPack(3, { 1, 2, 0, 3 }),
					  RankTest::ParamPack(4, { 2, 3, 1, 0 }), RankTest::ParamPack(5, { 2, 0, 3, 1 }),
					  RankTest::ParamPack(12, { 2, 1, 3, 0 }), RankTest::ParamPack(17, { 0, 2, 3, 1 }),
					  RankTest::ParamPack(21, { 0, 3, 2, 1 }), RankTest::ParamPack(23, { 0, 1, 2, 3 })));

TEST(RankTest, consistency) {
	for (std::size_t num_elements = 0; num_elements <= 8; ++num_elements) {
		for (std::size_t rank = 0; rank < perm::details::factorial(num_elements); ++rank) {
			const perm::Permutation perm = perm::unrank(rank, num_elements);

			ASSERT_EQ(rank, perm::rank(perm, num_elements)) << "num_elements = " << num_elements << " rank = " << rank;
		}
	}
}
