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

namespace perm {

PrimitivePermutationGroup::PrimitivePermutationGroup() : AbstractPermutationGroup(PermutationGroupType::Primitive) {
	// Add the identity element as a generator (this is the least a group can contain - aka: There is no such thing as
	// an empty group)
	setGenerators({ Permutation(ExplicitPermutation()) });
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

	regenerateGroup();
}

void PrimitivePermutationGroup::getElementsTo(std::vector< Permutation > &permutations) const {
	permutations.clear();
	permutations.reserve(m_elements.size());

	for (const Permutation &current : m_elements) {
		permutations.push_back(current);
	}
}

void PrimitivePermutationGroup::regenerateGroup() {
	m_elements = DiminoAlgorithm::generateGroupElements(m_generators);
}

} // namespace perm
