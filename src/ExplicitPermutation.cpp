// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/ExplicitPermutation.hpp"
#include "libperm/Cycle.hpp"

#include <algorithm>
#include <cassert>
#include <set>

#include <iostream>

namespace perm {

ExplicitPermutation ExplicitPermutation::fromCycle(const Cycle &cycle, int sign) {
	return ExplicitPermutation(cycle.toImage< value_type >(), sign);
}

ExplicitPermutation::ExplicitPermutation(int sign) : details::SignedPermutation(sign), m_image(1, 0) {
}

ExplicitPermutation::ExplicitPermutation(std::vector< value_type > image, int sign)
	: details::SignedPermutation(sign), m_image(std::move(image)) {
	// Assert that the image point contains all points in [0, n) where n = m_image.size()
	assert(std::accumulate(m_image.begin(), m_image.end(), static_cast< std::size_t >(0))
		   == m_image.size() * (m_image.size() - 1) / 2);

	reduceImageRepresentation();
}

ExplicitPermutation::~ExplicitPermutation() {
}

ExplicitPermutation::value_type ExplicitPermutation::maxElement() const {
	assert(!m_image.empty());
	return static_cast< value_type >(m_image.size() - 1);
}

ExplicitPermutation::value_type ExplicitPermutation::image(value_type value) const {
	if (value >= m_image.size()) {
		return value;
	} else {
		return m_image[value];
	}
}

const std::vector< ExplicitPermutation::value_type > &ExplicitPermutation::image() const {
	return m_image;
}

void ExplicitPermutation::invert() {
	details::SignedPermutation::invert();

	std::vector< value_type > inverseImage(m_image.size());

	for (value_type i = 0; i < inverseImage.size(); ++i) {
		inverseImage[m_image[i]] = i;
	}

	m_image = inverseImage;
}

void ExplicitPermutation::multiply(const AbstractPermutation &other) {
	details::SignedPermutation::multiply(other);

	// We need to create a copy, in case &other == this
	value_type overallMaxElement = std::max(maxElement(), other.maxElement());

	decltype(m_image) transformedImage(overallMaxElement + 1);

	// First, transform elements in our image
	for (value_type i = 0; i <= maxElement(); ++i) {
		transformedImage[i] = other.image(m_image[i]);
	}
	// Then, potentially add additional image points describing transformation of higher elements
	// (that were so far untouched by this permutation)
	for (value_type i = maxElement() + 1; i <= overallMaxElement; ++i) {
		transformedImage[i] = other.image(i);
	}

	m_image = std::move(transformedImage);

	// Assert that multiplication has not created any duplicate entries
	assert(std::set< value_type >(m_image.begin(), m_image.end()).size() == m_image.size());

	reduceImageRepresentation();
}

void ExplicitPermutation::insertIntoStream(std::ostream &stream) const {
	// Represent this object in disjoint cycle notation
	stream << (sign() < 0 ? "-" : "+") << Cycle::fromImage(m_image);
}

ExplicitPermutation operator*(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs) {
	ExplicitPermutation result(lhs);

	result *= rhs;

	return result;
}

void ExplicitPermutation::reduceImageRepresentation() {
	assert(!m_image.empty());

	// Shrink transformedImage by all righthand entries that map to themselves
	// (but ensure to keep at least a single entry)
	value_type i;
	for (i = static_cast< value_type >(m_image.size() - 1); i > 0; --i) {
		if (m_image[i] != i) {
			break;
		}
	}

	// The element that above loop stopped at is the last one that we want to keep
	i++;

	assert(i > 0);

	if (i < m_image.size()) {
		m_image.erase(m_image.begin() + i, m_image.end());
	}

	assert(!m_image.empty());
}

} // namespace perm
