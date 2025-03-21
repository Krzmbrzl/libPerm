// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_CYCLE_HPP_
#define LIBPERM_CYCLE_HPP_

#include <cassert>
#include <compare>
#include <initializer_list>
#include <iosfwd>
#include <ranges>
#include <type_traits>
#include <vector>

namespace perm {

/**
 * An individual cycle
 */
class Cycle {
public:
	using image_type = unsigned int;

private:
	using container_type = std::vector< image_type >;

public:
	using value_type     = container_type::value_type;
	using iterator       = container_type::iterator;
	using const_iterator = container_type::const_iterator;

	Cycle();
	Cycle(std::initializer_list< image_type > values) : Cycle(container_type(std::move(values))) {}
	template< typename Range > Cycle(Range &&rng) requires std::ranges::range< Range > {
		if constexpr (std::is_constructible_v< container_type, std::remove_cvref_t< Range > >) {
			m_cycle = container_type(std::forward< Range >(rng));
		} else {
			using std::ranges::begin;
			using std::ranges::end;
			using std::ranges::size;
			m_cycle.reserve(size(rng));
			m_cycle.insert(m_cycle.end(), begin(rng), end(rng));
		}

		assert(!m_cycle.empty());

		ensureCanonicalOrder();
	}

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	std::size_t size() const;

	image_type at(std::size_t pos) const;

	image_type operator[](std::size_t pos) const;
	std::strong_ordering operator<=>(const Cycle &other) const = default;
	bool operator==(const Cycle &other) const                  = default;

	friend std::ostream &operator<<(std::ostream &stream, const Cycle &cycle);

private:
	container_type m_cycle;

	void ensureCanonicalOrder();
};


} // namespace perm

#endif // LIBPERM_CYCLE_HPP_
