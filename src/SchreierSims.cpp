// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include "libperm/SchreierSims.hpp"
#include "libperm/Permutation.hpp"

#include <algorithm>
#include <cassert>
#include <stack>

namespace perm {

// See also
// - https://blogs.cs.st-andrews.ac.uk/codima/files/2015/11/CoDiMa2015_Holt.pdf
// - https://github.com/koba-e964/rust-schreier-sims/

std::pair< OrbitTransversal, StabilizerGenerators >
	orbitTransversalStabilizer(AbstractPermutation::image_type alpha, const std::span< Permutation > &generators) {
	assert(!generators.empty() && "Empty generator set is invalid (doesn't generate a group)");

	// Construct identity perm of same underlying perm type that rest of generators is using
	// Since we can't know which type that is, we need to construct the identity computationally
	Permutation identity = generators.front();
	identity->invert();
	identity *= generators.front();

	assert(identity->isIdentity());

	return orbitTransversalStabilizer(alpha, generators, std::move(identity));
}

std::pair< OrbitTransversal, StabilizerGenerators >
	orbitTransversalStabilizer(AbstractPermutation::image_type alpha, const std::span< Permutation > &generators,
							   Permutation identity) {
	assert(identity->isIdentity());

	OrbitTransversal transversal;
	StabilizerGenerators stabilizers;

	// TODO: do we even need to start with the identity element or could we simply initialize with first
	// perm in generators?
	transversal.emplace_back(alpha, identity);
	std::stack< OrbitTransversal::value_type > queue;
	queue.emplace(alpha, std::move(identity));

	while (!queue.empty()) {
		const auto [val, perm] = std::move(queue.top());
		queue.pop();

		for (const Permutation &gen : generators) {
			auto image = gen->image(val);
			auto it =
				std::ranges::lower_bound(transversal, image, std::ranges::less{}, &OrbitTransversal::value_type::first);

			if (it == transversal.end() || it->first != image) {
				// image not yet part of orbit transversal -> add it
				it = transversal.insert(it, std::make_pair(image, perm * gen));
				// also add it to queue
				queue.emplace(*it);
			} else {
				// image already part of orbit
				// -> perm * gen * (<perm that generated image before - aka: iter->second>)^(-1) belongs
				//    to stabilizers
				Permutation stab = it->second;
				stab->invert();
				stab->preMultiply(gen);
				stab->preMultiply(perm);
				if (std::ranges::find(stabilizers, stab) == stabilizers.end()) {
					stabilizers.emplace_back(std::move(stab));
				}
			}
		}
	}

	return std::make_pair(std::move(transversal), std::move(stabilizers));
}

} // namespace perm
