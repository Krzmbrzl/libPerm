// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_ABSTRACTPERMUTATIONGROUP_HPP_
#define LIBPERM_ABSTRACTPERMUTATIONGROUP_HPP_

#include "libperm/AbstractPermutation.hpp"
#include "libperm/Permutation.hpp"

#include <vector>

namespace perm {

enum class PermutationGroupType {
	Primitive,
};

/**
 * Class describing the general interface of a permutation group.
 */
class AbstractPermutationGroup {
public:
	constexpr AbstractPermutationGroup(PermutationGroupType type) : m_type(type) {}
	virtual ~AbstractPermutationGroup() = default;

	constexpr PermutationGroupType type() const { return m_type; }

	/**
	 * Computes the orbit of the given point under the action of this group's elements
	 *
	 * @param point The point to calculate the orbit of
	 * @returns The computed orbit
	 */
	virtual std::vector< AbstractPermutation::value_type > orbit(AbstractPermutation::value_type point) const = 0;

	/**
	 * @returns The order of this group (the amount of elements in it)
	 */
	virtual std::size_t order() const = 0;

	/**
	 * @returns Whether the given permutation is contained within this group
	 */
	virtual bool contains(const AbstractPermutation &perm) const = 0;

	/**
	 * Adds the given permutation as a generator to this group (if it is not already contained). Note, that
	 * adding a generator might be expensive (as the group representation might have to be regenerated).
	 *
	 * @returns Whether the passed permutation has actually been added as a generator
	 */
	virtual bool addGenerator(Permutation perm) = 0;

	/**
	 * Sets the generators for this group. This will cause the group's representation to be regenerated, which might be
	 * expensive.
	 */
	virtual void setGenerators(std::vector< Permutation > generators) = 0;

	/**
	 * Explicitly obtains all elements contained in this group. Note that depending on the group, this could
	 * be a very large amount.
	 *
	 * @param permutations The vector to write the permutations of this group into
	 */
	virtual void getElementsTo(std::vector< Permutation > &permutations) const = 0;

protected:
	PermutationGroupType m_type;
};

} // namespace perm

#endif // LIBPERM_ABSTRACTPERMUTATIONGROUP_HPP_
