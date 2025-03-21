// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/Cycle.hpp"

#include <algorithm>
#include <cassert>
#include <ostream>
#include <utility>

namespace perm {

Cycle::Cycle() : m_cycle(1, 0) {
}

Cycle::iterator Cycle::begin() {
	return m_cycle.begin();
}
Cycle::iterator Cycle::end() {
	return m_cycle.end();
}
Cycle::const_iterator Cycle::begin() const {
	return m_cycle.begin();
}
Cycle::const_iterator Cycle::end() const {
	return m_cycle.end();
}
Cycle::const_iterator Cycle::cbegin() const {
	return m_cycle.cbegin();
}
Cycle::const_iterator Cycle::cend() const {
	return m_cycle.cend();
}

std::size_t Cycle::size() const {
	return m_cycle.size();
}

Cycle::image_type Cycle::at(std::size_t pos) const {
	return m_cycle.at(pos);
}

Cycle::image_type Cycle::operator[](std::size_t pos) const {
	assert(pos < m_cycle.size());
	return m_cycle[pos];
}

std::ostream &operator<<(std::ostream &stream, const Cycle &cycle) {
	stream << "{";
	for (std::size_t i = 0; i < cycle.size(); ++i) {
		stream << cycle[i];

		if (i + 1 < cycle.size()) {
			stream << ", ";
		}
	}

	return stream << "}";
}

void Cycle::ensureCanonicalOrder() {
	auto it = std::max_element(m_cycle.begin(), m_cycle.end());
	if (it == m_cycle.end() || it == m_cycle.begin()) {
		return;
	}

	// Make sure the cycle starts with the biggest element
	std::rotate(m_cycle.begin(), it, m_cycle.end());
}

} // namespace perm
