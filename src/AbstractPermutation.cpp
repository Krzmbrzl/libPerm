// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.


#include "AbstractPermutation.hpp"

#include <sstream>

namespace perm {

AbstractPermutation::value_type AbstractPermutation::operator[](value_type value) const {
	return image(value);
}

bool operator==(const AbstractPermutation &lhs, const AbstractPermutation &rhs) {
	return lhs.equals(rhs);
}

bool operator!=(const AbstractPermutation &lhs, const AbstractPermutation &rhs) {
	return !(lhs == rhs);
}

AbstractPermutation &operator*=(AbstractPermutation &lhs, const AbstractPermutation &rhs) {
	lhs.multiply(rhs);

	return lhs;
}

std::ostream &operator<<(std::ostream &stream, const AbstractPermutation &perm) {
	perm.insertIntoStream(stream);

	return stream;
}

bool AbstractPermutation::isIdentity() const {
	// Check whether every point i in the set 0..n-1 is mapped to itself
	for (value_type i = 0; i < n(); ++i) {
		if (image(i) != i) {
			return false;
		}
	}

	return true;
}

bool AbstractPermutation::equals(const AbstractPermutation &other) const {
	if (n() != other.n()) {
		return false;
	}

	for (value_type i = 0; i < n(); ++i) {
		if (image(i) != other.image(i)) {
			return false;
		}
	}

	return true;
}

std::string AbstractPermutation::toString() const {
	std::stringstream sstream;

	insertIntoStream(sstream);

	return sstream.str();
}

} // namespace perm
