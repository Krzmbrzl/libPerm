// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_DETAILS_CONTIGUOUSITERATOR_HPP_
#define LIBPERM_DETAILS_CONTIGUOUSITERATOR_HPP_

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace perm {

namespace {
	// implementation details
	template< typename T, bool isConst > struct constifier { using type = T; };
	template< typename T > struct constifier< T, true > { using type = const T; };
} // namespace


/**
 * An iterator that can be used on a set of objects that lie in a contiguous block of memory, but which
 * shall be exposed by a common interface rather than their actual type.
 */
template< typename return_type > class ContiguousIterator {
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type   = std::ptrdiff_t;
	using value_type        = return_type;
	using pointer           = value_type *;
	using reference         = value_type &;

	ContiguousIterator() = default;
	template< typename T >
	ContiguousIterator(T *ptr)
		: m_ptr(reinterpret_cast< typename constifier< std::byte, std::is_const_v< T > >::type * >(ptr)),
		  m_objSize(sizeof(T)) {}

	// Prefix-increment
	ContiguousIterator< return_type > &operator++() {
		m_ptr += m_objSize;
		return *this;
	}

	// Postfix-increment
	ContiguousIterator< return_type > operator++(int) {
		ContiguousIterator< return_type > copy(*this);
		++(*this);
		return copy;
	}

	return_type &operator*() const { return *reinterpret_cast< return_type * >(m_ptr); }

	return_type *operator->() const { return reinterpret_cast< return_type * >(m_ptr); }

	friend bool operator==(const ContiguousIterator< return_type > &lhs, const ContiguousIterator< return_type > &rhs) {
		return lhs.m_ptr == rhs.m_ptr && lhs.m_objSize == rhs.m_objSize;
	}

	friend bool operator!=(const ContiguousIterator< return_type > &lhs, const ContiguousIterator< return_type > &rhs) {
		return !(lhs == rhs);
	}

protected:
	typename constifier< std::byte, std::is_const_v< return_type > >::type *m_ptr = nullptr;
	std::size_t m_objSize                                                         = 0;
};

} // namespace perm

#endif // LIBPERM_DETAILS_CONTIGUOUSITERATOR_HPP_
