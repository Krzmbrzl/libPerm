// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/Permutation.hpp>
#include <libperm/PermutationContainer.hpp>
#include <libperm/details/ExplicitPermutation.hpp>

#include <gtest/gtest.h>

TEST(PermutationContainer, permutation_creation) {
	perm::PermutationContainerImpl< perm::ExplicitPermutation > containerImpl;
	perm::PermutationContainer &container = containerImpl;

	ASSERT_TRUE(container.empty());

	perm::ExplicitPermutation refIdentity(4);

	ASSERT_TRUE(refIdentity.isIdentity());

	perm::Permutation &identity = container.newIdentity(4);

	ASSERT_EQ(identity, refIdentity);

	ASSERT_FALSE(container.empty());
	ASSERT_EQ(container.size(), 1);

	container.newIdentity(4);
	ASSERT_EQ(container.size(), 2);
}

