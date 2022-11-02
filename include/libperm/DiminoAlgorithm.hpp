// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_DOMINOALGORITHM_HPP_
#define LIBPERM_DOMINOALGORITHM_HPP_

#include "libperm/Permutation.hpp"

#include <vector>

namespace perm {

/**
 * This namespace contains functions implementing Dimino's algorithm for explicitly generating the elements
 * of a group, if only its generators are known or to extend a group by adding an additional generator for it.
 *
 * The algorithm is implemented close to how it is described in chapter 3 of the book
 * "Fundamental Algorithms for Permutation Groups" by Gregory Butler (1991), ISBN: 3-540-54955-2
 *
 * Note: This implementation will not take advantage, if the subgroup that is to be extended is normal
 * within its super-group.
 */
namespace DiminoAlgorithm {

	/**
	 * Given a set of generators S, explicitly generate the elements of the group G = < S >.
	 *
	 * @param The list of generators of G
	 * @returns A list of elements of G
	 */
	std::vector< Permutation > generateGroupElements(const std::vector< Permutation > &S);

	/**
	 * Given a subgroup H of a group G (H <= G) and a set of generators S = < S_H, s > such that
	 * G = < S > and H = < S_H >, this function will extend the elements in H to add the
	 * missing elements G - H. In other words: H will be extended such that it will become G.
	 *
	 * @param H A list of elements currently contained in H
	 * @param S The list of generators, such that the new generator s is the last entry in this list
	 * @param i Index of the last entry in the list. This parameter is useful, if you want to successively
	 *     extend H by multiple generators. Then you can already collect all generators in S but always
	 *     point i to the first new generator in the current iteration. Then call this function again
	 *     with i = i + 1, until all generators are accounted for.
	 * @returns Whether the new generator has caused an extension of H. That is: it was not redundant
	 */
	bool extendGroup(std::vector< Permutation > &H, const std::vector< Permutation > &S, std::size_t i);

} // namespace DiminoAlgorithm

} // namespace perm

#endif // LIBPERM_DOMINOALGORITHM_HPP_
