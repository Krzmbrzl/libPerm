// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_EXPLICITPERMUTATION_HPP_
#define LIBPERM_EXPLICITPERMUTATION_HPP_

#include "libperm/Cycle.hpp"
#include "libperm/details/SignedPermutation.hpp"

#include <vector>

namespace perm {

class ExplicitPermutation : public details::SignedPermutation {
public:
	/**
	 * Construct an ExplicitPermutation object off the given (disjoint) cycle notation.
	 *
	 * @param cycle The Cycle to construct this perm from
	 * @param sign The sign associated with the to-be-constructed perm
	 */
	static ExplicitPermutation fromCycle(const Cycle &cycle, int sign = 1);

	explicit ExplicitPermutation(std::size_t n, int sign = 1);
	explicit ExplicitPermutation(const std::vector< value_type > &image, int sign = 1);
	explicit ExplicitPermutation(std::vector< value_type > &&image, int sign = 1);
	ExplicitPermutation(const ExplicitPermutation &other) = default;
	ExplicitPermutation(ExplicitPermutation &&other)      = default;
	~ExplicitPermutation();
	ExplicitPermutation &operator=(const ExplicitPermutation &other) = default;

	value_type n() const override;

	value_type image(value_type value) const override;

	/**
	 * @returns The image of the set 0..n-1 under this permutation where n is its size
	 */
	const std::vector< value_type > &image() const;

	void invert() override;

	void multiply(const AbstractPermutation &other) override;

	void insertIntoStream(std::ostream &stream) const override;

	friend ExplicitPermutation operator*(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs);

protected:
	std::vector< value_type > m_image;
};

} // namespace perm

#endif // LIBPERM_EXPLICITPERMUTATION_HPP_
