// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/ExplicitPermutation.hpp>
#include <libperm/PrimitivePermutationGroup.hpp>
#include <libperm/SpecialGroups.hpp>

#include <gtest/gtest.h>

#include <cstdint>


std::size_t factorial(unsigned int num) {
	std::size_t fac = 1;

	for (std::size_t i = 1; i <= num; ++i) {
		fac *= i;
	}

	return fac;
}


template< typename G, typename P > struct TypeHolder {
	using Group = G;
	using Perm  = P;
};

template< typename T > struct SpecialGroupsTest : ::testing::Test {};


using Types = ::testing::Types< TypeHolder< perm::PrimitivePermutationGroup, perm::ExplicitPermutation > >;
TYPED_TEST_SUITE(SpecialGroupsTest, Types, );


TYPED_TEST(SpecialGroupsTest, Sym) {
	using Group = typename TypeParam::Group;
	using Perm  = typename TypeParam::Perm;

	for (unsigned int n : { 0, 1, 2, 3, 4, 5, 6 }) {
		Group group = perm::Sym< Group, Perm >(n);

		ASSERT_EQ(group.order(), factorial(n)) << "Failed for n = " << static_cast< unsigned int >(n);
	}
}

struct Signs {
	std::size_t positive;
	std::size_t negative;
};

template< typename Group > Signs countSigns(Group g) {
	std::vector< perm::Permutation > elements;
	g.getElementsTo(elements);

	Signs signs{};

	for (const perm::Permutation &currentElement : elements) {
		if (currentElement->sign() == -1) {
			signs.negative++;
		} else {
			signs.positive++;
		}
	}

	return signs;
}

TYPED_TEST(SpecialGroupsTest, antisymmetricRanges) {
	using Group = typename TypeParam::Group;
	using Perm  = typename TypeParam::Perm;

	// Note: Each antisymmetric range turns into the symmetric group over that range when disregarding the sign
	// Furthermore, the amount of permutations with positive sign must always be equal to the amount of negative signs,
	// unless the group is "empty" in which case there is one positive (identity) and zero negative permutations.

	// If all ranges consist of only a single element, then the resulting group only contains the identity element
	Group expectedGroup;
	Signs signs = countSigns(expectedGroup);
	ASSERT_EQ(expectedGroup.order(), factorial(1));
	ASSERT_EQ(signs.positive, 1);
	ASSERT_EQ(signs.negative, 0);

	Group actualGroup = perm::antisymmetricRanges< Group, Perm >({ { 0, 0 }, { 4, 4 } });
	ASSERT_EQ(actualGroup, expectedGroup);


	// Antisymmetry of a range of two elements
	expectedGroup.setGenerators({ Perm(perm::Cycle({ 0, 1 }), -1) });
	signs = countSigns(expectedGroup);
	ASSERT_EQ(expectedGroup.order(), factorial(2));
	ASSERT_EQ(signs.positive, signs.negative);

	actualGroup = perm::antisymmetricRanges< Group, Perm >({ { 0, 1 } });
	ASSERT_EQ(actualGroup, expectedGroup);


	// Antisymmetry of a range of three elements
	expectedGroup.setGenerators({ Perm(perm::Cycle({ 0, 1 }), -1), Perm(perm::Cycle({ 0, 2 }), -1) });
	signs = countSigns(expectedGroup);
	ASSERT_EQ(expectedGroup.order(), factorial(3));
	ASSERT_EQ(signs.positive, signs.negative);

	actualGroup = perm::antisymmetricRanges< Group, Perm >({ { 0, 2 } });
	ASSERT_EQ(actualGroup, expectedGroup);


	// Antisymmetry of two disjoint ranges of three and two elements respectively
	expectedGroup.setGenerators(
		{ Perm(perm::Cycle({ 1, 2 }), -1), Perm(perm::Cycle({ 1, 3 }), -1), Perm(perm::Cycle({ 5, 6 }), -1) });
	signs = countSigns(expectedGroup);
	ASSERT_EQ(expectedGroup.order(), factorial(3) * factorial(2));
	ASSERT_EQ(signs.positive, signs.negative);

	actualGroup = perm::antisymmetricRanges< Group, Perm >({ { 1, 3 }, { 5, 6 } });
	ASSERT_EQ(actualGroup, expectedGroup);
}
