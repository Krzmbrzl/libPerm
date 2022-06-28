// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_DETAILS_PERMUTATION_HPP_
#define LIBPERM_DETAILS_PERMUTATION_HPP_

#include <vector>

namespace perm {

class ExplicitPermutation {
public:
	using image_type     = unsigned int;
	using iterator       = image_type *;
	using const_iterator = const image_type *;

	explicit ExplicitPermutation(std::size_t n);
	explicit  ExplicitPermutation(const std::vector< image_type > &image, int sign = 1);
	explicit ExplicitPermutation(std::vector< image_type > &&image, int sign = 1);
	explicit ExplicitPermutation(const ExplicitPermutation &other) = default;
	ExplicitPermutation(ExplicitPermutation &&other)               = default;
	~ExplicitPermutation();

	std::size_t size() const;

	int sign() const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	image_type &operator[](std::size_t index);
	const image_type &operator[](std::size_t index) const;

	const std::vector< image_type > &image() const;

	bool isIdentity() const;

	friend ExplicitPermutation &operator*=(ExplicitPermutation &lhs, const ExplicitPermutation &rhs);
	friend ExplicitPermutation operator*(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs);

	friend bool operator==(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs);
	friend bool operator!=(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs);

	ExplicitPermutation &operator=(const ExplicitPermutation &other) = default;

	/**
	 * Construct an ExplicitPermutation object off the given (disjoint) cycle notation (1-based).
	 */
	static ExplicitPermutation fromCycle(const std::vector< std::vector< image_type > > &cycles);

protected:
	std::vector< image_type > m_image;
	// True == +1, False == -1
	bool m_sign;
};

} // namespace perm

#endif // LIBPERM_DETAILS_PERMUTATION_HPP_
