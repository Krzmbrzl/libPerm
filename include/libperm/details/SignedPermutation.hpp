// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_DETAILS_SIGNEDPERMUTATION_HPP_
#define LIBPERM_DETAILS_SIGNEDPERMUTATION_HPP_

#include "libperm/AbstractPermutation.hpp"

namespace perm::details {

/**
 * Abstract base class for all permutation objects that want to make use of the possibility to explicitly
 * carry a sign property.
 * The sign handling is completely encapsulated in this class and will apply automatically, as long as
 * child classes will call the parent implementations before adding their own functionality for overridden
 * functions.
 */
class SignedPermutation : public AbstractPermutation {
public:
	static constexpr const bool is_signed = true;

	SignedPermutation(int sign = +1);
	~SignedPermutation() = default;

	int sign() const override final;

	void setSign(int sign) override final;

	void invert(bool invertSign = true) override;

	void multiply(const AbstractPermutation &other) override;

protected:
	bool m_negative = false;
};

} // namespace perm::details

#endif // LIBPERM_DETAILS_SIGNEDPERMUTATION_HPP_
