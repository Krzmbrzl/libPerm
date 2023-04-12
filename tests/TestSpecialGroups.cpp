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
