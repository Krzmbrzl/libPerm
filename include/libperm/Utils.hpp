// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_UTILS_HPP_
#define LIBPERM_UTILS_HPP_

#include "libperm/AbstractPermutation.hpp"
#include "libperm/Cycle.hpp"
#include "libperm/ExplicitPermutation.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <vector>

namespace perm {

/**
 * Computes the permutation that, applied to the given container, will bring the elements in the provided
 * container into sorted order.
 *
 * @tparam Container The container whose elements shall be considered (must provide random-access to its elements)
 * @tparam Compare The comparator to use in order to determine the sorted order
 * @param container A reference to the respective container
 * @param cmp An instance of the comparator to use
 * @returns An ExplicitPermutation representing the permutation that will bring the container's elements into sorted
 * order
 */
template< typename Container, typename Compare = std::less< typename Container::value_type > >
ExplicitPermutation computeSortPermutation(const Container &container, Compare cmp = {}) {
	static_assert(std::is_same_v< typename std::iterator_traits< typename Container::iterator >::iterator_category,
								  std::random_access_iterator_tag >,
				  "Can only process containers providing random-access to their elements");

	std::vector< ExplicitPermutation::value_type > image(container.size());
	std::iota(image.begin(), image.end(), 0);

	auto begin = container.begin();

	std::sort(image.begin(), image.end(),
			  [&](ExplicitPermutation::value_type lhs, ExplicitPermutation::value_type rhs) {
				  // Note: We access container[lhs] (and container[rhs]) in a funny way, because our static assertion
				  // only guarantees that the container has random-access iterators, but not necessarily an operator[]
				  return cmp(begin[lhs], begin[rhs]);
			  });

	return ExplicitPermutation(image);
}

/**
 * Applies the given permutation to the elements of the provided container.
 *
 * @tparam Container The type of the container to operate on
 * @tparam Permutation The type of permutation that shall act on the container
 * @param container The container whose elements shall be permuted (in-place)
 * @param perm The respective permutation
 */
template< typename Container, typename Permutation >
void applyPermutation(Container &container, const Permutation &perm) {
	static_assert(std::is_base_of_v< AbstractPermutation, Permutation >, "Can only use actual Permutation classes");
	static_assert(std::is_same_v< typename std::iterator_traits< typename Container::iterator >::iterator_category,
								  std::random_access_iterator_tag >,
				  "Can only work with random-access iterators");

	Cycle cycles = perm.toCycle();

	auto begin = container.begin();

	for (const std::vector< Cycle::value_type > &currentCycle : cycles) {
		assert(!currentCycle.empty());
		Cycle::value_type baseIndex = currentCycle[0];

		// We always store the element that has to be moved next in the first element
		// referenced by this cycle
		for (Cycle::value_type currentIndex : currentCycle) {
			assert(baseIndex < container.size());
			assert(currentIndex < container.size());

			std::swap(begin[baseIndex], begin[currentIndex]);
			baseIndex = currentIndex;
		}
	}
}

} // namespace perm

#endif // LIBPERM_UTILS_HPP_
