// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/DisjointCycles.hpp"
#include "libperm/Cycle.hpp"

#include <algorithm>
#include <cassert>
#include <compare>
#include <ostream>
#include <ranges>
#include <set>

namespace perm {

DisjointCycles::DisjointCycles(Cycle cycle) : DisjointCycles(container_type({ std::move(cycle) })) {
}

DisjointCycles::iterator DisjointCycles::begin() {
	return m_cycles.begin();
}

DisjointCycles::iterator DisjointCycles::end() {
	return m_cycles.end();
}

DisjointCycles::const_iterator DisjointCycles::begin() const {
	return m_cycles.begin();
}

DisjointCycles::const_iterator DisjointCycles::end() const {
	return m_cycles.end();
}

DisjointCycles::const_iterator DisjointCycles::cbegin() const {
	return m_cycles.cbegin();
}

DisjointCycles::const_iterator DisjointCycles::cend() const {
	return m_cycles.cend();
}

std::size_t DisjointCycles::size() const {
	return m_cycles.size();
}

bool is_not_one_cycle(const Cycle &cycle) {
	return cycle.size() > 1;
}

std::strong_ordering DisjointCycles::operator<=>(const DisjointCycles &other) const {
	auto relevant_lhs = m_cycles | std::ranges::views::filter(is_not_one_cycle);
	auto relevant_rhs = other.m_cycles | std::ranges::views::filter(is_not_one_cycle);

	return std::lexicographical_compare_three_way(relevant_lhs.begin(), relevant_lhs.end(), relevant_rhs.begin(),
												  relevant_rhs.end());
}

bool DisjointCycles::operator==(const DisjointCycles &other) const {
	auto relevant_lhs = m_cycles | std::ranges::views::filter(is_not_one_cycle);
	auto relevant_rhs = other.m_cycles | std::ranges::views::filter(is_not_one_cycle);

	return std::ranges::equal(relevant_lhs, relevant_rhs);
}

std::ostream &operator<<(std::ostream &stream, const DisjointCycles &cycles) {
	bool isIdentity = true;
	for (const Cycle &current : cycles) {
		if (current.size() < 2) {
			continue;
		}

		isIdentity = false;

		stream << current;
	}

	if (isIdentity) {
		stream << "()";
	}

	return stream;
}

Cycle::image_type DisjointCycles::maxElement() const {
	Cycle::image_type max = 0;

	for (const Cycle &current : m_cycles) {
		auto maxElement = std::max_element(current.begin(), current.end());

		if (maxElement == current.end()) {
			continue;
		}

		max = std::max(max, *maxElement);
	}

	return max;
}

void DisjointCycles::ensureCanonicalOrder() {
	std::sort(m_cycles.begin(), m_cycles.end());
}

bool DisjointCycles::isDisjoint() const {
	std::set< Cycle::image_type > seen;

	for (const Cycle &cycle : m_cycles) {
		for (Cycle::image_type current : cycle) {
			if (seen.find(current) != seen.end()) {
				return false;
			}

			seen.insert(current);
		}
	}

	return true;
}

void DisjointCycles::splitCycles() {
	*this = fromImage(toImage< Cycle::image_type >(), true);
}

} // namespace perm
