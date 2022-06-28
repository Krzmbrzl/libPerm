// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "Cycle.hpp"

#include <algorithm>
#include <cassert>

namespace perm {

Cycle::Cycle(Cycle::value_type order) : m_cycles(), m_order(order) {
}

Cycle::Cycle(Cycle::value_type order, const std::vector< Cycle::value_type > &cycle) : m_order(order) {
	m_cycles.push_back(cycle);
	assert(m_order >= computeOrder());
}

Cycle::Cycle(Cycle::value_type order, std::vector< Cycle::value_type > &&cycle) : m_order(order) {
	m_cycles.push_back(std::move(cycle));
	assert(m_order >= computeOrder());
}

Cycle::Cycle(Cycle::value_type order, const std::vector< std::vector< Cycle::value_type > > &cycles)
	: m_cycles(cycles), m_order(order) {
	assert(m_order >= computeOrder());
}

Cycle::Cycle(Cycle::value_type order, std::vector< std::vector< Cycle::value_type > > &&cycles)
	: m_cycles(std::move(cycles)), m_order(order) {
	assert(m_order >= computeOrder());
}

Cycle::iterator Cycle::begin() {
	return m_cycles.begin();
}

Cycle::iterator Cycle::end() {
	return m_cycles.end();
}

Cycle::const_iterator Cycle::begin() const {
	return m_cycles.begin();
}

Cycle::const_iterator Cycle::end() const {
	return m_cycles.end();
}

Cycle::const_iterator Cycle::cbegin() const {
	return m_cycles.cbegin();
}

Cycle::const_iterator Cycle::cend() const {
	return m_cycles.cend();
}

Cycle::value_type Cycle::order() const {
	return m_order;
}

Cycle::value_type Cycle::computeOrder() const {
	Cycle::value_type max = 0;

	for (const std::vector< Cycle::value_type > &currentCycle : m_cycles) {
		auto maxElement = std::max_element(currentCycle.begin(), currentCycle.end());

		if (maxElement == currentCycle.end()) {
			continue;
		}

		max = std::max(max, *maxElement);
	}

	return max;
}

bool operator==(const Cycle &lhs, const Cycle &rhs) {
	return lhs.order() == rhs.order() && lhs.toImage< unsigned int >() == rhs.toImage< unsigned int >();
}

bool operator!=(const Cycle &lhs, const Cycle &rhs) {
	return !(lhs == rhs);
}

} // namespace perm
