// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/DisjointCycles.hpp>

#include <compare>
#include <tuple>

#include <gtest/gtest.h>

TEST(DisjointCycles, construction) {
	perm::DisjointCycles c1;
	// (1,2)
	perm::DisjointCycles c2({ { 0, 1 } });
	// (1,2)(3,4)
	perm::DisjointCycles c3({ { 0, 1 }, { 2, 3 } });

	ASSERT_EQ(c1.begin(), c1.end());
	ASSERT_EQ(*c2.begin(), *c3.begin());
}

TEST(DisjointCycles, toImage) {
	// This simply represents the identity ()
	perm::DisjointCycles c1;
	perm::DisjointCycles c2({ 0, 1 });
	perm::DisjointCycles c3({ 2, 1, 3 });
	perm::DisjointCycles c4({ { 0, 1 }, { 2, 3 } });
	// This is a quirky (read: non-disjoint) way of writing down (1,2,3)
	perm::DisjointCycles c5({ { 0, 1 }, { 1, 2 } });

	std::vector< unsigned int > expectedImage = { 0 };
	ASSERT_EQ(c1.toImage< unsigned int >(), expectedImage);

	expectedImage = { 1, 0 };
	ASSERT_EQ(c2.toImage< unsigned int >(), expectedImage);

	expectedImage = { 0, 3, 1, 2 };
	ASSERT_EQ(c3.toImage< unsigned int >(), expectedImage);

	expectedImage = { 1, 0, 3, 2 };
	ASSERT_EQ(c4.toImage< unsigned int >(), expectedImage);

	expectedImage = { 1, 2, 0 };
	ASSERT_EQ(c5.toImage< unsigned int >(), expectedImage);
}

struct ComparisonTest
	: ::testing::TestWithParam< std::tuple< perm::DisjointCycles, perm::DisjointCycles, std::strong_ordering > > {
	using ParamPack = std::tuple< perm::DisjointCycles, perm::DisjointCycles, std::strong_ordering >;
};

TEST_P(ComparisonTest, compare) {
	const auto [lhs, rhs, expectedOrder] = GetParam();

	ASSERT_EQ(lhs <=> rhs, expectedOrder);
	ASSERT_EQ(perm::DisjointCycles(), perm::DisjointCycles());

	if (expectedOrder == std::strong_ordering::equal) {
		ASSERT_EQ(lhs, rhs);
		ASSERT_EQ(rhs, lhs);
		ASSERT_LE(lhs, rhs);
		ASSERT_LE(rhs, lhs);
		ASSERT_GE(lhs, rhs);
		ASSERT_GE(rhs, lhs);
	} else if (expectedOrder == std::strong_ordering::greater) {
		ASSERT_GT(lhs, rhs);
		ASSERT_LT(rhs, lhs);
	} else if (expectedOrder == std::strong_ordering::less) {
		ASSERT_LT(lhs, rhs);
		ASSERT_GT(rhs, lhs);
	}
}

INSTANTIATE_TEST_SUITE_P(
	DisjointCycles, ComparisonTest,
	::testing::Values(ComparisonTest::ParamPack({}, {}, std::strong_ordering::equal),
					  ComparisonTest::ParamPack({ { 0, 1 } }, { { 1, 0 } }, std::strong_ordering::equal),
					  ComparisonTest::ParamPack({ { 2 }, { 0, 1 } }, { { 1, 0 } }, std::strong_ordering::equal),
					  ComparisonTest::ParamPack({ { 0, 1 }, { 2, 3 } }, { { 2, 3 }, { 0, 1 } },
												std::strong_ordering::equal),
					  ComparisonTest::ParamPack({ { 0, 1 }, { 1, 2 } }, { { 0, 1, 2 } }, std::strong_ordering::equal),
					  ComparisonTest::ParamPack({ { 0, 1 } }, { { 0, 1, 2 } }, std::strong_ordering::less)));

struct FromImageTest : ::testing::TestWithParam< std::tuple< std::vector< int >, perm::DisjointCycles, std::size_t > > {
	using ParamPack = std::tuple< std::vector< int >, perm::DisjointCycles, std::size_t >;
};

TEST_P(FromImageTest, fromImage) {
	const auto [image, expectedCycle, num_elements] = GetParam();

	for (std::size_t N : std::vector< std::size_t >{ 0, num_elements }) {
		const perm::DisjointCycles actual = perm::DisjointCycles::fromImage(image, N);

		ASSERT_EQ(actual, expectedCycle);

		if (N != 0) {
			ASSERT_EQ(actual.size(), expectedCycle.size());
		}
	}
}

INSTANTIATE_TEST_SUITE_P(
	DisjointCycles, FromImageTest,
	::testing::Values(FromImageTest::ParamPack({}, { { 0 } }, 1), FromImageTest::ParamPack({}, { { 0 }, { 1 } }, 2),
					  FromImageTest::ParamPack({}, { { 0 }, { 1 }, { 2 } }, 3),
					  FromImageTest::ParamPack({}, { { 0 }, { 1 }, { 2 } }, 3),
					  FromImageTest::ParamPack({ 0, 1 }, { { 0 }, { 1 }, { 2 } }, 3),
					  FromImageTest::ParamPack({ 1, 0 }, { { 0, 1 }, { 2 }, { 3 } }, 4),
					  FromImageTest::ParamPack({ 1, 0, 2 }, { { 0, 1 }, { 2 } }, 3),
					  FromImageTest::ParamPack({ 1, 2, 0 }, { { 0, 1, 2 } }, 3),
					  FromImageTest::ParamPack({ 1, 2, 0 }, { { 0, 1, 2 }, { 3 }, { 4 }, { 5 } }, 6)));
