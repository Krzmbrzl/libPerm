// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/AbstractPermutation.hpp"
#include "libperm/Cycle.hpp"
#include "libperm/ExplicitPermutation.hpp"
#include "libperm/PrimitivePermutationGroup.hpp"
#include "libperm/Utils.hpp"

#include "StreamOperators.hpp"

#include <gtest/gtest.h>

#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <vector>


TEST(Utils, applyPermutation) {
	// A few manual, hard-coded examples
	const std::array< std::string, 4 > mainSequence = { "I", "am", "a", "sentence" };

	// Turn into question
	auto sequence = mainSequence;
	perm::applyPermutation(sequence, perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 })));
	decltype(sequence) expectedSequence = { "am", "I", "a", "sentence" };
	ASSERT_EQ(sequence, expectedSequence);

	// Gibberish
	sequence                       = mainSequence;
	perm::ExplicitPermutation perm = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 3, 1, 2 }));
	perm::applyPermutation(sequence, perm);
	expectedSequence = { "I", "sentence", "am", "a" };
	ASSERT_EQ(sequence, expectedSequence);

	// Undo gibberish
	perm.invert();
	perm::applyPermutation(sequence, perm);
	ASSERT_EQ(sequence, mainSequence);

	// Yoda
	sequence = mainSequence;
	perm::ExplicitPermutation yodaPerm({ 2, 3, 0, 1 });
	perm::applyPermutation(sequence, yodaPerm);
	expectedSequence = { "a", "sentence", "I", "am" };
	ASSERT_EQ(sequence, expectedSequence);

	// Yoda question
	perm::ExplicitPermutation yodaQuestionTransformer = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 3 }));
	perm::applyPermutation(sequence, yodaQuestionTransformer);
	expectedSequence = { "a", "sentence", "am", "I" };
	ASSERT_EQ(sequence, expectedSequence);

	// Un-yoda
	perm::ExplicitPermutation undoYoda = yodaPerm;
	undoYoda *= yodaQuestionTransformer;
	undoYoda.invert();
	perm::applyPermutation(sequence, undoYoda);
	ASSERT_EQ(sequence, mainSequence);
}

struct UtilsTest : testing::TestWithParam< std::vector< perm::AbstractPermutation::value_type > > {
	using ParamPack = std::vector< perm::AbstractPermutation::value_type >;
};

TEST_P(UtilsTest, applyPermutation) {
	const std::vector< perm::AbstractPermutation::value_type > mainSequence = { 0, 1, 2, 3, 4, 5, 6 };

	const perm::ExplicitPermutation permutation(GetParam());
	const perm::AbstractPermutation &abstractPerm = permutation;

	std::vector< perm::AbstractPermutation::value_type > expectedSequence(mainSequence.size());
	for (std::size_t i = 0; i < mainSequence.size(); ++i) {
		perm::AbstractPermutation::value_type image =
			permutation.image(static_cast< perm::AbstractPermutation::value_type >(i));
		expectedSequence[image] = mainSequence[i];
	}

	auto sequence = mainSequence;
	perm::applyPermutation(sequence, permutation);
	ASSERT_EQ(sequence, expectedSequence);

	sequence = mainSequence;
	perm::applyPermutation(sequence, abstractPerm);
	ASSERT_EQ(sequence, expectedSequence);
}

INSTANTIATE_TEST_SUITE_P(Utils, UtilsTest,
						 ::testing::Values(UtilsTest::ParamPack{ 3, 1, 2, 0 }, UtilsTest::ParamPack{ 2, 0, 1 },
										   UtilsTest::ParamPack{ 3, 2, 5, 0, 1, 4 }, UtilsTest::ParamPack{ 0 },
										   UtilsTest::ParamPack{ 0, 1, 2, 3, 6, 5, 4 },
										   UtilsTest::ParamPack{ 3, 1, 0, 2 }));


TEST(Utils, computeSortPermutation) {
	// A few hand-coded tests
	std::vector< int > sequence = { 1, 3, 2 };

	ASSERT_EQ(perm::computeSortPermutation(sequence), perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2 })));
	ASSERT_EQ(perm::computeSortPermutation(sequence, std::greater< int >{}), perm::ExplicitPermutation({ 2, 0, 1 }));

	sequence = { 42, 12, -13, 0, 21 };
	ASSERT_EQ(perm::computeSortPermutation(sequence), perm::ExplicitPermutation({ 4, 2, 0, 1, 3 }));

	sequence = {};
	ASSERT_EQ(perm::computeSortPermutation(sequence), perm::ExplicitPermutation());

	sequence = { 42 };
	ASSERT_EQ(perm::computeSortPermutation(sequence), perm::ExplicitPermutation());
}

struct SortPermTest : ::testing::TestWithParam< perm::ExplicitPermutation > {};

TEST_P(SortPermTest, computeSortPermutation) {
	std::vector< int > sequence = { -42, -15, -2, 0, 23, 35, 42 };

	const perm::ExplicitPermutation shufflePermutation = GetParam();
	// Sorting is the inverse of shuffling
	perm::ExplicitPermutation expectedSortPerm = shufflePermutation;
	expectedSortPerm.invert(false);

	perm::applyPermutation(sequence, shufflePermutation);

	ASSERT_EQ(perm::computeSortPermutation(sequence), expectedSortPerm) << "Sequence to sort: " << sequence;


	const perm::ExplicitPermutation secondShuffle = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 6, 4 }));
	perm::applyPermutation(sequence, secondShuffle);

	expectedSortPerm = shufflePermutation;
	expectedSortPerm *= secondShuffle;
	expectedSortPerm.invert(false);

	ASSERT_EQ(perm::computeSortPermutation(sequence), expectedSortPerm) << "Sequence to sort: " << sequence;
}

INSTANTIATE_TEST_SUITE_P(
	Utils, SortPermTest,
	::testing::Values(perm::ExplicitPermutation::fromCycle(perm::Cycle{}),
					  perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2 })),
					  perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2 })),
					  perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 1, 0 })),
					  perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 6, 1 })),
					  perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 0, 1, 2 }, { 4, 5 } })),
					  perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 0, 3 }, { 1, 5 }, { 2, 4 } }))));
