// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/Cycle.hpp>
#include <libperm/ExplicitPermutation.hpp>
#include <libperm/Permutation.hpp>
#include <libperm/PrimitivePermutationGroup.hpp>

#include <gtest/gtest.h>

#include <vector>

TEST(PrimitivePermutationGroup, construction) {
	perm::PrimitivePermutationGroup group;

	// The identity element is always contained in a group
	ASSERT_EQ(group.order(), 1);

	group = perm::PrimitivePermutationGroup({ perm::ExplicitPermutation(perm::Cycle({ 0, 1 })) });

	ASSERT_EQ(group.order(), 2);

	std::vector< perm::ExplicitPermutation > generators = { perm::Cycle({ 0, 1 }) };

	perm::PrimitivePermutationGroup group2(generators.begin(), generators.end());

	ASSERT_EQ(group, group2);

	perm::PrimitivePermutationGroup group3({ perm::ExplicitPermutation(perm::Cycle({ 0, 1 })) });

	ASSERT_EQ(group, group3);
}
