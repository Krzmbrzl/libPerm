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
	 * @returns A list of generators for this group. Note that there is no guarantee that these are the same generators
	 * that were used to construct this object with (the implementation is free to choose them differently as long as
	 * the generated group stays the same)
	 */
	virtual const std::vector< Permutation > &getGenerators() const = 0;

	/**
	 * Explicitly obtains all elements contained in this group. Note that depending on the group, this could
	 * be a very large amount.
	 *
	 * @param permutations The vector to write the permutations of this group into
	 */
	virtual void getElementsTo(std::vector< Permutation > &permutations) const = 0;

	/**
	 * Calculates the canonical coset representative of the right coset H * g, defined as
	 * { h * g | h in H}. "Canonical" means that regardless of how the respective coset is generated
	 * (that is: regardless of which exact g is used), the returned element is guaranteed to always
	 * be the same, as long as the generated coset is the same.
	 * The group H is the group represented by this object.
	 *
	 * @param perm The permutation g to use to generate the coset.
	 * @returns The canonical coset representative
	 */
	virtual Permutation getCanonicalCosetRepresentative(const AbstractPermutation &perm) const = 0;

	/**
	 * Same as getCanonicalCosetRepresentative(const Permutation &perm), but the element g used to generate
	 * the coset is the identity permutation. Thus, the returned coset representative is also the
	 * canonical representative of this group.
	 */
	virtual const Permutation &getCanonicalCosetRepresentative() const = 0;

	/**
	 * @returns Whether the two given groups are equal
	 */
	friend bool operator==(const AbstractPermutationGroup &lhs, const AbstractPermutationGroup &rhs);

	/**
	 * @returns Whether the two given groups are unequal
	 */
	friend bool operator!=(const AbstractPermutationGroup &lhs, const AbstractPermutationGroup &rhs);

protected:
	PermutationGroupType m_type;
};

} // namespace perm

#endif // LIBPERM_ABSTRACTPERMUTATIONGROUP_HPP_
