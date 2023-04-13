// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_UTILS_HPP_
#define LIBPERM_UTILS_HPP_

#include "libperm/AbstractPermutation.hpp"
#include "libperm/AbstractPermutationGroup.hpp"
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
 * Computes the permutation that, applied to the given range, will bring the elements in the provided
 * range into sorted order.
 * Given a position i in the provided range, the image of i under the returned permutation describes
 * the location of the ith element in the sorted version of this range.
 *
 * @tparam Iterator The iterator type describing start and end of the range
 * @tparam Compare The comparator to use in order to determine the sorted order
 * @param begin Iterator to the begin of the range
 * @param end Iterator to the end of the range
 * @param cmp An instance of the comparator to use
 * @returns An ExplicitPermutation representing the permutation that will bring the range of elements
 * into sorted order
 */
template< typename Iterator, typename Compare = std::less< typename std::iterator_traits< Iterator >::value_type > >
ExplicitPermutation computeSortPermutation(Iterator begin, Iterator end, Compare cmp = {}) {
	static_assert(
		std::is_same_v< typename std::iterator_traits< Iterator >::iterator_category, std::random_access_iterator_tag >,
		"Can only process random-access iterators");

	assert(std::distance(begin, end) >= 0);
	std::vector< ExplicitPermutation::value_type > image(static_cast< std::size_t >(std::distance(begin, end)));
	std::iota(image.begin(), image.end(), 0);

	std::sort(image.begin(), image.end(),
			  [&](ExplicitPermutation::value_type lhs, ExplicitPermutation::value_type rhs) {
				  assert(lhs < std::distance(begin, end));
				  assert(rhs < std::distance(begin, end));
				  return cmp(begin[lhs], begin[rhs]);
			  });

	return ExplicitPermutation(std::move(image));
}

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
	return computeSortPermutation(container.cbegin(), container.cend(), cmp);
}


/**
 * Applies the given permutation to the elements in the provided range. The permutation
 * is interpreted to describe what element of the original range ends up at a given position.
 * E.g. the image of i under the given permutation describes which of the original elements will
 * be moved to the i-th position in the permuted range.
 *
 * @tparam Iterator The iterator type used to define the range
 * @tparam Permutation The type of permutation that shall act on the container
 * @param begin Iterator marking the beginning of the range
 * @param end Iterator marking the end of the range
 * @param perm The respective permutation
 */
template< typename Iterator, typename Permutation >
void applyPermutation(Iterator begin, Iterator end, const Permutation &perm) {
	if constexpr (!std::is_base_of_v< AbstractPermutation, Permutation >) {
		static_assert(std::is_same_v< Permutation, perm::Permutation >, "Can only use actual Permutation classes");
	}
	static_assert(
		std::is_same_v< typename std::iterator_traits< Iterator >::iterator_category, std::random_access_iterator_tag >,
		"Can only work with random-access iterators");
	static_assert(!std::is_const_v< typename std::iterator_traits< Iterator >::value_type >,
				  "Can't apply permutation to a range of const elements");

	Cycle cycles = [&]() {
		if constexpr (std::is_same_v< Permutation, perm::Permutation >) {
			// When using perm::Permutation, we have to use operator-> to access member functions
			return perm->toCycle();
		} else {
			return perm.toCycle();
		}
	}();

	for (const std::vector< Cycle::value_type > &currentCycle : cycles) {
		assert(!currentCycle.empty());
		Cycle::value_type baseIndex = currentCycle[0];

		// We always store the element that has to be moved next in the first element
		// referenced by this cycle
		for (Cycle::value_type currentIndex : currentCycle) {
			assert(baseIndex < std::distance(begin, end));
			assert(currentIndex < std::distance(begin, end));

			std::swap(begin[baseIndex], begin[currentIndex]);

			baseIndex = currentIndex;
		}
	}

	// Avoid warnings about unused parameters in release builds
	(void) end;
}

/**
 * Applies the given permutation to the elements of the provided container. The permutation
 * is interpreted to describe what element of the unmodified container ends up at a given position.
 * E.g. the image of i under the given permutation describes which of the original elements will
 * be moved to the i-th position in the permuted container.
 *
 * @tparam Container The type of the container to operate on
 * @tparam Permutation The type of permutation that shall act on the container
 * @param container The container whose elements shall be permuted (in-place)
 * @param perm The respective permutation
 */
template< typename Container, typename Permutation >
void applyPermutation(Container &container, const Permutation &perm) {
	applyPermutation(container.begin(), container.end(), perm);
}


/**
 * Computes the permutation that, applied to the provided range, will bring it into a canonical order, that is
 * related to the original order of elements by means of a permutation contained in the provided permutation group.
 *
 * The canonical order is unique, meaning that for every sequence of elements that are interconvertible by an element
 * of the provided group, the returned permutation will bring all of them into the exact same order.
 *
 * @tparam Iterator The iterator type of the iterators describing begin and end of the range
 * @tparam PermGroup The type of the provided permutation group
 * @tparam Compare The type of the used comparator
 * @param begin Iterator to the begin of the range
 * @param end Iterator to the end of the range
 * @param group The permutation group to consider
 * @param cmp The comparator used to determine the reference configuration that is used as a reference point in the
 * computation
 * @returns The computed permutation
 */
template< typename Iterator, typename PermGroup,
		  typename Compare = std::less< typename std::iterator_traits< Iterator >::value_type > >
Permutation computeCanonicalizationPermutation(Iterator begin, Iterator end, const PermGroup &group, Compare cmp = {}) {
	static_assert(std::is_base_of_v< AbstractPermutationGroup, PermGroup >, "Expected a proper permutation group");

	// We require a fix point that serves as an anchor to determine the reference configuration
	// and which can be reached by a known procedure for any given sequence of elements. Sorting the
	// sequence with respect to the provided comparator fulfills this need.
	const ExplicitPermutation sortPermutation = computeSortPermutation(begin, end, cmp);

	// The canonicalization idea is this: Determine a way to permute the standard configuration into the searched-for
	// canonical order of elements for the provided sequence. We can achieve this by considering how to reach the
	// current sequence of elements from the standard configuration
	// -> that's exactly the inverse of the sort permutation
	ExplicitPermutation cosetGenerator = sortPermutation;
	cosetGenerator.invert();

	// This we can then use to generate the left coset with the provided group, which will always be the same
	// for starting configurations that can be transformed into each other using the elements of the provided group
	// (and different in the other case).
	// From this coset, we then select one element deterministically (always the same, for the same coset, regardless
	// of the coset's order)
	Permutation canonicalization = group.rightCosetRepresentative(cosetGenerator);

	// Since we want to apply the canonicalization permutation to the original sequence rather than the standard
	// configuration, we first have to (formally) transform the current sequence into the standard configuration, which
	// we achieve by applying the determined sort permutation as a first step.
	// Note: The order of composition is reversed when applying permutations to sequences and therefore, we have to
	// think right-to-left rather than left-to-right.
	canonicalization->postMultiply(sortPermutation);

	return canonicalization;
}

/**
 * Computes the permutation that, applied to the provided container, will bring it into a canonical order, that is
 * related to the original order of elements by means of a permutation contained in the provided permutation group.
 *
 * The canonical order is unique, meaning that for every sequence of elements that are interconvertible by an element
 * of the provided group, the returned permutation will bring all of them into the exact same order.
 *
 * @tparam Container The type of the container the computed permutation shall act on
 * @tparam PermGroup The type of the provided permutation group
 * @tparam Compare The type of the used comparator
 * @param container The container whose elements shall be considered
 * @param group The permutation group to consider
 * @param cmp The comparator used to determine the reference configuration that is used as a reference point in the
 * computation
 * @returns The computed permutation
 */
template< typename Container, typename PermGroup, typename Compare = std::less< typename Container::value_type > >
Permutation computeCanonicalizationPermutation(const Container &container, const PermGroup &group, Compare cmp = {}) {
	return computeCanonicalizationPermutation(container.cbegin(), container.cend(), group, cmp);
}


/**
 * Brings the elements in the provided range into the canonical order
 *
 * @returns The sign of the permutation that was used to reorder the elements
 *
 * @see computeCanonicalizationPermutation
 */
template< typename Iterator, typename PermGroup,
		  typename Compare = std::less< typename std::iterator_traits< Iterator >::value_type > >
int canonicalize(Iterator begin, Iterator end, const PermGroup &group, Compare cmp = {}) {
	static_assert(!std::is_const_v< typename std::iterator_traits< Iterator >::value_type >,
				  "Can't canonicalize a range of const elements");

	Permutation canonicalizer = computeCanonicalizationPermutation(begin, end, group, cmp);

	applyPermutation(begin, end, canonicalizer);

	return canonicalizer->sign();
}

/**
 * Brings the elements in the provided container into the canonical order
 *
 * @returns The sign of the permutation that was used to reorder the elements
 *
 * @see computeCanonicalizationPermutation
 */
template< typename Container, typename PermGroup, typename Compare = std::less< typename Container::value_type > >
int canonicalize(Container &container, const PermGroup &group, Compare cmp = {}) {
	return canonicalize(container.begin(), container.end(), group, cmp);
}


} // namespace perm

#endif // LIBPERM_UTILS_HPP_
