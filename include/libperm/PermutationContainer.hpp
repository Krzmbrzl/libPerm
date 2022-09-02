// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_PERMUTATION_CONTAINER_HPP_
#define LIBPERM_PERMUTATION_CONTAINER_HPP_

#include "libperm/Permutation.hpp"
#include "libperm/details/ContiguousIterator.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

namespace perm {

/**
 * An abstract interface for a container for Permutation objects. The key property of this container is that
 * it provides a uniform, non-templated interface regardless of which underlying implementation is chosen to
 * represent permutations.
 * It is intended to be used as
 * std::unique_ptr< PermutationContainer > container = std::make_unique< PermutationContainerImpl< ... > >(...);
 */
class PermutationContainer {
public:
	using value_type     = Permutation;
	using iterator       = ContiguousIterator<value_type>;
	using const_iterator = ContiguousIterator<const value_type>;

	virtual iterator begin()              = 0;
	virtual iterator end()                = 0;
	virtual const_iterator begin() const  = 0;
	virtual const_iterator end() const    = 0;
	virtual const_iterator cbegin() const = 0;
	virtual const_iterator cend() const   = 0;

	virtual std::size_t size() const = 0;
	virtual void clear()             = 0;

	virtual value_type &operator[](std::size_t index)             = 0;
	virtual const value_type &operator[](std::size_t index) const = 0;

	virtual value_type &newIdentity(typename value_type::value_type order) = 0;

	bool operator==(const PermutationContainer &other) const {
		return size() == other.size() && std::is_permutation(cbegin(), cend(), other.cbegin());
	}

	bool operator!=(const PermutationContainer &other) const { return !(*this == other); }

	bool empty() const { return size() == 0; }
};

/**
 * Actual, templated implementation of a permutation container. The template parameter is required as we need
 * to know the used permutation type in order to be able to instance and store permutations.
 */
template< typename PermutationImpl > class PermutationContainerImpl : public PermutationContainer {
public:
	using value_type = PermutationImpl;


	PermutationContainerImpl()                                      = default;
	PermutationContainerImpl(const PermutationContainerImpl &other) = default;
	PermutationContainerImpl(PermutationContainerImpl &&other)      = default;
	PermutationContainerImpl &operator=(const PermutationContainerImpl &other) = default;
	PermutationContainerImpl &operator=(PermutationContainerImpl &&other) = default;

	explicit PermutationContainerImpl(const std::vector< value_type > &permutations) : m_permutations(permutations) {}

	explicit PermutationContainerImpl(std::vector< value_type > &&permutations)
		: m_permutations(std::move(permutations)) {}

	template< typename InputIterator >
	explicit PermutationContainerImpl(InputIterator begin, InputIterator end) : m_permutations(begin, end) {}

	explicit PermutationContainerImpl(std::size_t initialSize, typename value_type::value_type order) {
		for (std::size_t i = 0; i < initialSize; ++i) {
			createIdentity(order);
		}
	}


	iterator begin() override { return m_permutations.data(); }
	iterator end() override { return m_permutations.data() + m_permutations.size(); }
	const_iterator begin() const override { return m_permutations.data(); }
	const_iterator end() const override { return m_permutations.data() + m_permutations.size(); }
	const_iterator cbegin() const override { return m_permutations.data(); }
	const_iterator cend() const override { return m_permutations.data() + m_permutations.size(); }

	value_type &operator[](std::size_t index) override {
		assert(index < m_permutations.size());
		return m_permutations[index];
	}

	const value_type &operator[](std::size_t index) const override {
		assert(index < m_permutations.size());
		return m_permutations[index];
	}

	value_type &newIdentity(typename value_type::value_type order) override {
		m_permutations.push_back(value_type(order));

		// Return reference to newly created permutation object
		return m_permutations[m_permutations.size() - 1];
	}

	std::size_t size() const override { return m_permutations.size(); }

	void clear() override { m_permutations.clear(); }

protected:
	std::vector< value_type > m_permutations;
};
} // namespace perm

#endif // LIBPERM_PERMUTATION_CONTAINER_HPP_
