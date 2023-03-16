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
	perm::Cycle cycle1 = perm::Cycle({ 0, 1, 2, 3 });
	perm::Cycle cycle2 = perm::Cycle({ { 0, 1 }, { 2, 3 } });
	perm::Cycle cycle3 = perm::Cycle({ 0, 1 });
	perm::Cycle cycle4 = perm::Cycle({ 1, 3 });

	PermutationImpl permObj1 = PermutationImpl::fromCycle(cycle1);
	PermutationImpl permObj2 = PermutationImpl::fromCycle(cycle2);
	PermutationImpl permObj3 = permObj1;
	PermutationImpl permObj4 = permObj1;
	PermutationImpl permObj5 = PermutationImpl::fromCycle(cycle3);
	PermutationImpl result1  = PermutationImpl::fromCycle(cycle4);
	PermutationImpl result2  = PermutationImpl::fromCycle(perm::Cycle({ 0, 3, 2, 1 }));
	PermutationImpl result3  = PermutationImpl::fromCycle(perm::Cycle({ 1, 2, 3 }));
	PermutationImpl identity;

	AbstractPermutation &p1 = permObj1;
	AbstractPermutation &p2 = permObj2;
	AbstractPermutation &p3 = permObj3;
	AbstractPermutation &p4 = permObj4;
	AbstractPermutation &p5 = permObj5;
	AbstractPermutation &r1 = result1;
	AbstractPermutation &r2 = result2;
	AbstractPermutation &r3 = result3;
	AbstractPermutation &id = identity;

	ASSERT_EQ(p1.toCycle(), cycle1);
	ASSERT_EQ(p2.toCycle(), cycle2);
	ASSERT_EQ(p3.toCycle(), cycle1);
	ASSERT_EQ(p4.toCycle(), cycle1);
	ASSERT_EQ(p5.toCycle(), cycle3);
	ASSERT_EQ(r1.toCycle(), cycle4);

	ASSERT_EQ(p1.maxElement(), static_cast< AbstractPermutation::value_type >(3));
	ASSERT_EQ(p1.sign(), 1);

	std::vector< AbstractPermutation::value_type > expectedImage = { 1, 2, 3, 0 };
	for (AbstractPermutation::value_type i = 0; i <= p1.maxElement(); ++i) {
		ASSERT_EQ(p1[i], p1.image(i));
		ASSERT_EQ(p1[i], expectedImage[i]);
	}

	ASSERT_EQ(p1, p3);
	ASSERT_NE(p1, p2);

	p1 *= p2;

	ASSERT_EQ(p1, r1);

	ASSERT_FALSE(p1.isIdentity());
	ASSERT_TRUE(id.isIdentity());

	p1 *= id;

	ASSERT_EQ(p1, r1);

	p3.invert();
	ASSERT_EQ(p3, r2);

	std::string str = p3.toString();
	ASSERT_FALSE(str.empty());

	// We also want to support mixing permutations with different maxElement
	ASSERT_NE(p4.maxElement(), p5.maxElement());
	p4 *= p5;
	ASSERT_EQ(p4, r3);

	if constexpr (PermutationImpl::is_signed) {
		PermutationImpl signedPerm1   = PermutationImpl::fromCycle(perm::Cycle({ 0, 1, 2, 3 }), -1);
		PermutationImpl signedPerm2   = PermutationImpl::fromCycle(perm::Cycle({ 0, 1 }), -1);
		PermutationImpl signedResult1 = signedPerm1;
		PermutationImpl signedResult2 = PermutationImpl::fromCycle(perm::Cycle({ 1, 3 }), -1);

		AbstractPermutation &sp1 = signedPerm1;
		AbstractPermutation &sp2 = signedPerm2;
		AbstractPermutation &sr1 = signedResult1;
		AbstractPermutation &sr2 = signedResult2;

		ASSERT_EQ(sp1.sign(), -1);
		ASSERT_EQ(sp2.sign(), -1);

		sp1 *= id;

		ASSERT_EQ(sp1, sr1);

		sp1 *= p2;

		ASSERT_EQ(sp1, sr2);

		sp2 *= sp2;

		ASSERT_EQ(sp2, id);
	}
}

} // namespace perm::test

#endif // LIBPERM_TESTS_PERMUTATIONTEST_HPP_
