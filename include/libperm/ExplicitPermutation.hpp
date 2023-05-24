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
	[[deprecated("Prefer using the corresponding constructor directly")]] static ExplicitPermutation
		fromCycle(const Cycle &cycle, int sign = 1);

	explicit ExplicitPermutation(int sign = 1);
	explicit ExplicitPermutation(std::vector< value_type > image, int sign = 1);
	ExplicitPermutation(const Cycle &cycle, int sign = 1);
	ExplicitPermutation(const ExplicitPermutation &other) = default;
	ExplicitPermutation(ExplicitPermutation &&other)      = default;
	~ExplicitPermutation();
	ExplicitPermutation &operator=(const ExplicitPermutation &other) = default;

	value_type maxElement() const override;

	value_type image(value_type value) const override;

	/**
	 * @returns The image of the set 0..n under this permutation where n is the largest number that this permutation
	 * actually permutes.
	 */
	const std::vector< value_type > &image() const;

	void invert() override;

	void preMultiply(const AbstractPermutation &other) override;

	void postMultiply(const AbstractPermutation &other) override;

	Cycle toCycle() const override;

	void shift(int shift) override;

	void insertIntoStream(std::ostream &stream) const override;

	friend ExplicitPermutation operator*(const ExplicitPermutation &lhs, const AbstractPermutation &rhs);
	friend ExplicitPermutation operator*(const AbstractPermutation &lhs, const ExplicitPermutation &rhs);
	friend ExplicitPermutation operator*(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs);

protected:
	std::vector< value_type > m_image;

	/**
	 * Multiplies this perm by itself
	 */
	void selfMultiply();

	/**
	 * Removes redundant entries from the currently stored image. Entries in the image are
	 * redundant, if they just map a number to itself and this mapping happens at the end
	 * of the image. Thus, in {0, 2, 1, 3}, only 3 is redundant, even though 0 also maps to
	 * itself. The reason is that we always want to retain an image representation from 0
	 * to some highest number that is still permuted.
	 * The zero entry itself is never redundant, even in  { 0 }. That is: we always retain
	 * at least a single entry.
	 */
	void reduceImageRepresentation();
};

} // namespace perm

#endif // LIBPERM_EXPLICITPERMUTATION_HPP_
