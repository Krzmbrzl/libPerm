// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/ExplicitPermutation.hpp"
#include "libperm/DisjointCycles.hpp"

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <numeric>
#include <ostream>
#include <set>

namespace perm {

ExplicitPermutation::ExplicitPermutation(int sign) : details::SignedPermutation(sign), m_image(1, 0) {
}

ExplicitPermutation::ExplicitPermutation(std::initializer_list< image_type > image, int sign)
	: ExplicitPermutation(container_type(std::move(image)), sign) {
}

ExplicitPermutation::ExplicitPermutation(std::vector< image_type > image, int sign)
	: details::SignedPermutation(sign), m_image(std::move(image)) {
	// Assert that the image point contains all points in [0, n) where n = m_image.size()
	assert(std::accumulate(m_image.begin(), m_image.end(), static_cast< std::size_t >(0))
		   == m_image.size() * (m_image.size() - 1) / 2);

	if (!m_image.empty()) {
		reduceImageRepresentation();
	} else {
		// We don't want to deal with empty images
		m_image = { 0 };
	}
}

ExplicitPermutation::ExplicitPermutation(const DisjointCycles &cycles, int sign)
	: ExplicitPermutation(cycles.toImage< image_type, container_type >(), sign) {
}

ExplicitPermutation::~ExplicitPermutation() {
}

ExplicitPermutation::image_type ExplicitPermutation::maxElement() const {
	assert(!m_image.empty());
	return static_cast< image_type >(m_image.size() - 1);
}

ExplicitPermutation::image_type ExplicitPermutation::image(image_type value) const {
	if (value >= m_image.size()) {
		return value;
	} else {
		return m_image[value];
	}
}

const std::vector< ExplicitPermutation::image_type > &ExplicitPermutation::image() const {
	return m_image;
}

void ExplicitPermutation::invert() {
	std::vector< image_type > inverseImage(m_image.size());

	for (image_type i = 0; i < inverseImage.size(); ++i) {
		inverseImage[m_image[i]] = i;
	}

	m_image = inverseImage;
}

void ExplicitPermutation::preMultiply(const AbstractPermutation &other) {
	details::SignedPermutation::preMultiply(other);

	if (this == &other) {
		selfMultiply();
		return;
	}

	const image_type overallMaxElement = std::max(maxElement(), other.maxElement());

	decltype(m_image) transformedImage(overallMaxElement + 1);

	for (image_type i = 0; i <= overallMaxElement; ++i) {
		transformedImage[i] = image(other.image(i));
	}

	m_image = std::move(transformedImage);

	// Assert that multiplication has not created any duplicate entries
	assert(std::set< image_type >(m_image.begin(), m_image.end()).size() == m_image.size());

	reduceImageRepresentation();
}

void ExplicitPermutation::postMultiply(const AbstractPermutation &other) {
	details::SignedPermutation::postMultiply(other);

	if (this == &other) {
		// If we are multiplying this perm by itself, our in-place algorithm below
		// will get into trouble as it overrides image entries that might be needed
		// later, due to the two-step nature of computing the product of two perms
		// by means of their images.
		selfMultiply();
		return;
	}

	const image_type ownMax            = maxElement();
	const image_type overallMaxElement = std::max(ownMax, other.maxElement());

	m_image.resize(overallMaxElement + 1);

	// First, transform elements in our image
	for (image_type i = 0; i <= ownMax; ++i) {
		m_image[i] = other.image(m_image[i]);
	}
	// Then, potentially add additional image points describing transformation of higher elements
	// (that were so far untouched by this permutation)
	for (image_type i = ownMax + 1; i <= overallMaxElement; ++i) {
		m_image[i] = other.image(i);
	}

	// Assert that multiplication has not created any duplicate entries
	assert(std::set< image_type >(m_image.begin(), m_image.end()).size() == m_image.size());

	reduceImageRepresentation();
}

DisjointCycles ExplicitPermutation::toDisjointCycles(std::size_t N) const {
	return DisjointCycles::fromImage(m_image, N);
}

void ExplicitPermutation::shift(int shift, std::size_t startOffset) {
	if (startOffset >= m_image.size()) {
		return;
	}

	auto begin = m_image.begin() + static_cast< std::ptrdiff_t >(startOffset);

	// Handle image points i -> j where i >= startOffset
	if (shift >= 0) {
		m_image.insert(begin, static_cast< std::size_t >(shift), 0);

		// Insert invalidates iterators
		begin = m_image.begin() + static_cast< std::ptrdiff_t >(startOffset);

		std::iota(begin, begin + shift, static_cast< image_type >(startOffset));
	} else {
		const std::size_t upperBound = std::min(m_image.size(), startOffset + static_cast< std::size_t >(-shift) + 1);
		for (std::size_t i = startOffset; i < upperBound; ++i) {
			if (m_image.at(i) != static_cast< image_type >(i)) {
				// This part of the to-be-deleted range is part of an actual permutation -> ensure that
				// the information about this exchange is not lost.
				// We know that there are at least abs(shift) elements to the left of the i-th position
				// that map to themselves (prerequisite of this function). Thus, we can shift the exchange
				// by simply moving it abs(shift) elements to the left.
				m_image.at(i - static_cast< std::size_t >(-shift)) = m_image.at(i);
			}
		}

		m_image.erase(begin, begin
								 + static_cast< std::ptrdiff_t >(
									 std::min(m_image.size() - startOffset, static_cast< std::size_t >(-shift))));
	}

	// Handle image points i -> j where j >= startOffset
	for (std::size_t i = 0; i < m_image.size(); ++i) {
		if ((shift < 0 || m_image[i] != static_cast< image_type >(i))
			&& m_image[i] >= static_cast< image_type >(startOffset)) {
			assert(shift >= 0 || m_image[i] >= static_cast< image_type >(-shift));

			if (shift >= 0) {
				m_image[i] += static_cast< image_type >(shift);
			} else {
				m_image[i] -= static_cast< image_type >(-shift);
			}
		}
	}

	if (m_image.empty()) {
		// Ensure we always keep at least a single element in our image
		m_image.push_back(0);
	}

	reduceImageRepresentation();
}

void ExplicitPermutation::insertIntoStream(std::ostream &stream) const {
	// Represent this object in disjoint cycle notation
	stream << (sign() < 0 ? "-" : "+") << DisjointCycles::fromImage(m_image);
}

ExplicitPermutation operator*(const ExplicitPermutation &lhs, const AbstractPermutation &rhs) {
	ExplicitPermutation result(lhs);

	result.postMultiply(rhs);

	return result;
}

ExplicitPermutation operator*(const AbstractPermutation &lhs, const ExplicitPermutation &rhs) {
	ExplicitPermutation result(rhs);

	result.preMultiply(lhs);

	return result;
}

ExplicitPermutation operator*(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs) {
	// Post-multiplication is currently implemented more efficiently (in-place for most cases)
	ExplicitPermutation result(lhs);

	result.postMultiply(rhs);

	return result;
}

void ExplicitPermutation::selfMultiply() {
	decltype(m_image) transformedImage;
	transformedImage.resize(m_image.size());

	for (std::size_t i = 0; i < m_image.size(); ++i) {
		transformedImage[i] = m_image[m_image[i]];
	}

	m_image = std::move(transformedImage);

	// Assert that multiplication has not created any duplicate entries
	assert(std::set< image_type >(m_image.begin(), m_image.end()).size() == m_image.size());

	reduceImageRepresentation();
}

void ExplicitPermutation::reduceImageRepresentation() {
	assert(!m_image.empty());

	// Shrink transformedImage by all righthand entries that map to themselves
	// (but ensure to keep at least a single entry)
	image_type i;
	for (i = static_cast< image_type >(m_image.size() - 1); i > 0; --i) {
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
