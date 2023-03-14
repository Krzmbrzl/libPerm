// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_UTILS_HPP_
#define LIBPERM_UTILS_HPP_

#include "libperm/AbstractPermutation.hpp"
#include "libperm/Cycle.hpp"
#include "libperm/ExplicitPermutation.hpp"
#include "libperm/Permutation.hpp"

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
 * Given a position i in the provided container, the image of i under the returned permutation describes
 * the location of the ith element in the sorted version of this container.
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

	// The image that we have created above describes what element of the original container have to appear at a given
	// position in the sorted container. However, by definition, our permutation is supposed to indicate to which
	// position in the sorted container, a given entry in the original container has to go to.
	// That's exactly the inverse information.
	ExplicitPermutation p(std::move(image));
	p.invert();
	// Inversion is likely to change the sign, which doesn't make sense for our case
	p.setSign(1);
	return p;
}

/**
 * Applies the given permutation to the elements of the provided container. The permutation
 * is interpreted to describe where a given element in the container shall end up at.
 * E.g. the image of i under the given permutation describes to which location the ith
 * element in the container shall be moved to.
 *
 * @tparam Container The type of the container to operate on
 * @tparam Permutation The type of permutation that shall act on the container
 * @param container The container whose elements shall be permuted (in-place)
 * @param perm The respective permutation
 */
template< typename Container, typename Permutation >
void applyPermutation(Container &container, const Permutation &perm) {
	if constexpr (!std::is_base_of_v< AbstractPermutation, Permutation >) {
		static_assert(std::is_same_v< Permutation, perm::Permutation >, "Can only use actual Permutation classes");
	}
	static_assert(std::is_same_v< typename std::iterator_traits< typename Container::iterator >::iterator_category,
								  std::random_access_iterator_tag >,
				  "Can only work with random-access iterators");

	Cycle cycles = [&]() {
		if constexpr (std::is_same_v< Permutation, perm::Permutation >) {
			// When using perm::Permutation, we have to use operator-> to access member functions
			return perm->toCycle();
		} else {
			return perm.toCycle();
		}
	}();

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
		}
	}
}

} // namespace perm

#endif // LIBPERM_UTILS_HPP_
