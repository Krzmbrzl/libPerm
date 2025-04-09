// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_RANK_HPP_
#define LIBPERM_RANK_HPP_

#include "libperm/AbstractPermutation.hpp"
#include "libperm/Permutation.hpp"
#include "libperm/libperm_macros.hpp"

namespace perm {

/**
 * Computes the rank of the given permutation. The rank of a permutation is its index
 * in the list of all permutations of num_elements. Note that the exact order of this
 * list of all permutations is undefined. The only guarantee is that for every permutation
 * in S(N), this function will compute a unique rank that lies in the range [0, N! - 1].
 *
 * @param perm The permutation whose rank shall be determined
 * @param num_elements The number of elements the given permutation shall act on
 *
 * @see unrank
 */
LIBPERM_EXPORT std::size_t rank(const AbstractPermutation &perm, std::size_t num_elements);

/**
 * Given the rank of a permutation as well as the number of elements it is supposed to act
 * on, reconstructs the corresponding permutation. That means, this function performs
 * the inverse operation to the rank function.
 *
 * @param rank The rank of the desired permutation
 * @param num_elements The number of elements the permutation shall act on
 *
 * @see rank
 */
LIBPERM_EXPORT Permutation unrank(std::size_t rank, std::size_t num_elements);

} // namespace perm

#endif
