// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/AbstractPermutationGroup.hpp>
#include <libperm/Cycle.hpp>
#include <libperm/ExplicitPermutation.hpp>
#include <libperm/Permutation.hpp>
#include <libperm/PrimitivePermutationGroup.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>


using PermutationGroupTypes = ::testing::Types< perm::PrimitivePermutationGroup >;


template< typename Group > Group fromGenerators(const std::vector< perm::Cycle > &cycles) {
	Group g;

	for (const perm::Cycle &current : cycles) {
		g.addGenerator(perm::Permutation(perm::ExplicitPermutation(current)));
	}

	return g;
}

template< typename Group > struct PermutationGroupInterface : ::testing::Test {};

// Trailing comma in order to avoid clang warning -  see
// https://github.com/google/googletest/issues/2271#issuecomment-665742471
TYPED_TEST_SUITE(PermutationGroupInterface, PermutationGroupTypes, );


TYPED_TEST(PermutationGroupInterface, getElementsTo) {
	using Group = TypeParam;

	const Group g                               = fromGenerators< Group >({ perm::Cycle({ 0, 1, 2 }) });
	const perm::AbstractPermutationGroup &group = g;

	const std::vector< perm::Permutation > expectedElements = {
		perm::Permutation(perm::ExplicitPermutation()),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2 }))),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 2, 1 })))
	};

	std::vector< perm::Permutation > elements;
	group.getElementsTo(elements);

	EXPECT_THAT(elements, ::testing::UnorderedElementsAreArray(expectedElements));
}


TYPED_TEST(PermutationGroupInterface, equality) {
	using Group = TypeParam;

	std::vector< perm::ExplicitPermutation > firstList = {
		perm::ExplicitPermutation(),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 2 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2 })),
	};
	std::vector< perm::ExplicitPermutation > secondList = {
		perm::ExplicitPermutation(),
		firstList[1],
		perm::ExplicitPermutation(perm::Cycle({ 3, 5 })),
		perm::ExplicitPermutation(perm::Cycle({ 5, 4 })),
		perm::ExplicitPermutation(perm::Cycle({ { 1, 2 }, { 4, 5 } })),
	};
	std::vector< perm::ExplicitPermutation > thirdList = {
		perm::ExplicitPermutation(),
		firstList[1],
		secondList[2],
		perm::ExplicitPermutation(perm::Cycle({ { 6, 7 }, { 4, 8 } })),
		perm::ExplicitPermutation(perm::Cycle({ 2, 7 })),
	};

	std::vector< Group > groups;

	for (const perm::ExplicitPermutation &first : firstList) {
		for (const perm::ExplicitPermutation &second : secondList) {
			for (const perm::ExplicitPermutation &third : thirdList) {
				Group group;
				group.addGenerator(perm::Permutation{ first });
				group.addGenerator(perm::Permutation{ second });
				group.addGenerator(perm::Permutation{ third });

				groups.push_back(std::move(group));
			}
		}
	}

	for (const Group &outer : groups) {
		const perm::AbstractPermutationGroup &outerGroup = outer;

		std::vector< perm::Permutation > outerElements;
		outerGroup.getElementsTo(outerElements);

		ASSERT_EQ(outerGroup, outerGroup);

		for (const Group &inner : groups) {
			const perm::AbstractPermutationGroup &innerGroup = inner;

			std::vector< perm::Permutation > innerElements;
			innerGroup.getElementsTo(innerElements);

			const bool groupsAreEqual =
				outerElements.size() == innerElements.size()
				&& std::is_permutation(outerElements.begin(), outerElements.end(), innerElements.begin());

			ASSERT_EQ(innerGroup, innerGroup);

			if (groupsAreEqual) {
				ASSERT_EQ(outerGroup, innerGroup);
				ASSERT_EQ(innerGroup, outerGroup);
			} else {
				ASSERT_NE(outerGroup, innerGroup);
				ASSERT_NE(innerGroup, outerGroup);
			}
		}
	}
}


TYPED_TEST(PermutationGroupInterface, orbit) {
	using Group = TypeParam;

	const Group g                               = fromGenerators< Group >({ perm::Cycle({ 1, 2, 0 }) });
	const perm::AbstractPermutationGroup &group = g;

	std::vector< perm::AbstractPermutation::value_type > expectedOrbit = { 3 };
	ASSERT_EQ(group.orbit(3), expectedOrbit);

	expectedOrbit = { 1, 2, 0 };
	ASSERT_EQ(group.orbit(1), expectedOrbit);
}


TYPED_TEST(PermutationGroupInterface, order) {
	using Group = TypeParam;

	const std::vector< std::vector< perm::Cycle > > cycles = {
		{ perm::Cycle() },
		{ perm::Cycle({ 1, 2 }) },
		{ perm::Cycle({ 0, 1, 2 }) },
		{ perm::Cycle({ 0, 1, 2 }), perm::Cycle({ 2, 3, 4 }) },
		{ perm::Cycle({ 0, 1, 2 }), perm::Cycle({ 2, 3, 4 }), perm::Cycle({ 0, 1 }) },
	};
	const std::vector< std::size_t > expectedOrders = { 1, 2, 3, 60, 120 };

	ASSERT_EQ(cycles.size(), expectedOrders.size());

	for (std::size_t i = 0; i < cycles.size(); ++i) {
		const Group g                               = fromGenerators< Group >(cycles[i]);
		const perm::AbstractPermutationGroup &group = g;

		ASSERT_EQ(group.order(), expectedOrders[i]);
	}
}


TYPED_TEST(PermutationGroupInterface, contains) {
	using Group = TypeParam;

	const Group g = fromGenerators< Group >({ perm::Cycle({ 0, 1 }), perm::Cycle({ 2, 3 }) });
	const perm::AbstractPermutationGroup &group = g;

	ASSERT_TRUE(group.contains(perm::ExplicitPermutation(perm::Cycle({ 0, 1 }))));
	ASSERT_TRUE(group.contains(perm::ExplicitPermutation(perm::Cycle({ { 0, 1 }, { 2, 3 } }))));
	ASSERT_FALSE(group.contains(perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2 }))));
}


TYPED_TEST(PermutationGroupInterface, setGenerators) {
	using Group = TypeParam;

	Group g;
	perm::AbstractPermutationGroup &group = g;

	ASSERT_EQ(g.order(), static_cast< std::size_t >(1));

	group.setGenerators({ perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2 }))) });

	ASSERT_EQ(g.order(), static_cast< std::size_t >(3));
}


TYPED_TEST(PermutationGroupInterface, cosets) {
	using Group                       = TypeParam;
	const Group H                     = fromGenerators< Group >({ perm::Cycle({ 0, 1, 2 }) });
	const perm::ExplicitPermutation p = perm::ExplicitPermutation(perm::Cycle({ 2, 3 }));

	const std::vector< perm::Permutation > leftCoset  = H.leftCoset(p);
	const std::vector< perm::Permutation > rightCoset = H.rightCoset(p);

	const std::vector< perm::Permutation > expectedLeftCoset = {
		perm::Permutation(p),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2, 3 }))),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 2, 3, 1 }))),
	};
	const std::vector< perm::Permutation > expectedRightCoset = {
		perm::Permutation(p),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1, 3, 2 }))),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 3, 2, 1 }))),
	};

	EXPECT_THAT(leftCoset, ::testing::UnorderedElementsAreArray(expectedLeftCoset));
	EXPECT_THAT(rightCoset, ::testing::UnorderedElementsAreArray(expectedRightCoset));
}


TYPED_TEST(PermutationGroupInterface, cosetRepresentatives) {
	using Group = TypeParam;

	// All of these generator sets generate Sym(6)
	const std::vector< std::vector< perm::Cycle > > s6GeneratorSets = {
		{ perm::Cycle({ 0, 1, 2, 3, 4, 5 }), perm::Cycle({ 0, 1 }) },
		{ perm::Cycle({ 0, 1, 2, 3, 4, 5 }), perm::Cycle({ 1, 2 }) },
		{ perm::Cycle({ 0, 1, 2, 3 }), perm::Cycle({ 3, 4, 5 }) },
	};

	bool assignedRepresentative = false;
	perm::Permutation rightRepresentative;
	perm::Permutation leftRepresentative;
	const perm::ExplicitPermutation cosetGenerator = perm::ExplicitPermutation(perm::Cycle({ 2, 5 }));

	for (const std::vector< perm::Cycle > &currentGenerators : s6GeneratorSets) {
		const Group g                               = fromGenerators< Group >(currentGenerators);
		const perm::AbstractPermutationGroup &group = g;

		ASSERT_EQ(group.order(), 720);

		// Since the group is always the same, we expect the canonical coset representative to be the same regardless of
		// what generators were used to generate the group as well.
		if (!assignedRepresentative) {
			leftRepresentative     = group.leftCosetRepresentative(cosetGenerator);
			rightRepresentative    = group.rightCosetRepresentative(cosetGenerator);
			assignedRepresentative = true;
		} else {
			ASSERT_EQ(group.leftCosetRepresentative(cosetGenerator), leftRepresentative);
			ASSERT_EQ(group.rightCosetRepresentative(cosetGenerator), rightRepresentative);
		}

		// Since the group is the full Sym(6), any coset with the group with a permutation acting on the
		// elements [0, 5] will necessarily be the group itself again and therefore the coset representative
		// must still be the same as well.
		for (const perm::AbstractPermutation &currentPerm : {
				 perm::ExplicitPermutation(perm::Cycle({ 1, 2, 3 })),
				 perm::ExplicitPermutation(perm::Cycle({ 1, 5 })),
				 perm::ExplicitPermutation(perm::Cycle({ { 0, 2 }, { 4, 5 } })),
				 perm::ExplicitPermutation(perm::Cycle({ { 5, 1 }, { 2, 5 }, { 0, 2, 3 } })),
			 }) {
			ASSERT_EQ(group.leftCosetRepresentative(currentPerm), leftRepresentative);
			ASSERT_EQ(group.rightCosetRepresentative(currentPerm), rightRepresentative);
		}
	}


	// These generators describe the symmetries of an pairwise-symmetric 4-index tensor
	// t^{ab}_{ij} = t^{ba}_{ij} = t^{ba}_{ji} = t^{ab}_{ji}
	const std::vector< std::vector< perm::Cycle > > generatorSets = {
		{
			perm::Cycle({ 0, 1 }),
			perm::Cycle({ 2, 3 }),
		},
		{
			perm::Cycle({ { 0, 1 }, { 2, 3 } }),
			perm::Cycle({ 0, 1 }),
		},
	};

	const std::vector< perm::ExplicitPermutation > cosetGenerators{
		perm::ExplicitPermutation(),
		perm::ExplicitPermutation(perm::Cycle({ 0, 2 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 3 })),
		perm::ExplicitPermutation(perm::Cycle({ 1, 3 })),
		perm::ExplicitPermutation(perm::Cycle({ 1, 2 })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 2 }, { 1, 3 } })),
	};

	std::vector< perm::Permutation > leftRepresentatives;
	std::vector< perm::Permutation > rightRepresentatives;

	for (const std::vector< perm::Cycle > &currentGenerators : generatorSets) {
		const Group g                               = fromGenerators< Group >(currentGenerators);
		const perm::AbstractPermutationGroup &group = g;

		ASSERT_EQ(group.order(), static_cast< std::size_t >(4));

		// Each of the coset generators should generate a different coset, which in turn should have a unique
		// canonical coset representative
		for (const perm::ExplicitPermutation &currentCosetPerm : cosetGenerators) {
			leftRepresentative  = group.leftCosetRepresentative(currentCosetPerm);
			rightRepresentative = group.rightCosetRepresentative(currentCosetPerm);

			if (std::find(leftRepresentatives.begin(), leftRepresentatives.end(), leftRepresentative)
				!= leftRepresentatives.end()) {
				FAIL() << "Duplicate left coset representative " << leftRepresentative << " from coset perm "
					   << currentCosetPerm;
			}
			if (std::find(rightRepresentatives.begin(), rightRepresentatives.end(), rightRepresentative)
				!= rightRepresentatives.end()) {
				FAIL() << "Duplicate right coset representative " << rightRepresentative << " from coset perm "
					   << currentCosetPerm;
			}

			leftRepresentatives.push_back(std::move(leftRepresentative));
			rightRepresentatives.push_back(std::move(rightRepresentative));
		}

		leftRepresentatives.clear();
		rightRepresentatives.clear();
	}
}
