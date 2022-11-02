// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/DiminoAlgorithm.hpp"
#include "libperm/Permutation.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

namespace perm::DiminoAlgorithm {

std::vector< Permutation > generateGroupElements(const std::vector< Permutation > &S) {
	std::vector< Permutation > G;

	if (S.empty()) {
		return G;
	}

	// Start creating the first sub-group of G = < S > by adding all powers of the
	// first generator s. Since we expect to deal with finite groups, the series
	// g = s, s^2, s^3, ... will eventually produce the identity permutation, at which
	// point we have generated all elements of H = < s >.
	// We notice this, when the next power of s is equal to s again.
	const Permutation &s = S[0];
	Permutation g        = s;

	do {
		G.push_back(g);

		g *= s;
	} while (g != s);

	for (std::size_t i = 1; i < S.size(); ++i) {
		extendGroup(G, S, i);
	}

	return G;
}

bool extendGroup(std::vector< Permutation > &H, const std::vector< Permutation > &S, std::size_t i) {
	assert(!H.empty());
	// This function can only be used, if the group has been pre-constructed from at least one generator
	assert(i > 0);
	assert(i < S.size());

	const Permutation &s = S[i];

	if (std::find(H.begin(), H.end(), s) != H.end()) {
		// The next generator is redundant (already contained in group)
		// Therefore, extending the group by it, is a no-op.
		return false;
	}

	// Note: All cosets H x g have the same size, namely the order (size) of H: |H|
	// Therefore, we can determine the coset's size once here at the beginning.
	const std::size_t cosetSize = H.size();

	// Add the coset H x s, where s is the new generator
	// Note: Because cosets are either identical or disjoint, the fact that s
	// is not in the elements of H, tells us that no element of the coset
	// H x s is contained in elements. Therefore, we can add the entire coset
	// at once, without the need to check whether the individual elements might
	// be contained in H already.
	H.reserve(H.size() + cosetSize);
	for (std::size_t k = 0; k < cosetSize; ++k) {
		Permutation hs = H[k];
		hs *= s;
		H.push_back(std::move(hs));
	}

	std::size_t cosetRepresentativePos = cosetSize;

	// Check whether one of the generators of the group will generate a new coset, that is not
	// yet contained in H.
	// If it does, we can immediately add the entire new coset to H.
	// In order to check whether a potential new coset is contained in H, we again only have to
	// check whether once of its coset representatives is contained in H.
	// It can be easily shown that if H x g is a coset of H in G and s_i,g in G, then all elements
	// of (H x g) x s_i lie in the coset of H that has the representative g x s_i.
	// Therefore, if g is a coset representative of the previously added coset and s_i is a generator,
	// then if g x s_i is not yet contained in H, s_i generates a new coset out of the previous one.
	do {
		for (std::size_t k = 0; k <= i; ++k) {
			const Permutation &s_i = S[k];

			Permutation rep = H[cosetRepresentativePos];
			rep *= s_i;

			if (std::find(H.begin(), H.end(), rep) == H.end()) {
				// The found coset representative is not yet contained in the group -> add entire coset
				H.reserve(H.size() + cosetSize);

				for (std::size_t m = 0; m < cosetSize; ++m) {
					Permutation h = H[m];
					h *= rep;
					H.push_back(std::move(h));
				}
			}
		}

		// Move to the next coset representative (if any)
		cosetRepresentativePos += cosetSize;
	} while (cosetRepresentativePos < H.size());

	return true;
}

} // namespace perm::DiminoAlgorithm
