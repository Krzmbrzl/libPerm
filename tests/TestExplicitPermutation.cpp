// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/ExplicitPermutation.hpp>

#include "PermutationTest.hpp"

#include <gtest/gtest.h>

TEST(ExplicitPermutation, construction) {
	perm::ExplicitPermutation perm;

	ASSERT_EQ(perm.maxElement(), static_cast< perm::AbstractPermutation::value_type >(0));
	ASSERT_TRUE(perm.isIdentity());
	ASSERT_EQ(perm.sign(), 1);

	perm = perm::ExplicitPermutation(std::vector< perm::ExplicitPermutation::value_type >{});
	ASSERT_TRUE(perm.isIdentity());
	ASSERT_EQ(perm.sign(), 1);

	perm = perm::ExplicitPermutation(-1);
	ASSERT_TRUE(!perm.isIdentity());
	ASSERT_EQ(perm.sign(), -1);

	perm = perm::ExplicitPermutation({ 1, 2, 0 });

	ASSERT_EQ(perm.maxElement(), static_cast< perm::AbstractPermutation::value_type >(2));
	std::vector< perm::AbstractPermutation::value_type > expectedImage = { 1, 2, 0 };
	ASSERT_EQ(perm.image(), expectedImage);

	perm          = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2, 3 }));
	expectedImage = { 1, 2, 3, 0 };
	ASSERT_EQ(perm.image(), expectedImage);
}

TEST(ExplicitPermutation, equality) {
	perm::ExplicitPermutation p1;
	perm::ExplicitPermutation p2;
	perm::ExplicitPermutation p3({ 1, 2, 0, 3 });
	perm::ExplicitPermutation p4({ 0, 1, 2, 3 });

	ASSERT_EQ(p1, p2);
	ASSERT_NE(p1, p3);
	ASSERT_EQ(p1, p4);
}

TEST(ExplicitPermutation, multiplication) {
	perm::ExplicitPermutation id;
	perm::ExplicitPermutation p1({ 2, 0, 1, 3 });
	perm::ExplicitPermutation p2({ 0, 1, 3, 2 });
	perm::ExplicitPermutation p3 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2, 3 }));
	perm::ExplicitPermutation p4 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }));

	perm::ExplicitPermutation r1({ 3, 0, 1, 2 });
	perm::ExplicitPermutation r2 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2, 3 }));

	ASSERT_NE(id, p1);
	ASSERT_NE(p1, p2);

	// Multiplication with id is no-op
	ASSERT_EQ(id * id, id);
	ASSERT_EQ(id * p1, p1);
	ASSERT_EQ(p1 * id, p1);
	ASSERT_EQ(id * p2, p2);
	ASSERT_EQ(p2 * id, p2);

	ASSERT_EQ(p1 * p2, r1);
	ASSERT_EQ(p3 * p4, r2);
}

TEST(ExplicitPermutation, invert) {
	perm::ExplicitPermutation inverse = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }));
	inverse.invert();
	ASSERT_EQ(inverse, perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }), -1));


	inverse = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2 }));
	inverse.invert(false);
	ASSERT_EQ(inverse, perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 2, 1 }), +1));


	inverse = perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 0, 1, 2 }, { 3, 4 } }));
	inverse.invert();
	ASSERT_EQ(inverse, perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 0, 2, 1 }, { 3, 4 } }), -1));


	perm::ExplicitPermutation p1 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 3, 4 }));
	perm::ExplicitPermutation p2 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 3 }));
	inverse                      = p1 * p2;
	inverse.invert(false);
	perm::ExplicitPermutation identity;
	ASSERT_EQ(p1 * p2 * inverse, identity);
}

TEST(ExplicitPermutation, permutationInterface) {
	perm::test::testPermutationInterface< perm::ExplicitPermutation >();
}
