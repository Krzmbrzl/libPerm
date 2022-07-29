// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.


#include "Permutation.hpp"

#include <sstream>

namespace perm {

Permutation::value_type Permutation::operator[](value_type value) const {
	return image(value);
}

bool operator==(const Permutation &lhs, const Permutation &rhs) {
	return lhs.equals(rhs);
}

bool operator!=(const Permutation &lhs, const Permutation &rhs) {
	return !(lhs == rhs);
}

Permutation &operator*=(Permutation &lhs, const Permutation &rhs) {
	lhs.multiply(rhs);

	return lhs;
}

std::ostream &operator<<(std::ostream &stream, const Permutation &perm) {
	perm.insertIntoStream(stream);

	return stream;
}

bool Permutation::isIdentity() const {
	// Check whether every point i in the set 0..n-1 is mapped to itself
	for (value_type i = 0; i < order(); ++i) {
		if (image(i) != i) {
			return false;
		}
	}

	return true;
}

bool Permutation::equals(const Permutation &other) const {
	if (order() != other.order()) {
		return false;
	}

	for (value_type i = 0; i < order(); ++i) {
		if (image(i) != other.image(i)) {
			return false;
		}
	}

	return true;
}

std::string Permutation::toString() const {
	std::stringstream sstream;

	insertIntoStream(sstream);

	return sstream.str();
}

} // namespace perm
