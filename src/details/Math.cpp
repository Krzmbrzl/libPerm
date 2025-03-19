// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/details/Math.hpp"

namespace perm::details {

std::size_t factorial(std::size_t n) {
	std::size_t result = 1;

	for (std::size_t i = n; i > 1; --i) {
		result *= i;
	}

	return result;
}

} // namespace perm::details
