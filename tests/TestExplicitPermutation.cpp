// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/ExplicitPermutation.hpp>

#include "PermutationTest.hpp"

#include <gtest/gtest.h>

#include <functional>

TEST(ExplicitPermutation, construction) {
	perm::ExplicitPermutation perm;

	ASSERT_EQ(perm.maxElement(), static_cast< perm::AbstractPermutation::value_type >(0));
	ASSERT_TRUE(perm.isIdentity());
	ASSERT_EQ(perm.sign(), 1);

	perm = perm::ExplicitPermutation(std::vector< perm::ExplicitPermutation::value_type >{});
	ASSERT_TRUE(perm.isIdentity());
	ASSERT_EQ(perm.sign(), 1);

	perm = perm::ExplicitPermutation(-1);
	ASSERT_TRUE(!perm.isIdentity());
	ASSERT_EQ(perm.sign(), -1);

	perm = perm::ExplicitPermutation({ 1, 2, 0 });

	ASSERT_EQ(perm.maxElement(), static_cast< perm::AbstractPermutation::value_type >(2));
	std::vector< perm::AbstractPermutation::value_type > expectedImage = { 1, 2, 0 };
	ASSERT_EQ(perm.image(), expectedImage);

	perm          = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2, 3 }));
	expectedImage = { 1, 2, 3, 0 };
	ASSERT_EQ(perm.image(), expectedImage);
}

TEST(ExplicitPermutation, equality) {
	perm::ExplicitPermutation p1;
	perm::ExplicitPermutation p2;
	perm::ExplicitPermutation p3({ 1, 2, 0, 3 });
	perm::ExplicitPermutation p4({ 0, 1, 2, 3 });

	ASSERT_EQ(p1, p2);
	ASSERT_NE(p1, p3);
	ASSERT_EQ(p1, p4);
}

TEST(ExplicitPermutation, preMultiply) {
	perm::ExplicitPermutation id;
	perm::ExplicitPermutation p1 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2, 3 }));
	perm::ExplicitPermutation p2 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 3, 4, 1 }));

	auto copy = p1;
	copy.preMultiply(id);
	ASSERT_EQ(copy, p1);

	p1.preMultiply(p2);
	ASSERT_EQ(p1, perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 3, 4 })));

	// Multiplication with self
	p2.preMultiply(p2);
	ASSERT_EQ(p2, perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 4, 3 })));
}

TEST(ExplicitPermutation, postMultiply) {
	perm::ExplicitPermutation id;
	perm::ExplicitPermutation p1 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2, 3 }));
	perm::ExplicitPermutation p2 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 3, 4, 1 }));

	auto copy = p1;
	copy.postMultiply(id);
	ASSERT_EQ(copy, p1);

	p1.postMultiply(p2);
	ASSERT_EQ(p1, perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2, 4 })));

	// Multiplication with self
	p2.postMultiply(p2);
	ASSERT_EQ(p2, perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 4, 3 })));
}

TEST(ExplicitPermutation, binary_multiply_operator) {
	const perm::ExplicitPermutation id;
	const perm::ExplicitPermutation p1({ 2, 0, 1, 3 });
	const perm::ExplicitPermutation p2({ 0, 1, 3, 2 });
	const perm::ExplicitPermutation p3 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2, 3 }));
	const perm::ExplicitPermutation p4 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }));

	const perm::AbstractPermutation &p1Abstract = p1;
	const perm::AbstractPermutation &p2Abstract = p2;

	const perm::ExplicitPermutation r1({ 3, 0, 1, 2 });
	const perm::ExplicitPermutation r2 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2, 3 }));

	ASSERT_NE(id, p1);
	ASSERT_NE(p1, p2);

	// Multiplication with id is no-op
	ASSERT_EQ(id * id, id);
	ASSERT_EQ(id * p1, p1);
	ASSERT_EQ(p1 * id, p1);
	ASSERT_EQ(id * p2, p2);
	ASSERT_EQ(p2 * id, p2);

	ASSERT_EQ(p1 * p2, r1);
	ASSERT_EQ(p3 * p4, r2);

	// Multiplication with permutation via the AbstractPermutation interface
	ASSERT_EQ(p1 * p2Abstract, r1);
	ASSERT_EQ(p1Abstract * p2, r1);
}

struct MultiplicationTest : ::testing::TestWithParam< std::tuple< perm::Cycle, perm::Cycle > > {};

TEST_P(MultiplicationTest, consistency) {
	const perm::ExplicitPermutation p1 = perm::ExplicitPermutation::fromCycle(std::get< 0 >(GetParam()));
	const perm::ExplicitPermutation p2 = perm::ExplicitPermutation::fromCycle(std::get< 1 >(GetParam()));

	perm::ExplicitPermutation preMultiplied = p1;
	preMultiplied.preMultiply(p2);

	perm::ExplicitPermutation postMultiplied = p1;
	postMultiplied.postMultiply(p2);

	ASSERT_EQ(preMultiplied, p2 * p1);
	ASSERT_EQ(postMultiplied, p1 * p2);


	perm::ExplicitPermutation preMultipliedSelf = p1;
	preMultipliedSelf.preMultiply(p1);

	perm::ExplicitPermutation postMultipliedSelf = p1;
	postMultipliedSelf.postMultiply(p1);

	ASSERT_EQ(preMultipliedSelf, postMultipliedSelf);
	ASSERT_EQ(preMultipliedSelf, p1 * p1);


	perm::ExplicitPermutation p1Inv = p1;
	p1Inv.invert();

	perm::ExplicitPermutation p2Inv = p2;
	p2Inv.invert();


	preMultiplied.preMultiply(p2Inv);
	EXPECT_EQ(preMultiplied, p1);

	postMultiplied.preMultiply(p1Inv);
	EXPECT_EQ(postMultiplied, p2);
}

INSTANTIATE_TEST_SUITE_P(
	ExplicitPermutation, MultiplicationTest,
	::testing::Combine(::testing::Values(perm::Cycle({ 0, 1 }), perm::Cycle({ { 2, 4 }, { 0, 3 } }),
										 perm::Cycle({ 0, 2, 3, 5 }), perm::Cycle({ { 0, 1 }, { 2, 3 }, { 4, 5, 6 } })),
					   ::testing::Values(perm::Cycle(), perm::Cycle({ { 0, 5, 3 }, { 1, 2, 4 } }),
										 perm::Cycle({ 0, 1, 2, 3, 4, 5, 6 }),
										 perm::Cycle({ { 2, 1 }, { 0, 5, 3, 4 } }))));


struct InvertTest : ::testing::TestWithParam< std::tuple< perm::Cycle, perm::Cycle > > {
	using ParamPack = std::tuple< perm::Cycle, perm::Cycle >;
};

TEST_P(InvertTest, invert) {
	const perm::ExplicitPermutation identity;
	perm::ExplicitPermutation original        = perm::ExplicitPermutation::fromCycle(std::get< 0 >(GetParam()));
	perm::ExplicitPermutation perm            = original;
	perm::ExplicitPermutation expectedInverse = perm::ExplicitPermutation::fromCycle(std::get< 1 >(GetParam()));

	perm.invert();
	ASSERT_EQ(perm, expectedInverse);

	// perm times its inverse should be identity
	ASSERT_EQ(original * perm, identity);

	// Inverting twice should give back the original perm
	perm.invert();
	ASSERT_EQ(perm, original);

	// The sign should be retained during inversion
	original.setSign(-1);
	perm.setSign(-1);
	expectedInverse.setSign(-1);
	perm.invert();
	ASSERT_EQ(perm, expectedInverse);

	// Inverting twice should give back the original perm
	perm.invert();
	ASSERT_EQ(perm, original);

	perm.invert();

	// perm times its inverse should be identity
	ASSERT_EQ(original * perm, identity);
}

INSTANTIATE_TEST_SUITE_P(ExplicitPermutation, InvertTest,
						 ::testing::Values(InvertTest::ParamPack(perm::Cycle({ 0, 1 }), perm::Cycle({ 0, 1 })),
										   InvertTest::ParamPack(perm::Cycle({ 0, 1, 2 }), perm::Cycle({ 0, 2, 1 })),
										   InvertTest::ParamPack(perm::Cycle({ { 0, 1, 2 }, { 3, 4 } }),
																 perm::Cycle({ { 0, 2, 1 }, { 3, 4 } })),
										   InvertTest::ParamPack(perm::Cycle({ { 1, 3, 4 }, { 7, 8, 2, 5 } }),
																 perm::Cycle({ { 1, 4, 3 }, { 2, 8, 7, 5 } }))));

TEST(ExplicitPermutation, permutationInterface) {
	perm::test::testPermutationInterface< perm::ExplicitPermutation >();
}
