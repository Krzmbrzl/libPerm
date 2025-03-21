// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/Rank.hpp"
#include "libperm/ExplicitPermutation.hpp"
#include "libperm/details/Math.hpp"

#include <cassert>
#include <numeric>
#include <utility>

namespace perm {

// Implementation based on rank1 in doi.org/10.1016/S0020-0190(01)00141-7
std::size_t rank(const AbstractPermutation &perm, std::size_t num_elements) {
	assert(perm.maxElement() <= num_elements);

	if (num_elements <= 1) {
		return 0;
	}

	std::vector< AbstractPermutation::image_type > image(num_elements);
	for (AbstractPermutation::image_type i = 0; i < num_elements; ++i) {
		image[i] = perm.image(i);
	}

	ExplicitPermutation inverse(image);
	inverse.invert();

	decltype(image) inverse_image = inverse.image();

	for (std::size_t i = inverse_image.size(); i < num_elements; ++i) {
		inverse_image.push_back(static_cast< AbstractPermutation::image_type >(i));
	}

	assert(inverse_image.size() == image.size());

	std::size_t rank   = 0;
	std::size_t factor = 1;

	for (std::size_t i = image.size(); i > 1; --i) {
		const std::size_t s = image[i - 1];

		std::swap(image[i - 1], image[inverse_image[i - 1]]);
		std::swap(inverse_image[s], inverse_image[i - 1]);

		rank += factor * s;
		factor *= i;
	}

	return rank;
}


// Implementation based on unrank1 in doi.org/10.1016/S0020-0190(01)00141-7
Permutation unrank(std::size_t rank, std::size_t num_elements) {
	assert(rank <= details::factorial(num_elements));

	// Initialize as identity
	std::vector< AbstractPermutation::image_type > image(num_elements);
	std::iota(image.begin(), image.end(), 0);

	for (std::size_t i = num_elements; i > 0; --i) {
		std::swap(image[i - 1], image[rank % i]);

		rank /= i;
	}

	return ExplicitPermutation(std::move(image));
}

} // namespace perm
