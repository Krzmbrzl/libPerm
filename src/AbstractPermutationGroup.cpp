// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/AbstractPermutationGroup.hpp"

namespace perm {

bool operator==(const AbstractPermutationGroup &lhs, const AbstractPermutationGroup &rhs) {
	if (lhs.order() != rhs.order()) {
		return false;
	}

	for (const Permutation &current : lhs.getGenerators()) {
		if (!rhs.contains(current)) {
			return false;
		}
	}

	// By now we know that lhs is a subgroup of rhs

	for (const Permutation &current : rhs.getGenerators()) {
		if (!lhs.contains(current)) {
			return false;
		}
	}

	// This proofs that rhs is also a subgroup of lhs. With the above, this means
	// that the groups must be equal

	return true;
}

bool operator!=(const AbstractPermutationGroup &lhs, const AbstractPermutationGroup &rhs) {
	return !(lhs == rhs);
}

} // namespace perm
