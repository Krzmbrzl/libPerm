// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/ExplicitPermutation.hpp"
#include "libperm/Cycle.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>

namespace perm {

ExplicitPermutation ExplicitPermutation::fromCycle(const Cycle &cycle) {
	return ExplicitPermutation(cycle.toImage< value_type >());
}

ExplicitPermutation::ExplicitPermutation(std::size_t n) : m_image(n) {
	std::iota(m_image.begin(), m_image.end(), 0);
}

ExplicitPermutation::ExplicitPermutation(const std::vector< value_type > &image) : m_image(image) {
	// Assert that the image point contains all points in [0, n) where n = m_image.size()
	assert(std::accumulate(m_image.begin(), m_image.end(), static_cast< std::size_t >(0))
		   == m_image.size() * (m_image.size() - 1) / 2);
}

ExplicitPermutation::ExplicitPermutation(std::vector< value_type > &&image) : m_image(std::move(image)) {
	// Assert that the image point contains all points in [0, n) where n = m_image.size()
	assert(std::accumulate(m_image.begin(), m_image.end(), static_cast< std::size_t >(0))
		   == m_image.size() * (m_image.size() - 1) / 2);
}

ExplicitPermutation::~ExplicitPermutation() {
}

ExplicitPermutation::value_type ExplicitPermutation::n() const {
	return static_cast< value_type >(m_image.size());
}

ExplicitPermutation::value_type ExplicitPermutation::image(value_type value) const {
	assert(value < n());
	return m_image[value];
}

const std::vector< ExplicitPermutation::value_type > &ExplicitPermutation::image() const {
	return m_image;
}

void ExplicitPermutation::invert() {
	std::vector< value_type > inverseImage(n());

	for (value_type i = 0; i < n(); ++i) {
		inverseImage[m_image[i]] = i;
	}

	m_image = inverseImage;
}

void ExplicitPermutation::multiply(const AbstractPermutation &other) {
	assert(other.n() == n());

	for (value_type i = 0; i < n(); ++i) {
		m_image[i] = other.image(m_image[i]);
	}
}

void ExplicitPermutation::insertIntoStream(std::ostream &stream) const {
	// Represent this object in disjoint cycle notation
	stream << Cycle::fromImage(m_image);
}

ExplicitPermutation operator*(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs) {
	ExplicitPermutation result(lhs);

	result *= rhs;

	return result;
}

} // namespace perm
