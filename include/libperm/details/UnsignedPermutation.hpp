// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_DETAILS_UNSIGNEDPERMUTATION_HPP_
#define LIBPERM_DETAILS_UNSIGNEDPERMUTATION_HPP_

#include "libperm/AbstractPermutation.hpp"

#include <cassert>

namespace perm::details {

/**
 * Abstract base class for all permutation objects that don't want to make use of the possibility
 * to explicitly carry a sign property.
 * Children of this class will always report to have a sign of +1.
 */
class UnsignedPermutation : public AbstractPermutation {
public:
	static constexpr const bool is_signed = false;

	UnsignedPermutation()  = default;
	~UnsignedPermutation() = default;

	int sign() const override final { return 1; }

	void setSign(int sign) override final { assert(sign > 0); };
};

} // namespace perm::details

#endif // LIBPERM_DETAILS_UNSIGNEDPERMUTATION_HPP_
