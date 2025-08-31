// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/DisjointCycles.hpp>
#include <libperm/ExplicitPermutation.hpp>
#include <libperm/Permutation.hpp>
#include <libperm/SchreierSims.hpp>

#include <span>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(SchreierSims, orbitTransversalStabilizer) {
	// Test taken from https://blogs.cs.st-andrews.ac.uk/codima/files/2015/11/CoDiMa2015_Holt.pdf
	std::vector< perm::Permutation > generators = { perm::ExplicitPermutation(perm::DisjointCycles({ 0, 1, 2 })),
													perm::ExplicitPermutation(perm::DisjointCycles({ 0, 3 })) };

	auto [transversal, stabilizers] = perm::orbitTransversalStabilizer(0, generators);

	decltype(transversal) expectedTransversal = {
		{ 0, perm::ExplicitPermutation() },
		{ 1, generators.front() },
		{ 2, generators.front() * generators.front() },
		{ 3, generators.back() },
	};

	decltype(stabilizers) expectedStabilizers = {
		perm::ExplicitPermutation(perm::DisjointCycles({ 2, 3 })),
		perm::ExplicitPermutation(),
		perm::ExplicitPermutation(perm::DisjointCycles({ 1, 3 })),
		perm::ExplicitPermutation(perm::DisjointCycles({ 1, 2, 3 })),
	};

	ASSERT_THAT(transversal, testing::UnorderedElementsAreArray(expectedTransversal));
	ASSERT_THAT(stabilizers, testing::UnorderedElementsAreArray(expectedStabilizers));
}
