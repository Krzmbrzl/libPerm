// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "details/ExplicitPermutation.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>

namespace perm {

ExplicitPermutation::ExplicitPermutation(std::size_t n) : m_image(n), m_sign(true) {
	std::iota(m_image.begin(), m_image.end(), 0);
}

ExplicitPermutation::ExplicitPermutation(const std::vector< ExplicitPermutation::image_type > &image, int sign)
	: m_image(image), m_sign(sign > 0) {
	// Assert that the image point contains all points in [0, n) where n = m_image.size()
	assert(std::accumulate(m_image.begin(), m_image.end(), static_cast< std::size_t >(0))
		   == m_image.size() * (m_image.size() - 1) / 2);
}

ExplicitPermutation::ExplicitPermutation(std::vector< ExplicitPermutation::image_type > &&image, int sign)
	: m_image(std::move(image)), m_sign(sign > 0) {
	// Assert that the image point contains all points in [0, n) where n = m_image.size()
	assert(std::accumulate(m_image.begin(), m_image.end(), static_cast< std::size_t >(0))
		   == m_image.size() * (m_image.size() - 1) / 2);
}

ExplicitPermutation::~ExplicitPermutation() {
}

std::size_t ExplicitPermutation::size() const {
	return m_image.size();
}

int ExplicitPermutation::sign() const {
	// As per C++-Standard bool-to-int conversion is guaranteed to either result in 0 (false) or 1 (true)
	assert(m_sign == 0 || m_sign == 1);
	// Branchless version of return m_sign ? 1 : -1;
	return -1 + m_sign * 2;
}

ExplicitPermutation::iterator ExplicitPermutation::begin() {
	return &m_image[0];
}

ExplicitPermutation::iterator ExplicitPermutation::end() {
	return begin() + m_image.size();
}

ExplicitPermutation::const_iterator ExplicitPermutation::begin() const {
	return &m_image[0];
}

ExplicitPermutation::const_iterator ExplicitPermutation::end() const {
	return begin() + m_image.size();
}

ExplicitPermutation::const_iterator ExplicitPermutation::cbegin() const {
	return begin();
}

ExplicitPermutation::const_iterator ExplicitPermutation::cend() const {
	return end();
}

ExplicitPermutation::image_type &ExplicitPermutation::operator[](std::size_t index) {
	assert(index < m_image.size());
	return m_image[index];
}

const ExplicitPermutation::image_type &ExplicitPermutation::operator[](std::size_t index) const {
	assert(index < m_image.size());
	return m_image[index];
}

const std::vector< ExplicitPermutation::image_type > &ExplicitPermutation::image() const {
	return m_image;
}

ExplicitPermutation &operator*=(ExplicitPermutation &lhs, const ExplicitPermutation &rhs) {
	assert(lhs.size() == rhs.size());

	for (std::size_t i = 0; i < lhs.size(); ++i) {
		lhs[i] = rhs[lhs[i]];
	}

	// Negated XOR to mimick a sign extraction for the multiplication of signed integers
	lhs.m_sign = !(lhs.m_sign ^ rhs.m_sign);

	return lhs;
}

ExplicitPermutation operator*(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs) {
	ExplicitPermutation result(lhs);

	result *= rhs;

	return result;
}

bool operator==(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs) {
	return lhs.m_sign == rhs.m_sign && lhs.m_image == rhs.m_image;
}

bool operator!=(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs) {
	return !(lhs == rhs);
}

ExplicitPermutation ExplicitPermutation::fromCycle(const std::vector< std::vector< ExplicitPermutation::image_type >> &cycles) {
	// TODO: Use Cycle class & implement
}

} // namespace perm
