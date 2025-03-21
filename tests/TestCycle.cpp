// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/Cycle.hpp>

#include <compare>
#include <tuple>

#include <gtest/gtest.h>

TEST(Cycle, construction) {
	perm::Cycle c1;
	ASSERT_EQ(c1.size(), 1);

	perm::Cycle c2({ 4 });
	ASSERT_EQ(c2.size(), 1);
	ASSERT_EQ(c2[0], 4);

	perm::Cycle c3({ 1, 2, 3 });
	ASSERT_EQ(c3.size(), 3);
}

TEST(Cycle, canonicalOrder) {
	perm::Cycle c1({ 1, 2, 3 });
	ASSERT_EQ(c1.at(0), 3);
	ASSERT_EQ(c1.at(1), 1);
	ASSERT_EQ(c1.at(2), 2);

	perm::Cycle c2({ 4, 3, 5, 1 });
	ASSERT_EQ(c2.at(0), 5);
	ASSERT_EQ(c2.at(1), 1);
	ASSERT_EQ(c2.at(2), 4);
	ASSERT_EQ(c2.at(3), 3);

	perm::Cycle c3({ 3, 2 });
	ASSERT_EQ(c3.at(0), 3);
	ASSERT_EQ(c3.at(1), 2);
}

struct CycleTest : ::testing::TestWithParam< std::tuple< perm::Cycle, perm::Cycle, std::strong_ordering > > {
	using ParamPack = std::tuple< perm::Cycle, perm::Cycle, std::strong_ordering >;
};

TEST_P(CycleTest, comparison) {
	const auto [lhs, rhs, expectedOrder] = GetParam();

	ASSERT_EQ(lhs <=> rhs, expectedOrder);

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

INSTANTIATE_TEST_SUITE_P(Cycle, CycleTest,
						 ::testing::Values(CycleTest::ParamPack({ 0, 1 }, { 1, 0 }, std::strong_ordering::equal),
										   CycleTest::ParamPack({ 0, 1, 2 }, { 2, 1, 0 }, std::strong_ordering::less),
										   CycleTest::ParamPack({ 3, 0, 2, 1 }, { 0, 1, 2, 3 },
																std::strong_ordering::greater)));
