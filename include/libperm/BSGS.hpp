// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_BSGS_HPP_
#define LIBPERM_BSGS_HPP_

#include "libperm/AbstractPermutationGroup.hpp"
#include "libperm/Permutation.hpp"
#include "libperm/libperm_macros.hpp"

namespace perm {

class LIBPERM_EXPORT BSGS : public AbstractPermutationGroup {
public:
	BSGS();
	BSGS(const std::vector<Permutation> &generators);
};

}

#endif // LIBPERM_BSGS_HPP_
