// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_DETAILS_PERMUTATION_HPP_
#define LIBPERM_DETAILS_PERMUTATION_HPP_

#include "libperm/Cycle.hpp"
#include "libperm/Permutation.hpp"

#include <vector>

namespace perm {

class ExplicitPermutation : public Permutation {
public:
	/**
	 * Construct an ExplicitPermutation object off the given (disjoint) cycle notation (1-based).
	 */
	static ExplicitPermutation fromCycle(const Cycle &cycle);

	explicit ExplicitPermutation(std::size_t n);
	explicit ExplicitPermutation(const std::vector< Permutation::value_type > &image);
	explicit ExplicitPermutation(std::vector< Permutation::value_type > &&image);
	explicit ExplicitPermutation(const ExplicitPermutation &other) = default;
	ExplicitPermutation(ExplicitPermutation &&other)               = default;
	~ExplicitPermutation();
	ExplicitPermutation &operator=(const ExplicitPermutation &other) = default;

	Permutation::value_type order() const override;

	Permutation::value_type image(Permutation::value_type value) const override;

	/**
	 * @returns The image of the set 0..n-1 under this permutation where n is its size
	 */
	const std::vector< Permutation::value_type > &image() const;

	void invert() override;

	void multiply(const Permutation &other) override;

	void insertIntoStream(std::ostream &stream) const override;

	friend ExplicitPermutation operator*(const ExplicitPermutation &lhs, const ExplicitPermutation &rhs);

protected:
	std::vector< Permutation::value_type > m_image;
};

} // namespace perm

#endif // LIBPERM_DETAILS_PERMUTATION_HPP_
