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

TEST(PermutationContainer, add) {
	perm::PermutationContainerImpl< perm::ExplicitPermutation > containerImpl1;
	perm::PermutationContainer &container1 = containerImpl1;

	perm::PermutationContainerImpl< perm::ExplicitPermutation > containerImpl2;
	perm::PermutationContainer &container2 = containerImpl2;

	perm::PermutationContainerImpl< perm::ExplicitPermutation > containerImpl3;
	perm::PermutationContainer &container3 = containerImpl3;

	perm::ExplicitPermutation permutation({ 0, 2, 1 });

	// Add via the generic add-method in the PermutationContainer interface
	container1.add(permutation);

	// Add via the specialized (type-aware) specialization in the PermutationContainerImpl class
	containerImpl2.add(permutation);

	ASSERT_EQ(container1.size(), 1);
	ASSERT_EQ(container1[0], permutation);
	ASSERT_EQ(container1, container2);

	// Add via the (type-aware) move-spevialization in the PermutationContainerImpl class
	containerImpl3.add(std::move(permutation));

	ASSERT_EQ(container2, container3);
}
