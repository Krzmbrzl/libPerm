// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/Cycle.hpp>
#include <libperm/DiminoAlgorithm.hpp>
#include <libperm/ExplicitPermutation.hpp>
#include <libperm/Permutation.hpp>

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include <iostream>

constexpr std::size_t faculty(std::size_t i) {
	if (i == 0) {
		return 1;
	} else {
		return i * faculty(i - 1);
	}
}

void print(const std::vector< perm::Permutation > &perms) {
	std::cout << "{\n";
	for (const perm::Permutation &perm : perms) {
		perm->insertIntoStream(std::cout);
		std::cout << "\n";
	}
	std::cout << "}" << std::endl;
}

// Reference elements of the tested groups were generated via GAP 4.11

TEST(DiminoAlgorithm, generateGroupElements) {
	std::vector< perm::Permutation > generators;

	generators = { perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2, 3 }))) };

	std::vector< perm::Permutation > elements = perm::DiminoAlgorithm::generateGroupElements(generators);
	std::vector< perm::ExplicitPermutation > expectedElements = {
		perm::ExplicitPermutation(perm::Cycle()),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2, 3 })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 2 }, { 1, 3 } })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 3, 2, 1 })),
	};

	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));



	generators = { perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1 }))),
				   perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 2, 3, 4, 5 }))) };

	expectedElements = {
		perm::ExplicitPermutation(perm::Cycle()),
		perm::ExplicitPermutation(perm::Cycle({ 2, 3, 4, 5 })),
		perm::ExplicitPermutation(perm::Cycle({ { 2, 4 }, { 3, 5 } })),
		perm::ExplicitPermutation(perm::Cycle({ 2, 5, 4, 3 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1 })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 1 }, { 2, 3, 4, 5 } })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 1 }, { 2, 4 }, { 3, 5 } })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 1 }, { 2, 5, 4, 3 } })),
	};

	elements = perm::DiminoAlgorithm::generateGroupElements(generators);

	print(elements);

	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));



	generators = { perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2 }))),
				   perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1 }))) };

	expectedElements = {
		perm::ExplicitPermutation(perm::Cycle()),
		perm::ExplicitPermutation(perm::Cycle({ 1, 2 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 2, 1 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 2 })),
	};

	elements = perm::DiminoAlgorithm::generateGroupElements(generators);

	print(elements);

	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));



	generators = { perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2 }))),
				   perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1 }))) };

	expectedElements = {
		perm::ExplicitPermutation(perm::Cycle()),
		perm::ExplicitPermutation(perm::Cycle({ 1, 2 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 2, 1 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 2 })),
	};

	elements = perm::DiminoAlgorithm::generateGroupElements(generators);

	print(elements);

	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));



	generators = {
		perm::Permutation(perm::ExplicitPermutation(
			perm::Cycle(std::vector< std::vector< perm::Cycle::value_type > >{ { { 0, 1 }, { 2, 3 } } }))),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 3, 4 }))),
	};

	expectedElements = {
		perm::ExplicitPermutation(perm::Cycle()),
		perm::ExplicitPermutation(perm::Cycle({ 3, 4 })),
		perm::ExplicitPermutation(perm::Cycle({ 2, 3 })),
		perm::ExplicitPermutation(perm::Cycle({ 2, 3, 4 })),
		perm::ExplicitPermutation(perm::Cycle({ 2, 4, 3 })),
		perm::ExplicitPermutation(perm::Cycle({ 2, 4 })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1 })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 1 }, { 3, 4 } })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 1 }, { 2, 3 } })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 1 }, { 2, 3, 4 } })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 1 }, { 2, 4, 3 } })),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 1 }, { 2, 4 } })),
	};

	elements = perm::DiminoAlgorithm::generateGroupElements(generators);

	print(elements);

	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));
}

TEST(DiminoAlgorithm, generateGroupElementsSigned) {
	std::vector< perm::Permutation > generators;

	generators = { perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2, 3 }), -1)) };

	std::vector< perm::Permutation > elements = perm::DiminoAlgorithm::generateGroupElements(generators);
	std::vector< perm::ExplicitPermutation > expectedElements = {
		perm::ExplicitPermutation(perm::Cycle()),
		perm::ExplicitPermutation(perm::Cycle({ 0, 1, 2, 3 }), -1),
		perm::ExplicitPermutation(perm::Cycle({ { 0, 2 }, { 1, 3 } })),
		perm::ExplicitPermutation(perm::Cycle({ 0, 3, 2, 1 }), -1),
	};

	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));
}

TEST(DiminoAlgorithm, generateGroupOrder) {
	// The groups tested in here are too large to list their individual elements in a reasonable way. Therefore, we only
	// check the order of the generated group and assume that if the order is correct, the elements in the group are as
	// well.

	// These generators generate the complete symmetric group of order 6 (Sym(6))
	std::vector< perm::Permutation > generators = {
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 5, 2 }))),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 4, 5 }))),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 1, 3, 2 }))),
	};

	std::vector< perm::Permutation > elements = perm::DiminoAlgorithm::generateGroupElements(generators);

	ASSERT_EQ(elements.size(), faculty(6));



	generators = {
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 2, 4, 6 }))),
		perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 1 }))),
	};

	elements = perm::DiminoAlgorithm::generateGroupElements(generators);

	ASSERT_EQ(elements.size(), 120);
}

TEST(DiminoAlgorithm, extendGroup) {
	std::vector< perm::Permutation > generators;

	generators.push_back(perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 0, 3 }))));

	std::vector< perm::Permutation > elements = perm::DiminoAlgorithm::generateGroupElements(generators);
	std::vector< perm::ExplicitPermutation > expectedElements = {
		perm::ExplicitPermutation(perm::Cycle()),
		perm::ExplicitPermutation(perm::Cycle({ 0, 3 })),
	};

	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));



	generators.push_back(perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 1, 4 }))));

	expectedElements.push_back(perm::ExplicitPermutation(perm::Cycle({ 1, 4 })));
	expectedElements.push_back(perm::ExplicitPermutation(perm::Cycle({ { 0, 3 }, { 1, 4 } })));

	ASSERT_TRUE(perm::DiminoAlgorithm::extendGroup(elements, generators, generators.size() - 1));
	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));



	generators.push_back(perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ 2, 5 }))));

	expectedElements.push_back(perm::ExplicitPermutation(perm::Cycle({ 2, 5 })));
	expectedElements.push_back(perm::ExplicitPermutation(perm::Cycle({ { 1, 4 }, { 2, 5 } })));
	expectedElements.push_back(perm::ExplicitPermutation(perm::Cycle({ { 0, 3 }, { 2, 5 } })));
	expectedElements.push_back(perm::ExplicitPermutation(perm::Cycle({ { 0, 3 }, { 1, 4 }, { 2, 5 } })));

	ASSERT_TRUE(perm::DiminoAlgorithm::extendGroup(elements, generators, generators.size() - 1));
	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));


	// Use one of the already contained elements as a "new" generator
	// Thus, extending should be a no-op
	generators.push_back(perm::Permutation(perm::ExplicitPermutation(perm::Cycle({ { 0, 3 }, { 2, 5 } }))));

	ASSERT_FALSE(perm::DiminoAlgorithm::extendGroup(elements, generators, generators.size() - 1));
	ASSERT_EQ(elements.size(), expectedElements.size());
	ASSERT_TRUE(std::is_permutation(elements.begin(), elements.end(), expectedElements.begin()));
}
