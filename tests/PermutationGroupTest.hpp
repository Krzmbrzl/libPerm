// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_TESTS_PERMUTATIONGROUPTEST_HPP_
#define LIBPERM_TESTS_PERMUTATIONGROUPTEST_HPP_

#include <libperm/AbstractPermutationGroup.hpp>
#include <libperm/Cycle.hpp>
#include <libperm/ExplicitPermutation.hpp>

#include <algorithm>

#include <gtest/gtest.h>

namespace perm::test {

template< typename PermutationGroupImpl > void testPermutationGroupInterface() {
	PermutationGroupImpl groupImpl;
	AbstractPermutationGroup &group = groupImpl;

	// Group only contains identity
	ASSERT_EQ(group.order(), 1);
	std::vector< AbstractPermutation::value_type > orbit = group.orbit(3);
	ASSERT_EQ(orbit.size(), 1);
	ASSERT_EQ(orbit[0], 3);


	group.addGenerator(perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }))));
	ASSERT_EQ(group.order(), 2);
	orbit = group.orbit(3);
	ASSERT_EQ(orbit.size(), 1);
	ASSERT_EQ(orbit[0], 3);



	group.addGenerator(perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 3, 1 }))));
	ASSERT_EQ(group.order(), 6);
	orbit                                                        = group.orbit(3);
	std::vector< AbstractPermutation::value_type > expectedOrbit = { 3, 1, 0 };
	ASSERT_EQ(orbit.size(), expectedOrbit.size());
	ASSERT_TRUE(std::is_permutation(orbit.begin(), orbit.end(), expectedOrbit.begin()));



	group.setGenerators({ perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }))),
						  perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 3, 4, 5 }))) });
	ASSERT_EQ(group.order(), 8);
	orbit         = group.orbit(3);
	expectedOrbit = { 3, 4, 5, 2 };
	ASSERT_EQ(orbit.size(), expectedOrbit.size());
	ASSERT_TRUE(std::is_permutation(orbit.begin(), orbit.end(), expectedOrbit.begin()));
}

} // namespace perm::test

#endif // LIBPERM_TESTS_PERMUTATIONGROUPTEST_HPP_
