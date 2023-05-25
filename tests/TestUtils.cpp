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

#include <algorithm>
#include <array>
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

perm::PrimitivePermutationGroup asGroup(const std::vector< perm::Cycle > &generators) {
	perm::PrimitivePermutationGroup group;

	for (const perm::Cycle &currentCycle : generators) {
		group.addGenerator(perm::ExplicitPermutation(currentCycle));
	}

	return group;
}


TEST(Utils, applyPermutation) {
	// A few manual, hard-coded examples
	const std::array< std::string, 4 > mainSequence = { "I", "am", "a", "sentence" };

	// Turn into question
	auto sequence = mainSequence;
	perm::applyPermutation(sequence, perm::ExplicitPermutation(perm::Cycle({ 0, 1 })));
	decltype(sequence) expectedSequence = { "am", "I", "a", "sentence" };
	ASSERT_EQ(sequence, expectedSequence);

	// Gibberish
	sequence                       = mainSequence;
	perm::ExplicitPermutation perm = perm::ExplicitPermutation(perm::Cycle({ 3, 1, 2 }));
	perm::applyPermutation(sequence, perm);
	expectedSequence = { "I", "a", "sentence", "am" };
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
	perm::ExplicitPermutation yodaQuestionTransformer = perm::ExplicitPermutation(perm::Cycle({ 2, 3 }));
	perm::applyPermutation(sequence, yodaQuestionTransformer);
	expectedSequence = { "a", "sentence", "am", "I" };
	ASSERT_EQ(sequence, expectedSequence);

	// one-step yoda-question
	// Remember that the order of composition is reversed when applying to sequences. That is: the multiplication
	// has to be read from right-to-left rather than from left-to-right.
	perm::ExplicitPermutation yodaQuestion = yodaPerm;
	yodaQuestion.preMultiply(yodaQuestionTransformer);
	sequence = mainSequence;
	perm::applyPermutation(sequence, yodaQuestion);
	ASSERT_EQ(sequence, expectedSequence);

	// Un-yoda
	perm::ExplicitPermutation undoYoda = yodaQuestion;
	undoYoda.invert();
	perm::applyPermutation(sequence, undoYoda);
	ASSERT_EQ(sequence, mainSequence);
}

TEST(Utils, applyPermutation_consistency) {
	// Ensure applyPermutation is consistent with the permutation's image
	std::vector< perm::AbstractPermutation::value_type > sequence = { 0, 1, 2, 3, 4, 5 };
	const perm::ExplicitPermutation perm = perm::ExplicitPermutation(perm::Cycle({ 0, 1, 5, 3 }));

	perm::applyPermutation(sequence, perm);

	ASSERT_EQ(sequence, perm.image());
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
		expectedSequence[i] = mainSequence[image];
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

	ASSERT_EQ(perm::computeSortPermutation(sequence), perm::ExplicitPermutation(perm::Cycle({ 1, 2 })));
	ASSERT_EQ(perm::computeSortPermutation(sequence, std::greater< int >{}), perm::ExplicitPermutation({ 1, 2, 0 }));

	sequence = { 42, 12, -13, 0, 21 };
	ASSERT_EQ(perm::computeSortPermutation(sequence), perm::ExplicitPermutation({ 2, 3, 1, 4, 0 }));

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
	expectedSortPerm.invert();

	perm::applyPermutation(sequence, shufflePermutation);

	perm::ExplicitPermutation sortPerm = perm::computeSortPermutation(sequence);
	ASSERT_EQ(sortPerm, expectedSortPerm) << "Sequence to sort: " << sequence;
	auto sortedSeq = sequence;
	perm::applyPermutation(sortedSeq, sortPerm);
	ASSERT_TRUE(std::is_sorted(sortedSeq.begin(), sortedSeq.end()))
		<< "Computed sort permutation " << sortPerm << " did not sort " << sequence;


	const perm::ExplicitPermutation secondShuffle = perm::ExplicitPermutation(perm::Cycle({ 1, 6, 4 }));
	perm::applyPermutation(sequence, secondShuffle);

	// Remember that composition order is reversed when applying to sequences
	expectedSortPerm = shufflePermutation;
	expectedSortPerm.preMultiply(secondShuffle);
	expectedSortPerm.invert();

	sortPerm = perm::computeSortPermutation(sequence);
	ASSERT_EQ(sortPerm, expectedSortPerm) << "Sequence to sort: " << sequence;
	sortedSeq = sequence;
	perm::applyPermutation(sortedSeq, sortPerm);
	ASSERT_TRUE(std::is_sorted(sortedSeq.begin(), sortedSeq.end()))
		<< "Computed sort permutation " << sortPerm << " did not sort " << sequence;
}

INSTANTIATE_TEST_SUITE_P(Utils, SortPermTest,
						 ::testing::Values(perm::ExplicitPermutation(perm::Cycle{}),
										   perm::ExplicitPermutation(perm::Cycle({ 1, 2 })),
										   perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2 })),
										   perm::ExplicitPermutation(perm::Cycle({ 2, 1, 0 })),
										   perm::ExplicitPermutation(perm::Cycle({ 2, 6, 1 })),
										   perm::ExplicitPermutation(perm::Cycle({ { 0, 1, 2 }, { 4, 5 } })),
										   perm::ExplicitPermutation(perm::Cycle({ { 0, 3 }, { 1, 5 }, { 2, 4 } }))));


struct CanonicalizeTest : ::testing::TestWithParam< std::vector< perm::Cycle > > {
	using ParamPack = std::vector< perm::Cycle >;
};

TEST_P(CanonicalizeTest, canonicalize) {
	perm::PrimitivePermutationGroup group;

	for (const perm::Cycle &currentCycle : GetParam()) {
		group.addGenerator(perm::ExplicitPermutation(currentCycle));
	}

	std::vector< perm::Permutation > elements;
	group.getElementsTo(elements);

	const perm::ExplicitPermutation shufflePermutation =
		perm::ExplicitPermutation(perm::Cycle({ { 1, 2 }, { 3, 5, 0 } }));

	std::vector< perm::AbstractPermutation::value_type > baseSequence = { 0, 1, 2, 3, 4, 5, 6, 7 };
	perm::applyPermutation(baseSequence, shufflePermutation);

	decltype(baseSequence) expectedSequence = baseSequence;
	perm::canonicalize(expectedSequence, group);

	for (perm::Permutation &currentPerm : elements) {
		decltype(baseSequence) sequence = baseSequence;
		perm::applyPermutation(sequence, currentPerm);

		perm::canonicalize(sequence, group);

		ASSERT_EQ(sequence, expectedSequence) << "Current perm: " << currentPerm;
	}
}

INSTANTIATE_TEST_SUITE_P(
	Utils, CanonicalizeTest,
	::testing::Values(CanonicalizeTest::ParamPack{}, CanonicalizeTest::ParamPack{ perm::Cycle({ 0, 1 }) },
					  CanonicalizeTest::ParamPack{ perm::Cycle({ 0, 1, 2 }) },
					  CanonicalizeTest::ParamPack{ perm::Cycle({ { 0, 2, 3 }, { 4, 5 } }) },
					  CanonicalizeTest::ParamPack{ perm::Cycle({ { 0, 2 }, { 4, 1 }, { 3, 7 } }) }));



struct ConcatenateTest
	: ::testing::TestWithParam<
		  std::tuple< perm::PrimitivePermutationGroup, std::vector< std::size_t >, perm::PrimitivePermutationGroup,
					  std::vector< std::size_t >, perm::PrimitivePermutationGroup > > {
	using ParamPack =
		std::tuple< perm::PrimitivePermutationGroup, std::vector< std::size_t >, perm::PrimitivePermutationGroup,
					std::vector< std::size_t >, perm::PrimitivePermutationGroup >;
};

TEST_P(ConcatenateTest, concatenate) {
	const perm::PrimitivePermutationGroup lhsGroup      = std::get< 0 >(GetParam());
	const std::vector< std::size_t > lhsExcludes        = std::get< 1 >(GetParam());
	const perm::PrimitivePermutationGroup rhsGroup      = std::get< 2 >(GetParam());
	const std::vector< std::size_t > rhsExcludes        = std::get< 3 >(GetParam());
	const perm::PrimitivePermutationGroup expectedGroup = std::get< 4 >(GetParam());

	std::vector< perm::Permutation > elements;
	lhsGroup.getElementsTo(elements);
	std::unordered_set< std::size_t > effectiveExcludes;
	for (const perm::Permutation &current : elements) {
		for (std::size_t currentExclude : lhsExcludes) {
			if (current->maxElement() >= currentExclude) {
				effectiveExcludes.insert(currentExclude);
			}
		}
	}

	std::size_t remainingLeftSize = 8 - effectiveExcludes.size();

	perm::PrimitivePermutationGroup actualGroup = perm::concatenate< perm::PrimitivePermutationGroup >(
		lhsGroup, remainingLeftSize, rhsGroup, lhsExcludes, rhsExcludes);

	ASSERT_EQ(actualGroup, expectedGroup);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
	Utils, ConcatenateTest,
	::testing::Values(
		ConcatenateTest::ParamPack{perm::PrimitivePermutationGroup{}, {4}, perm::PrimitivePermutationGroup{}, {42}, perm::PrimitivePermutationGroup{}},
		ConcatenateTest::ParamPack{asGroup({perm::Cycle({0,1})}), {1}, asGroup({perm::Cycle({4,2})}), {4}, perm::PrimitivePermutationGroup{}},
		ConcatenateTest::ParamPack{
			asGroup({perm::Cycle({0,1}),  perm::Cycle({2,3})}),
			{1},
			asGroup({perm::Cycle({4,2}), perm::Cycle({{0,1}, {3,5}})}),
			{4},
			asGroup({perm::Cycle({1,2}), perm::Cycle({{7,8}, {10,11}})})
		},
		ConcatenateTest::ParamPack{
			asGroup({perm::Cycle({0,1}),  perm::Cycle({2,3})}),
			{5},
			asGroup({perm::Cycle({4,2}), perm::Cycle({{0,1}, {3,5}})}),
			{},
			asGroup({perm::Cycle({0,1}),  perm::Cycle({2,3}), perm::Cycle({12,10}), perm::Cycle({{8,9}, {11,13}})})
		},
		ConcatenateTest::ParamPack{
			asGroup({perm::Cycle({0,4,6})}),
			{2,5,10},
			asGroup({perm::Cycle({4,2}), perm::Cycle({1,5,9})}),
			{3,4},
			asGroup({perm::Cycle({0,3,4}), perm::Cycle({7,9,13})})
		},
		ConcatenateTest::ParamPack{
			asGroup({perm::Cycle({1,4,6}), perm::Cycle({4,6})}),
			{0,5},
			asGroup({perm::Cycle({2,3}), perm::Cycle({3,4})}),
			{0,1,5},
			asGroup({perm::Cycle({0,3,4}), perm::Cycle({3,4}), perm::Cycle({6,7}), perm::Cycle({7,8})})
		}
	));
// clang-format on
