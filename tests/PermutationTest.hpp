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
	PermutationImpl result2  = [permObj3]() {
        if constexpr (PermutationImpl::is_signed) {
            // For signed permutations, inversion leads to a sign change
            return PermutationImpl::fromCycle(perm::Cycle(4, { 0, 3, 2, 1 }), -1 * permObj3.sign());
        } else {
            return PermutationImpl::fromCycle(perm::Cycle(4, { 0, 3, 2, 1 }));
        }
	}();
	PermutationImpl identity4 = PermutationImpl::fromCycle(perm::Cycle(4));
	AbstractPermutation &p1   = permObj1;
	AbstractPermutation &p2   = permObj2;
	AbstractPermutation &p3   = permObj3;
	AbstractPermutation &r1   = result1;
	AbstractPermutation &r2   = result2;
	AbstractPermutation &id4  = identity4;

	ASSERT_EQ(p1.n(), static_cast< std::size_t >(4));
	ASSERT_EQ(p1.sign(), 1);

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
	ASSERT_TRUE(id4.isIdentity());

	p3.invert();
	ASSERT_EQ(p3, r2);

	std::string str = p3.toString();
	ASSERT_FALSE(str.empty());

	if constexpr (PermutationImpl::is_signed) {
		PermutationImpl signedPerm1   = PermutationImpl::fromCycle(perm::Cycle(4, { 0, 1, 2, 3 }), -1);
		PermutationImpl signedPerm2   = PermutationImpl::fromCycle(perm::Cycle(2, { 0, 1 }), -1);
		PermutationImpl identity2     = PermutationImpl(2);
		PermutationImpl signedResult1 = signedPerm1;
		PermutationImpl signedResult2 = PermutationImpl::fromCycle(perm::Cycle(4, { 1, 3 }), -1);

		AbstractPermutation &sp1 = signedPerm1;
		AbstractPermutation &sp2 = signedPerm2;
		AbstractPermutation &sr1 = signedResult1;
		AbstractPermutation &sr2 = signedResult2;
		AbstractPermutation &id2 = identity2;

		ASSERT_EQ(sp1.sign(), -1);
		ASSERT_EQ(sp2.sign(), -1);

		sp1 *= id4;

		ASSERT_EQ(sp1, sr1);

		sp1 *= p2;

		ASSERT_EQ(sp1, sr2);

		sp2 *= sp2;

		ASSERT_EQ(sp2, id2);
	}
}

} // namespace perm::test

#endif // LIBPERM_TESTS_PERMUTATIONTEST_HPP_
