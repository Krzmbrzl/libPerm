// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_CYCLE_HPP_
#define LIBPERM_CYCLE_HPP_

#include <algorithm>
#include <cassert>
#include <numeric>
#include <ostream>
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

	/**
	 * Decomposes the permutation given into disjoint cycles.
	 *
	 * @return The corresponding disjoint cycle representation
	 */
	template< typename image_type > static Cycle fromImage(const std::vector< image_type > &image) {
		static_assert(std::is_integral_v< image_type >, "Expected image to use integral type");
		// Assert that the image point contains all points in [0, n) where n = m_image.size()
		assert(std::accumulate(image.begin(), image.end(), static_cast< std::size_t >(0))
			   == image.size() * (image.size() - 1) / 2);

		std::set< image_type > visited;
		std::vector< std::vector< Cycle::value_type > > cycles;

		for (image_type i = 0; i < image.size(); ++i) {
			if (visited.find(i) != visited.end()) {
				continue;
			}

			visited.insert(i);
			std::vector< Cycle::value_type > currentCycle;
			currentCycle.push_back(static_cast< Cycle::value_type >(i));

			image_type j = image[i];
			while (j != i) {
				visited.insert(j);
				currentCycle.push_back(j);

				j = image[j];
			}

			if (currentCycle.size() > 1) {
				cycles.push_back(std::move(currentCycle));
			}

			if (visited.size() == image.size()) {
				// All elements have been visited -> we can abort the loop
				break;
			}
		}

		return Cycle(std::move(cycles));
	}

	explicit Cycle() = default;
	explicit Cycle(std::vector< value_type > cycle);
	explicit Cycle(std::vector< std::vector< value_type > > cycle);
	~Cycle() = default;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	friend bool operator==(const Cycle &lhs, const Cycle &rhs);
	friend bool operator!=(const Cycle &lhs, const Cycle &rhs);

	template< typename image_type > std::vector< image_type > toImage(image_type startValue = 0) const {
		static_assert(std::is_integral_v< image_type >, "Can only create images with integral types");

		std::vector< image_type > image(maxElement() + 1);

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

				// Assert that the elements in currentCycle are >= 0. In order to not create a warning that this
				// comparison is always true when using unsigned types, we use the +1 > 0 trick.
				assert(currentCycle[i] + 1 > 0);
				assert(currentCycle[i + 1] + 1 > 0);

				image[currentCycle[i]] = image[currentCycle[i + 1]];
			}

			// Close the cycle by performing the last -> first switch
			image[currentCycle[currentCycle.size() - 1]] = temp;
		}

		return image;
	}

	friend std::ostream &operator<<(std::ostream &stream, const Cycle &cycle);

protected:
	std::vector< std::vector< value_type > > m_cycles;

	/**
	 * @returns The maximum element referenced in this Cycle
	 */
	value_type maxElement() const;
};

} // namespace perm

#endif // LIBPERM_CYCLE_HPP_
