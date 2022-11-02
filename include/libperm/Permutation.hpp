// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_PERMUTATION_HPP_
#define LIBPERM_PERMUTATION_HPP_

#include "libperm/AbstractPermutation.hpp"
#include "libperm/ExplicitPermutation.hpp"

#include <pv/polymorphic_variant.hpp>

namespace perm {

/**
 * Type-definition for a general permutation object. The actual permutation's implementation could be any of the ones
 * provided in the list given as template arguments (except for the first entry, which only defines the base-class's
 * type). The defined type will expose the same interface as the provided base-class (e.g. as if a base-class pointer
 * would be used (plus a bit more)), but without the need to deal with pointers and/or dynamic memory allocations just
 * to be able to use polymorphism.
 */
using Permutation = pv::polymorphic_variant< AbstractPermutation, ExplicitPermutation >;

} // namespace perm

#endif // LIBPERM_PERMUTATION_HPP_
