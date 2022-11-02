// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_TESTS_PERMUTATIONTEST_HPP_
#define LIBPERM_TESTS_PERMUTATIONTEST_HPP_

#include <libperm/AbstractPermutation.hpp>
#include <libperm/Cycle.hpp>

#include <string>

#include <gtest/gtest.h>

namespace perm::test {

template< typename PermutationImpl > void testPermutationInterface() {
	PermutationImpl permObj1 = PermutationImpl::fromCycle(perm::Cycle(4, { 0, 1, 2, 3 }));
	PermutationImpl permObj2 = PermutationImpl::fromCycle(perm::Cycle(4, { { 0, 1 }, { 2, 3 } }));
	PermutationImpl permObj3 = PermutationImpl::fromCycle(perm::Cycle(4, { 0, 1, 2, 3 }));
	PermutationImpl result1  = PermutationImpl::fromCycle(perm::Cycle(4, { 1, 3 }));
	PermutationImpl result2  = PermutationImpl::fromCycle(perm::Cycle(4, { 0, 3, 2, 1 }));
	PermutationImpl identity = PermutationImpl::fromCycle(perm::Cycle(4));
	AbstractPermutation &p1  = permObj1;
	AbstractPermutation &p2  = permObj2;
	AbstractPermutation &p3  = permObj3;
	AbstractPermutation &r1  = result1;
	AbstractPermutation &r2  = result2;
	AbstractPermutation &id  = identity;

	ASSERT_EQ(p1.n(), static_cast< std::size_t >(4));

	std::vector< AbstractPermutation::value_type > expectedImage = { 1, 2, 3, 0 };
	for (AbstractPermutation::value_type i = 0; i < p1.n(); ++i) {
		ASSERT_EQ(p1[i], p1.image(i));
		ASSERT_EQ(p1[i], expectedImage[i]);
	}

	ASSERT_EQ(p1, p3);
	ASSERT_NE(p1, p2);

	p1 *= p2;

	ASSERT_EQ(p1, r1);

	ASSERT_FALSE(p1.isIdentity());
	ASSERT_TRUE(id.isIdentity());

	p3.invert();
	ASSERT_EQ(p3, r2);

	std::string str = p3.toString();
	ASSERT_FALSE(str.empty());
}

} // namespace perm::test

#endif // LIBPERM_TESTS_PERMUTATIONTEST_HPP_
