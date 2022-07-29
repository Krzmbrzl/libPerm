// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_CYCLE_HPP_
#define LIBPERM_CYCLE_HPP_

#include <cassert>
#include <numeric>
#include <set>
#include <type_traits>
#include <vector>

namespace perm {

/**
 * Class meant to represent permutations in (disjoint) cycle notation
 */
class Cycle {
public:
	using value_type     = unsigned int;
	using iterator       = std::vector< std::vector< value_type > >::iterator;
	using const_iterator = std::vector< std::vector< value_type > >::const_iterator;

	explicit Cycle(value_type order);
	explicit Cycle(value_type order, const std::vector< value_type > &cycle);
	explicit Cycle(value_type order, std::vector< value_type > &&cycle);
	explicit Cycle(value_type order, const std::vector< std::vector< value_type > > &cycle);
	explicit Cycle(value_type order, std::vector< std::vector< value_type > > &&cycle);
	~Cycle() = default;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	/**
	 * @returns The size n of the set of points {0,...,n-1} that this permutation acts on
	 */
	value_type order() const;
	/**
	 * Computes the order of this permutation
	 */
	value_type computeOrder() const;

	friend bool operator==(const Cycle &lhs, const Cycle &rhs);
	friend bool operator!=(const Cycle &lhs, const Cycle &rhs);

	template< typename image_type > std::vector< image_type > toImage(image_type startValue = 0) const {
		static_assert(std::is_integral_v< image_type >, "Can only create images with integral types");
		assert(m_order >= computeOrder());

		std::vector< image_type > image(order());

		std::iota(image.begin(), image.end(), startValue);

		for (const std::vector< value_type > &currentCycle : m_cycles) {
			if (currentCycle.empty()) {
				continue;
			}

			// Assert that there are no duplicate elements in the cycle
			assert(std::set< value_type >(currentCycle.begin(), currentCycle.end()).size() == currentCycle.size());

			// Save the original value of this before it is potentially modified
			image_type temp = image[currentCycle[0]];

			for (std::size_t i = 0; i < currentCycle.size() - 1; ++i) {
				// Perform element reassignments according to current cycle
				assert(currentCycle[i] >= 0);
				assert(currentCycle[i + 1] >= 0);

				image[currentCycle[i]] = image[currentCycle[i + 1]];
			}

			// Close the cycle by performing the last -> first switch
			image[currentCycle[currentCycle.size() - 1]] = temp;
		}

		return image;
	}

protected:
	std::vector< std::vector< value_type > > m_cycles;
	value_type m_order;
};

} // namespace perm

#endif // LIBPERM_CYCLE_HPP_
