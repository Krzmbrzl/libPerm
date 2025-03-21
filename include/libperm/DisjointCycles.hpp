#ifndef LIBPERM_DISJOINT_CYCLES_HPP
#define LIBPERM_DISJOINT_CYCLES_HPP

#include "libperm/Cycle.hpp"

#include <cassert>
#include <compare>
#include <initializer_list>
#include <iosfwd>
#include <numeric>
#include <ranges>
#include <set>
#include <type_traits>
#include <vector>

namespace perm {

/**
 * Representation of a permutation in terms of disjoint cycles
 */
class DisjointCycles {
private:
	using container_type = std::vector< Cycle >;

public:
	using value_type     = container_type::value_type;
	using iterator       = container_type::iterator;
	using const_iterator = container_type::const_iterator;
	/**
	 * Decomposes the given image of a permutation into disjoint cycles.
	 *
	 * @param N Number of entries the represented permutation acts on. Can be zero to indicate that
	 *          the value shall be deduced to the minimum possible value.
	 *
	 * @return The corresponding disjoint cycle representation
	 */
	template< typename Range >
	static DisjointCycles fromImage(const Range &image, std::size_t N = 0) requires std::ranges::range< Range > {
		using std::ranges::begin;
		using std::ranges::end;

		using image_type = std::remove_cvref_t< decltype(*begin(image)) >;
		static_assert(std::is_integral_v< image_type >, "Expected image to use integral type");

		// Assert that the image point contains all points in [0, n) where n = m_image.size()
		assert(std::accumulate(begin(image), end(image), static_cast< std::size_t >(0))
			   == image.size() * (image.size() - 1) / 2);

		std::set< image_type > visited;
		container_type cycles;

		for (image_type i = 0; i < static_cast< image_type >(image.size()); ++i) {
			if (visited.find(i) != visited.end()) {
				continue;
			}

			visited.insert(i);
			std::vector< Cycle::image_type > currentCycle;
			currentCycle.push_back(static_cast< Cycle::image_type >(i));

			image_type j = image[static_cast< std::size_t >(i)];
			while (j != i) {
				visited.insert(j);
				currentCycle.push_back(static_cast< Cycle::image_type >(j));

				j = image[static_cast< std::size_t >(j)];
			}

			if (currentCycle.size() > 1 || N > 0) {
				cycles.emplace_back(std::move(currentCycle));
			}

			if (visited.size() == image.size()) {
				// All elements have been visited -> we can abort the loop
				break;
			}
		}

		for (std::size_t i = visited.size(); i < N; ++i) {
			// Add missing 1-cycles
			cycles.emplace_back(Cycle({ static_cast< Cycle::image_type >(i) }));
#ifndef NDEBUG
			// To make assertion work
			visited.insert(static_cast< image_type >(i));
#endif
		}

		assert(N == 0 || visited.size() == N);

		return DisjointCycles(std::move(cycles));
	}

	/**
	 * @param image The image of the permutation that shall be decomposed
	 * @param keep1cycles Whether to explicitly keep cycles of size 1
	 *
	 * @see fromImage
	 */
	template< typename Range >
	static DisjointCycles fromImage(const Range &image, bool keep1cycles) requires std::ranges::range< Range > {
		using std::ranges::size;
		return fromImage(image, keep1cycles ? size(image) : 0);
	}

	DisjointCycles() = default;
	// DisjointCycles(std::initializer_list<Cycle::image_type> cycle) : DisjointCycles(Cycle(std::move(cycle))) {}
	DisjointCycles(Cycle cycles);
	DisjointCycles(std::initializer_list< Cycle > cycles) : DisjointCycles(container_type(std::move(cycles))) {}
	template< typename Range > DisjointCycles(Range &&cycles) requires std::ranges::range< Range > {
		if constexpr (std::is_constructible_v< container_type, std::remove_cvref_t< Range > >) {
			m_cycles = container_type(std::forward< Range >(cycles));
		} else {
			using std::ranges::begin;
			using std::ranges::end;
			using std::ranges::size;
			m_cycles.reserve(size(cycles));
			m_cycles.insert(m_cycles.end(), begin(cycles), end(cycles));
		}

		if (!isDisjoint()) {
			splitCycles();
		}

		ensureCanonicalOrder();
		assert(isDisjoint());
	}
	~DisjointCycles() = default;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	std::size_t size() const;

	std::strong_ordering operator<=>(const DisjointCycles &other) const;
	bool operator==(const DisjointCycles &other) const;

	template< typename image_type, typename Container = std::vector< image_type > >
	Container toImage(image_type startValue = 0) const {
		static_assert(std::is_integral_v< image_type >, "Can only create images with integral types");

		Container image(maxElement() + 1);

		std::iota(image.begin(), image.end(), startValue);

		for (const Cycle &currentCycle : m_cycles) {
			if (currentCycle.size() < 1) {
				continue;
			}

			// Assert that there are no duplicate elements in the cycle
			assert(std::set< Cycle::value_type >(currentCycle.begin(), currentCycle.end()).size()
				   == currentCycle.size());

			// Save the original value of this before it is potentially modified
			image_type temp = image[static_cast< std::size_t >(currentCycle[0])];

			for (std::size_t i = 0; i < currentCycle.size() - 1; ++i) {
				// Perform element reassignments according to current cycle

				// Assert that the elements in currentCycle are >= 0. In order to not create a warning that this
				// comparison is always true when using unsigned types, we use the +1 > 0 trick.
				assert(currentCycle[i] + 1 > 0);
				assert(currentCycle[i + 1] + 1 > 0);

				image[static_cast< std::size_t >(currentCycle[i])] =
					image[static_cast< std::size_t >(currentCycle[i + 1])];
			}

			// Close the cycle by performing the last -> first switch
			image[static_cast< std::size_t >(currentCycle[currentCycle.size() - 1])] = temp;
		}

		return image;
	}

	friend std::ostream &operator<<(std::ostream &stream, const DisjointCycles &cycles);

protected:
	container_type m_cycles;

	/**
	 * @returns The maximum element referenced in the represented permutation
	 */
	Cycle::image_type maxElement() const;

	void ensureCanonicalOrder();

	bool isDisjoint() const;

	void splitCycles();
};

} // namespace perm

#endif
