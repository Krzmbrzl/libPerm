// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/Cycle.hpp>
#include <libperm/ExplicitPermutation.hpp>
#include <libperm/Permutation.hpp>
#include <libperm/PrimitivePermutationGroup.hpp>

#include "PermutationGroupTest.hpp"

#include <gtest/gtest.h>

TEST(PrimitivePermutationGroup, construction) {
	perm::PrimitivePermutationGroup group;

	// The identity element is always contained in a group
	ASSERT_EQ(group.order(), 1);

	group = perm::PrimitivePermutationGroup(
		{ perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }))) });

	ASSERT_EQ(group.order(), 2);
}

TEST(PrimitivePermutationGroup, permutationGroupInterface) {
	perm::test::testPermutationGroupInterface< perm::PrimitivePermutationGroup >();
}

TEST(PrimitivePermutationGroup, cosetRepresentative) {
	perm::test::testCosetRepresentative< perm::PrimitivePermutationGroup >();
}
