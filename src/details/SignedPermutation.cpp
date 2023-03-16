// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/details/SignedPermutation.hpp"

#include <cassert>

namespace perm::details {

SignedPermutation::SignedPermutation(int sign) : AbstractPermutation(), m_negative(sign < 0) {
	assert(sign == -1 || sign == 1);
}

int SignedPermutation::sign() const {
	// As per C++-Standard bool-to-int conversion is guaranteed to either result in 0 (false) or 1 (true)
	// Branchless version of return m_negative ? -1 : 1;
	return 1 - static_cast< int >(m_negative) * 2;
}

void SignedPermutation::setSign(int sign) {
	m_negative = sign < 0;
}

void SignedPermutation::invert(bool invertSign) {
	if (invertSign) {
		m_negative = !m_negative;
	}
}

void SignedPermutation::preMultiply(const AbstractPermutation &other) {
	// As far as the sign is concerned, there is no difference between pre and post multiplication
	SignedPermutation::postMultiply(other);
}

void SignedPermutation::postMultiply(const AbstractPermutation &other) {
	// XOR to mimick a sign extraction for the multiplication of signed integers
	m_negative = (m_negative ^ (other.sign() < 0));
}

} // namespace perm::details
