// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_ABSTRACTPERMUTATION_HPP_
#define LIBPERM_ABSTRACTPERMUTATION_HPP_

#include <ostream>
#include <string>
#include <vector>

namespace perm {

/**
 * Abstract base class for everything that represents a permutation. This class is meant to be used as a
 * general Permutation interface.
 * Technically, this interface goes beyond a simple permutation, but can actually be used to represent
 * elements of a group that is the result of the direct product of a regular permutation group and {-1, +1}.
 * That means, that permutations represented through this interface also have a sign associated with them
 * that can be read out and manipulated as an orthogonal property to the standard permutation properties.
 */
class AbstractPermutation {
public:
	/**
	 * The data type of the values of the set that this permutation shall work on
	 */
	using value_type = unsigned int;

	AbstractPermutation()          = default;
	virtual ~AbstractPermutation() = default;

	/**
	 * @returns The image of the given value under the operation of this permutation
	 */
	value_type operator[](value_type value) const;

	/**
	 * @returns Whether lhs and rhs are considered to be equal
	 */
	friend bool operator==(const AbstractPermutation &lhs, const AbstractPermutation &rhs);
	/**
	 * @returns Whether lhs and rhs are considered to be unequal
	 */
	friend bool operator!=(const AbstractPermutation &lhs, const AbstractPermutation &rhs);

	/**
	 * Multiplies the lhs permutation with rhs and modifies lhs in-place.
	 *
	 * @returns The modified lhs
	 */
	friend AbstractPermutation &operator*=(AbstractPermutation &lhs, const AbstractPermutation &rhs);

	/**
	 * Inserts a string representation of this permutation into the given stream
	 */
	friend std::ostream &operator<<(std::ostream &stream, const AbstractPermutation &perm);

	/**
	 * @returns The biggest element that this permutation acts on
	 */
	virtual value_type maxElement() const = 0;

	/**
	 * @returns The image of the given value under the operation of this permutation
	 */
	virtual value_type image(value_type value) const = 0;

	/**
	 * @returns Whether this permutation represents the identity permutation
	 */
	virtual bool isIdentity() const;

	/*
	 * Inverts this permutation in-place
	 */
	virtual void invert() = 0;

	/**
	 * Gets the sign associated with this permutation.
	 *
	 * @returns Either -1 or +1
	 */
	virtual int sign() const = 0;

	/**
	 * Sets the sign for this permutation
	 *
	 * @param sign The sign to use. Must be either -1 or +1
	 */
	virtual void setSign(int sign) = 0;

	/**
	 * Multiplies this permutation by other, modifying this permutation in-place
	 */
	virtual void multiply(const AbstractPermutation &other) = 0;

	/**
	 * @returns Whether this permutation is considered equal to other
	 */
	virtual bool equals(const AbstractPermutation &other) const;

	/**
	 * @returns A string representation of this permutation
	 */
	virtual std::string toString() const;

	/**
	 * Inserts a string representation of this permutation into the given stream object.
	 * The implementation of this function determines the object's string representation.
	 */
	virtual void insertIntoStream(std::ostream &stream) const = 0;
};

} // namespace perm

#endif // LIBPERM_ABSTRACTPERMUTATION_HPP_