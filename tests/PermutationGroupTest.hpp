// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_TESTS_PERMUTATIONGROUPTEST_HPP_
#define LIBPERM_TESTS_PERMUTATIONGROUPTEST_HPP_

#include <libperm/AbstractPermutationGroup.hpp>
#include <libperm/Cycle.hpp>
#include <libperm/ExplicitPermutation.hpp>

#include <algorithm>

#include <gtest/gtest.h>

namespace perm::test {

template< typename PermutationGroupImpl > void testPermutationGroupInterface() {
	PermutationGroupImpl groupImpl;
	AbstractPermutationGroup &group = groupImpl;

	// Group only contains identity
	ASSERT_EQ(group.order(), static_cast< std::size_t >(1));
	std::vector< AbstractPermutation::value_type > orbit = group.orbit(3);
	ASSERT_EQ(orbit.size(), static_cast< std::size_t >(1));
	ASSERT_EQ(orbit[0], static_cast< AbstractPermutation::value_type >(3));


	group.addGenerator(perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }))));
	ASSERT_EQ(group.order(), static_cast< std::size_t >(2));
	orbit = group.orbit(3);
	ASSERT_EQ(orbit.size(), static_cast< std::size_t >(1));
	ASSERT_EQ(orbit[0], static_cast< AbstractPermutation::value_type >(3));



	group.addGenerator(perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 3, 1 }))));
	ASSERT_EQ(group.order(), static_cast< std::size_t >(6));
	orbit                                                        = group.orbit(3);
	std::vector< AbstractPermutation::value_type > expectedOrbit = { 3, 1, 0 };
	ASSERT_EQ(orbit.size(), expectedOrbit.size());
	ASSERT_TRUE(std::is_permutation(orbit.begin(), orbit.end(), expectedOrbit.begin()));



	group.setGenerators({ perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }))),
						  perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 3, 4, 5 }))) });
	ASSERT_EQ(group.order(), static_cast< std::size_t >(8));
	orbit         = group.orbit(3);
	expectedOrbit = { 3, 4, 5, 2 };
	ASSERT_EQ(orbit.size(), expectedOrbit.size());
	ASSERT_TRUE(std::is_permutation(orbit.begin(), orbit.end(), expectedOrbit.begin()));
}

template< typename PermutationGroupImpl > void testCosetRepresentative() {
	// All of these generator sets generate Sym(6)
	const std::vector< std::vector< perm::Permutation > > s6GeneratorSets = {
		{ perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2, 3, 4, 5 }))),
		  perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }))) },
		{ perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2, 3, 4, 5 }))),
		  perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2 }))) },
		{ perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2, 3 }))),
		  perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 3, 4, 5 }))) },
	};

	bool assignedRepresentative = false;
	perm::Permutation representative;

	for (const std::vector< perm::Permutation > &currentGenerators : s6GeneratorSets) {
		PermutationGroupImpl groupImpl;
		AbstractPermutationGroup &group = groupImpl;

		group.setGenerators(currentGenerators);

		ASSERT_EQ(group.order(), 720);

		// Since the group is always the same, we expect the canonical coset representative to be the same regardless of
		// what generators were used to generate the group as well.
		if (!assignedRepresentative) {
			representative = group.getCanonicalCosetRepresentative();
		} else {
			ASSERT_EQ(group.getCanonicalCosetRepresentative(), representative);
		}

		// Since the group is the full Sym(6), any coset with the group with a permutation acting on the
		// elements [0, 5] will necessarily be the group itself again and therefore the coset representative
		// must still be the same as well.
		for (const perm::AbstractPermutation &currentPerm : {
				 perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2, 3 })),
				 perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 5 })),
				 perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 0, 2 }, { 4, 5 } })),
				 perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 5, 1 }, { 2, 5 }, { 0, 2, 3 } })),
			 }) {
			ASSERT_EQ(group.getCanonicalCosetRepresentative(currentPerm), representative);
		}
	}


	// These generators describe the symmetries of an antisymmetric 4-index tensor
	// t^{ab}_{ij} = -t^{ba}_{ij} = t^{ba}_{ji} = -t^{ab}_{ji}
	// the indices in the first tensor are arranged as [a,b,i,j], which will also be considered
	// the "standard configuration"
	const std::vector< std::vector< perm::Permutation > > generatorSets = {
		{
			perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }), -1)),
			perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 3 }), -1)),
		},
		{
			perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 0, 1 }, { 2, 3 } }))),
			perm::Permutation(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 }), -1)),
		},
	};

	const std::vector< perm::ExplicitPermutation > cosetGenerators{
		perm::ExplicitPermutation(),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 2 })),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 3 })),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 3 })),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 2 })),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 0, 2 }, { 1, 3 } })),
	};

	std::vector< perm::Permutation > representatives;

	for (const std::vector< perm::Permutation > &currentGenerators : generatorSets) {
		PermutationGroupImpl groupImpl;
		AbstractPermutationGroup &group = groupImpl;

		group.setGenerators(currentGenerators);

		ASSERT_EQ(group.order(), static_cast< std::size_t >(4));

		// Each of the coset generators should generate a different coset, which in turn should have a unique
		// canonical coset representative
		for (const perm::ExplicitPermutation &currentCosetPerm : cosetGenerators) {
			representative = group.getCanonicalCosetRepresentative(currentCosetPerm);

			if (std::find(representatives.begin(), representatives.end(), representative) != representatives.end()) {
				FAIL() << "Duplicate coset representative " << representative << " from coset perm "
					   << currentCosetPerm;
			}

			representatives.push_back(std::move(representative));
		}

		representatives.clear();
	}
}

template< typename PermutationGroupImpl > void testEquality() {
	std::vector< perm::ExplicitPermutation > firstList = {
		perm::ExplicitPermutation(),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1 })),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 2 })),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2 })),
	};
	std::vector< perm::ExplicitPermutation > secondList = {
		perm::ExplicitPermutation(),
		firstList[1],
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 3, 5 })),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 5, 4 })),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 1, 2 }, { 4, 5 } })),
	};
	std::vector< perm::ExplicitPermutation > thirdList = {
		perm::ExplicitPermutation(),
		firstList[1],
		secondList[2],
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 6, 7 }, { 4, 8 } })),
		perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 7 })),
	};

	std::vector< PermutationGroupImpl > groups;

	for (const perm::ExplicitPermutation &first : firstList) {
		for (const perm::ExplicitPermutation &second : secondList) {
			for (const perm::ExplicitPermutation &third : thirdList) {
				PermutationGroupImpl group;
				group.addGenerator(perm::Permutation{ first });
				group.addGenerator(perm::Permutation{ second });
				group.addGenerator(perm::Permutation{ third });

				groups.push_back(std::move(group));
			}
		}
	}

	for (const PermutationGroupImpl &outer : groups) {
		std::vector< perm::Permutation > outerElements;
		outer.getElementsTo(outerElements);

		for (const PermutationGroupImpl &inner : groups) {
			std::vector< perm::Permutation > innerElements;
			inner.getElementsTo(innerElements);

			const bool groupsAreEqual =
				outerElements.size() == innerElements.size()
				&& std::is_permutation(outerElements.begin(), outerElements.end(), innerElements.begin());

			ASSERT_EQ(outer == inner, groupsAreEqual) << "outer: " << outer << " - inner: " << inner;
		}
	}
}

} // namespace perm::test

#endif // LIBPERM_TESTS_PERMUTATIONGROUPTEST_HPP_
