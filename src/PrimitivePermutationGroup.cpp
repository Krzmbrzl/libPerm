// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/PrimitivePermutationGroup.hpp"
#include "libperm/AbstractPermutation.hpp"
#include "libperm/DiminoAlgorithm.hpp"
#include "libperm/ExplicitPermutation.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace perm {

PrimitivePermutationGroup::PrimitivePermutationGroup() : AbstractPermutationGroup(PermutationGroupType::Primitive) {
	setGenerators({});
}

PrimitivePermutationGroup::PrimitivePermutationGroup(std::vector< Permutation > generators)
	: AbstractPermutationGroup(PermutationGroupType::Primitive) {
	setGenerators(std::move(generators));
}

std::vector< AbstractPermutation::value_type >
	PrimitivePermutationGroup::orbit(AbstractPermutation::value_type point) const {
	std::vector< AbstractPermutation::value_type > orbit;

	for (const Permutation &current : m_elements) {
		AbstractPermutation::value_type image = current->image(point);

		if (std::find(orbit.begin(), orbit.end(), image) == orbit.end()) {
			orbit.push_back(std::move(image));
		}
	}

	return orbit;
}

std::size_t PrimitivePermutationGroup::order() const {
	return m_elements.size();
}

bool PrimitivePermutationGroup::contains(const AbstractPermutation &perm) const {
	// TODO: Improve speed of lookup by e.g. keeping elements sorted
	return std::find(m_elements.begin(), m_elements.end(), perm) != m_elements.end();
}

bool PrimitivePermutationGroup::addGenerator(Permutation perm) {
	if (!contains(perm.get())) {
		m_generators.push_back(std::move(perm));
		return DiminoAlgorithm::extendGroup(m_elements, m_generators, m_generators.size() - 1);
	} else {
		return false;
	}
}

void PrimitivePermutationGroup::setGenerators(std::vector< Permutation > generators) {
	m_generators = std::move(generators);

	if (m_generators.empty()) {
		// There is no such thing as an empty group. It must always at least contain the identity element
		m_generators.emplace_back(ExplicitPermutation());
	}

	regenerateGroup();
}

const std::vector< Permutation > &PrimitivePermutationGroup::getGenerators() const {
	return m_generators;
}

void PrimitivePermutationGroup::getElementsTo(std::vector< Permutation > &permutations) const {
	permutations.clear();
	permutations.reserve(m_elements.size());

	for (const Permutation &current : m_elements) {
		permutations.push_back(current);
	}
}

enum class Coset { Left, Right };

template< Coset cosetType >
std::vector< Permutation > computeCoset(const AbstractPermutation &perm, const std::vector< Permutation > &elements) {
	std::vector< Permutation > coset;
	coset.reserve(elements.size());

	for (Permutation currentElement : elements) {
		if constexpr (cosetType == Coset::Left) {
			currentElement->preMultiply(perm);
		} else {
			currentElement->postMultiply(perm);
		}

		coset.push_back(std::move(currentElement));
	}

	return coset;
}

std::vector< Permutation > PrimitivePermutationGroup::leftCoset(const AbstractPermutation &perm) const {
	return computeCoset< Coset::Left >(perm, m_elements);
}

std::vector< Permutation > PrimitivePermutationGroup::rightCoset(const AbstractPermutation &perm) const {
	return computeCoset< Coset::Right >(perm, m_elements);
}

void PrimitivePermutationGroup::regenerateGroup() {
	m_elements = DiminoAlgorithm::generateGroupElements(m_generators);
}

std::ostream &operator<<(std::ostream &stream, const PrimitivePermutationGroup &group) {
	stream << "group generated by { ";
	for (std::size_t i = 0; i < group.m_generators.size(); ++i) {
		stream << group.m_generators[i];

		if (i + 1 < group.m_generators.size()) {
			stream << ", ";
		}
	}

	return stream << "}";
}

struct Canonicalizer {
	bool operator()(const Permutation &lhs, const Permutation &rhs) const {
		/*
		 * The idea used to establish an order between the different permutation elements (required for a minimum
		 * element to exist) consists in defining a set B that contains all numbers between 0 and n (the elements the
		 * given permutation objects act on) in a defined order. This can then be used to establish an order between two
		 * image points: a < b, if a comes before b in B (going left-to-right). Thus, sets of numbers can be ordered: A
		 * < B, if for the first element (a,b) in which they differ a < b. Now permutations are ordered by taking the
		 * pointwise image B^g into account, which is defined as {b^g | b in B} and ^ means applying the permutation to
		 * the respective object. Thus: p1 < p2, if B^p1 < B^p2
		 *
		 * Since we don't have any reason to choose B otherwise, we can simply take it as the numbers 0, 1, ..., n in
		 * ascending order. Therefore, we can simply compare two images numerically to determine if one comes before the
		 * other in B.
		 */
		const AbstractPermutation::value_type n = std::max(lhs->maxElement(), rhs->maxElement());

		for (AbstractPermutation::value_type i = 0; i <= n; ++i) {
			AbstractPermutation::value_type lhsImage = lhs->image(i);
			AbstractPermutation::value_type rhsImage = rhs->image(i);
			if (lhsImage != rhsImage) {
				return lhsImage < rhsImage;
			}
		}

		// Permutations are identical (this is something that we don't expect to happen as the elements passed
		// in should represent a group and therefore there should be no duplicates.
		assert(false);
		return false;
	}
};

Permutation PrimitivePermutationGroup::leftCosetRepresentative(const AbstractPermutation &perm) const {
	assert(!m_elements.empty());

	if (perm.isIdentity() || contains(perm)) {
		// If perm is contained in this group (which is guaranteed, if perm == identity), the resulting coset
		// will just be the group itself, so there is no point in explicitly calculating the coset.
		return *std::min_element(m_elements.begin(), m_elements.end(), Canonicalizer{});
	}

	std::vector< Permutation > coset = computeCoset< Coset::Left >(perm, m_elements);
	return *std::min_element(coset.begin(), coset.end(), Canonicalizer{});
}

Permutation PrimitivePermutationGroup::rightCosetRepresentative(const AbstractPermutation &perm) const {
	assert(!m_elements.empty());

	if (perm.isIdentity() || contains(perm)) {
		// If perm is contained in this group (which is guaranteed, if perm == identity), the resulting coset
		// will just be the group itself, so there is no point in explicitly calculating the coset.
		return *std::min_element(m_elements.begin(), m_elements.end(), Canonicalizer{});
	}

	std::vector< Permutation > coset = computeCoset< Coset::Right >(perm, m_elements);
	return *std::min_element(coset.begin(), coset.end(), Canonicalizer{});
}

} // namespace perm
