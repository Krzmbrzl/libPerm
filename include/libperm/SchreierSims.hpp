// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_SCHREIERSIMS_HPP_
#define LIBPERM_SCHREIERSIMS_HPP_

#include "libperm/AbstractPermutation.hpp"
#include "libperm/Permutation.hpp"

#include <span>
#include <utility>
#include <vector>

namespace perm {

using OrbitTransversal     = std::vector< std::pair< AbstractPermutation::image_type, Permutation > >;
using StabilizerGenerators = std::vector< Permutation >;

std::pair< OrbitTransversal, StabilizerGenerators >
	orbitTransversalStabilizer(AbstractPermutation::image_type alpha, const std::span< Permutation > &generators);

std::pair< OrbitTransversal, StabilizerGenerators >
	orbitTransversalStabilizer(AbstractPermutation::image_type alpha, const std::span< Permutation > &generators, Permutation identity);

} // namespace perm

#endif // LIBPERM_SCHREIERSIMS_HPP_
