// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "Cycle.hpp"

#include <algorithm>
#include <cassert>

namespace perm {

Cycle::Cycle(std::vector< Cycle::value_type > cycle) {
	m_cycles.push_back(std::move(cycle));
}

Cycle::Cycle(std::vector< std::vector< Cycle::value_type > > cycles) : m_cycles(std::move(cycles)) {
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

bool operator==(const Cycle &lhs, const Cycle &rhs) {
	return lhs.maxElement() == rhs.maxElement() && lhs.toImage< unsigned int >() == rhs.toImage< unsigned int >();
}

bool operator!=(const Cycle &lhs, const Cycle &rhs) {
	return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &stream, const Cycle &cycle) {
	bool isIdentity = true;
	for (const std::vector< Cycle::value_type > &currentCycle : cycle.m_cycles) {
		if (currentCycle.size() == 1) {
			continue;
		}

		isIdentity = false;

		stream << "( ";
		for (Cycle::value_type currentValue : currentCycle) {
			stream << currentValue << " ";
		}
		stream << ")";
	}

	if (isIdentity) {
		stream << "()";
	}

	return stream;
}

Cycle::value_type Cycle::maxElement() const {
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


} // namespace perm
