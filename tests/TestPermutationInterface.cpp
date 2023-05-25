// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/AbstractPermutation.hpp>
#include <libperm/Cycle.hpp>
#include <libperm/ExplicitPermutation.hpp>

#include <gtest/gtest.h>

#include <vector>


using PermutationTypes = ::testing::Types< perm::ExplicitPermutation >;


template< typename T > struct PermCtor {
	static T construct(const perm::Cycle &cycle) { return T(cycle); }
};


template< typename Perm > struct PermutationInterface : ::testing::Test {};

// Trailing comma in order to avoid clang warning -  see
// https://github.com/google/googletest/issues/2271#issuecomment-665742471
TYPED_TEST_SUITE(PermutationInterface, PermutationTypes, );


TYPED_TEST(PermutationInterface, equality) {
	using Perm = TypeParam;

	std::vector< perm::Cycle > cycles = {
		perm::Cycle(),
		perm::Cycle({ 1, 2 }),
		perm::Cycle({ 0, 2, 1 }),
		perm::Cycle({ { 0, 3, 5 }, { 2, 4, 1 } }),
	};

	for (const perm::Cycle &outer : cycles) {
		for (const perm::Cycle &inner : cycles) {
			const Perm p1 = PermCtor< Perm >::construct(outer);
			const Perm p2 = PermCtor< Perm >::construct(inner);

			const perm::AbstractPermutation &perm1 = p1;
			const perm::AbstractPermutation &perm2 = p2;

			if (outer == inner) {
				ASSERT_EQ(perm1, perm2);
				ASSERT_EQ(perm2, perm1);

				ASSERT_TRUE(perm2.equals(perm1));
			} else {
				ASSERT_NE(perm1, perm2);
				ASSERT_NE(perm2, perm1);

				ASSERT_FALSE(perm2.equals(perm1));
			}
		}
	}
}


TYPED_TEST(PermutationInterface, image) {
	using Perm = TypeParam;

	const Perm p = PermCtor< Perm >::construct(perm::Cycle({ { 1, 3, 4 }, { 2, 5 } }));

	const perm::AbstractPermutation &perm = p;

	ASSERT_EQ(perm[2], static_cast< perm::AbstractPermutation::value_type >(5));
	ASSERT_EQ(perm.image(4), static_cast< perm::AbstractPermutation::value_type >(1));

	// We can also get images of points outside the actual permutation's action range
	ASSERT_EQ(perm[42], static_cast< perm::AbstractPermutation::value_type >(42));
	ASSERT_EQ(perm.image(42), static_cast< perm::AbstractPermutation::value_type >(42));
}


TYPED_TEST(PermutationInterface, multiplication) {
	using Perm = TypeParam;

	const Perm identity = PermCtor< Perm >::construct(perm::Cycle());
	const Perm basePerm = PermCtor< Perm >::construct(perm::Cycle({ 0, 2, 3 }));
	const Perm factor   = PermCtor< Perm >::construct(perm::Cycle({ 1, 2, 4 }));

	{
		// operator *=
		Perm p                          = basePerm;
		perm::AbstractPermutation &perm = p;

		perm *= factor;
		ASSERT_EQ(perm, perm::ExplicitPermutation(perm::Cycle({ 0, 4, 1, 2, 3 })));
	}
	{
		// postMultiply
		Perm p                          = basePerm;
		perm::AbstractPermutation &perm = p;

		perm.postMultiply(factor);
		ASSERT_EQ(perm, perm::ExplicitPermutation(perm::Cycle({ 0, 4, 1, 2, 3 })));
	}
	{
		// preMultiply
		Perm p                          = basePerm;
		perm::AbstractPermutation &perm = p;

		perm.preMultiply(factor);
		ASSERT_EQ(perm, perm::ExplicitPermutation(perm::Cycle({ 0, 2, 4, 1, 3 })));
	}
	{
		// Multiply with identity
		Perm p                          = basePerm;
		perm::AbstractPermutation &perm = p;

		perm *= identity;
		ASSERT_EQ(perm, basePerm);

		perm.preMultiply(identity);
		ASSERT_EQ(perm, basePerm);

		perm.postMultiply(identity);
		ASSERT_EQ(perm, basePerm);
	}
}

TYPED_TEST(PermutationInterface, maxElement) {
	using Perm = TypeParam;

	const Perm p1 = PermCtor< Perm >::construct(perm::Cycle());
	const Perm p2 = PermCtor< Perm >::construct(perm::Cycle({ 4, 5 }));

	const perm::AbstractPermutation &perm1 = p1;
	const perm::AbstractPermutation &perm2 = p2;

	ASSERT_EQ(perm1.maxElement(), static_cast< perm::AbstractPermutation::value_type >(0));
	ASSERT_EQ(perm2.maxElement(), static_cast< perm::AbstractPermutation::value_type >(5));
}


TYPED_TEST(PermutationInterface, isIdentity) {
	using Perm = TypeParam;

	const Perm p1 = PermCtor< Perm >::construct(perm::Cycle());
	const Perm p2 = PermCtor< Perm >::construct(perm::Cycle({ 4, 5 }));

	const perm::AbstractPermutation &perm1 = p1;
	const perm::AbstractPermutation &perm2 = p2;

	ASSERT_TRUE(perm1.isIdentity());
	ASSERT_FALSE(perm2.isIdentity());
}


TYPED_TEST(PermutationInterface, invert) {
	using Perm = TypeParam;

	Perm p                     = PermCtor< Perm >::construct(perm::Cycle({ 3, 2, 5, 7 }));
	const Perm expectedInverse = PermCtor< Perm >::construct(perm::Cycle({ 7, 5, 2, 3 }));

	perm::AbstractPermutation &perm = p;

	p.invert();

	ASSERT_EQ(perm, expectedInverse);
}


TYPED_TEST(PermutationInterface, sign) {
	using Perm = TypeParam;

	Perm p = PermCtor< Perm >::construct(perm::Cycle({ 1, 2 }));

	perm::AbstractPermutation &perm = p;

	ASSERT_EQ(perm.sign(), 1);

	perm.setSign(-1);

	if constexpr (Perm::is_signed) {
		ASSERT_EQ(perm.sign(), -1);
	} else {
		ASSERT_EQ(perm.sign(), 1);
	}
}


TYPED_TEST(PermutationInterface, toCycle) {
	using Perm = TypeParam;

	const perm::Cycle cycle({ { 1, 2, 4, 5 }, { 3, 7, 6 }, { 9, 8 } });
	const Perm p                          = PermCtor< Perm >::construct(cycle);
	const perm::AbstractPermutation &perm = p;

	ASSERT_EQ(perm.toCycle(), cycle);
}

TYPED_TEST(PermutationInterface, shift) {
	using Perm = TypeParam;

	Perm actual = PermCtor< Perm >::construct(perm::Cycle({ 0, 1 }));
	actual.shift(3);
	Perm expected = PermCtor< Perm >::construct(perm::Cycle({ 3, 4 }));
	ASSERT_EQ(actual, expected);

	actual = PermCtor< Perm >::construct(perm::Cycle({ { 0, 1 }, { 7, 3 } }));
	actual.shift(15);
	expected = PermCtor< Perm >::construct(perm::Cycle({ { 15, 16 }, { 22, 18 } }));
	ASSERT_EQ(actual, expected);

	actual.shift(-7);
	expected = PermCtor< Perm >::construct(perm::Cycle({ { 8, 9 }, { 15, 11 } }));
	ASSERT_EQ(actual, expected);

	actual = PermCtor< Perm >::construct(perm::Cycle());
	ASSERT_TRUE(actual.isIdentity());
	actual.shift(5);
	ASSERT_TRUE(actual.isIdentity());
}
