// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_PERMUTATION_HPP_
#define LIBPERM_PERMUTATION_HPP_

#include <ostream>
#include <string>
#include <vector>

namespace perm {

/**
 * Abstract base class for everything that represents a permutation. This class is meant to be used as a
 * general Permutation interface.
 */
class Permutation {
public:
	/**
	 * The data type of the values of the set that this permutation shall work on
	 */
	using value_type = unsigned int;

	/**
	 * @returns The image of the given value under the operation of this permutation
	 */
	value_type operator[](value_type value) const;

	/**
	 * @returns Whether lhs and rhs are considered to be equal
	 */
	friend bool operator==(const Permutation &lhs, const Permutation &rhs);
	/**
	 * @returns Whether lhs and rhs are considered to be unequal
	 */
	friend bool operator!=(const Permutation &lhs, const Permutation &rhs);

	/**
	 * Multiplies the lhs permutation with rhs and modifies lhs in-place.
	 *
	 * @returns The modified lhs
	 */
	friend Permutation &operator*=(Permutation &lhs, const Permutation &rhs);

	/**
	 * Inserts a string representation of this permutation into the given stream
	 */
	friend std::ostream &operator<<(std::ostream &stream, const Permutation &perm);


	/**
	 * @returns The order n of the set {0,...,n-1} this permutation is working on
	 */
	virtual value_type order() const = 0;

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
	 * Multiplies this permutation by other, modifying this permutation in-place
	 */
	virtual void multiply(const Permutation &other) = 0;

	/**
	 * @returns Whether this permutation is considered equal to other
	 */
	virtual bool equals(const Permutation &other) const;

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

#endif // LIBPERM_PERMUTATION_HPP_
