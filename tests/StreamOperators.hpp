// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_TESTS_STREAMOPERATORS_HPP_
#define LIBPERM_TESTS_STREAMOPERATORS_HPP_

#include <iostream>
#include <sstream>
#include <vector>


namespace std {
template< typename T > stringstream &operator<<(stringstream &stream, const vector< T > &vec) {
	static_cast< ostream & >(stream) << vec;
	return stream;
}

template< typename T > ostream &operator<<(ostream &stream, const vector< T > &vec) {
	stream << "{ ";
	for (std::size_t i = 0; i < vec.size(); ++i) {
		stream << vec[i];

		if (i + 1 < vec.size()) {
			stream << ",";
		}

		stream << " ";
	}

	stream << "}";

	return stream;
}
} // namespace std

#endif // LIBPERM_TESTS_STREAMOPERATORS_HPP_
