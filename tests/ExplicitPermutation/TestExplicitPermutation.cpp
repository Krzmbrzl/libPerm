// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/details/ExplicitPermutation.hpp>

#include "PermutationTest.hpp"

#include <gtest/gtest.h>

TEST(ExplicitPermutation, construction) {
	perm::ExplicitPermutation perm(4);

	ASSERT_EQ(perm.order(), 4);

	ASSERT_TRUE(perm.isIdentity());

	perm = perm::ExplicitPermutation({ 1, 0, 2 });

	ASSERT_EQ(perm.order(), 3);
	std::vector< perm::Permutation::value_type > expectedImage = { 1, 0, 2 };
	ASSERT_EQ(perm.image(), expectedImage);
}

TEST(ExplicitPermutation, equality) {
	perm::ExplicitPermutation p1(4);
	perm::ExplicitPermutation p2(4);
	perm::ExplicitPermutation p3({ 1, 2, 0, 3 });
	perm::ExplicitPermutation p4({ 0, 1, 2, 3 });

	ASSERT_EQ(p1, p2);
	ASSERT_NE(p1, p3);
	ASSERT_EQ(p1, p4);
}

TEST(ExplicitPermutation, multiplication) {
	perm::ExplicitPermutation id(4);
	perm::ExplicitPermutation p1({ 2, 0, 1, 3 });
	perm::ExplicitPermutation p2({ 0, 1, 3, 2 });

	perm::ExplicitPermutation r1({ 3, 0, 1, 2 });

	ASSERT_NE(id, p1);
	ASSERT_NE(p1, p2);

	// Multiplication with id is no-op
	ASSERT_EQ(id * id, id);
	ASSERT_EQ(id * p1, p1);
	ASSERT_EQ(p1 * id, p1);
	ASSERT_EQ(id * p2, p2);
	ASSERT_EQ(p2 * id, p2);

	ASSERT_EQ(p1 * p2, r1);
}

TEST(ExplicitPermutation, permutationInterface) {
	perm::test::testPermutationInterface<perm::ExplicitPermutation>();
}
