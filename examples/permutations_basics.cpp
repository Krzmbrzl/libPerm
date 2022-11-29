// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/Cycle.hpp>
#include <libperm/ExplicitPermutation.hpp>

#include <iostream>

int main() {
	/*
	 * IMPORTANT: libPerm uses zero-based number systems throughout.
	 */

	std::cout << std::boolalpha;

	/*
	 * libPerm allows permutations to be represented in different way. Each of these representations is covered by a
	 * different class, but all classes implement the perm::AbstractPermutation interface. Currently, the following
	 * representations are implemented:
	 * - perm::ExplicitPermutation - represents a permutation by explicitly storing the permutation's image points
	 *
	 * Feel free to exchange ExplicitPermutation with any other permutation implementation
	 */

	// Default-constructed permutations always represent the identity permutation
	perm::ExplicitPermutation perm1;

	std::cout << "perm1 is the identity permutation: " << perm1.isIdentity() << std::endl;
	std::cout << "perm1 is " << perm1 << std::endl << std::endl;


	// Usually the most convenient way to construct permutations is to create them from (disjoint) cycle notation.
	// Beware the zero-based number system though!
	// Different permutation implementations may also offer different methods of construction

	// The following creates a permutation representing the (1-based) cycle (1 2)(2 3) or equivalently: (1 2 3)
	perm::ExplicitPermutation perm2 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 0, 1 }, { 1, 2 } }));

	std::cout << "perm2 is " << perm2 << std::endl;
	// There are different ways of getting the image of a point under the action of a permutation
	std::cout << "The image of 0 under perm2 is " << perm2[0] << std::endl;
	std::cout << "The image of 2 under perm2 is " << perm2.image(2) << std::endl;
	// You can also get the image of points that the permutation doesn't act on
	std::cout << "The image of 42 under perm2 is " << perm2[42] << std::endl << std::endl;


	// Permutations may also carry a sign information
	perm::ExplicitPermutation perm3 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 1, 3 }, { 0, 4 } }), -1);

	std::cout << "perm3 is " << perm3 << std::endl;
	std::cout << "Its sign is " << perm3.sign() << std::endl;
	std::cout << "perm2's sign is " << perm2.sign() << std::endl << std::endl;


	// Permutations can be multiplied. a*b means to first apply a and then apply b.
	// Due to some C++ restrictions (see docs), the base interface can only support in-place multiplication
	// If you require the multiplication result in a separate object, just copy a and then multiply b with that
	// in-place.
	perm::ExplicitPermutation perm4 = perm2;
	perm4 *= perm3;

	std::cout << "perm2 * perm3 is " << perm4 << std::endl;
	std::cout << "Because perm2 maps 0 to " << perm2[0] << " and perm3 maps " << perm2[0] << " to " << perm3[perm2[0]]
			  << ", perm2 * perm3 also maps 0 to " << perm4[0] << std::endl;
	std::cout << "Because the sign of perm2 is " << perm2.sign() << " and the sign of perm3 is " << perm3.sign()
			  << ", the sign of perm2 * perm3 is " << perm4.sign() << std::endl
			  << std::endl;


	// Permutations can also be inverted in-place
	perm::ExplicitPermutation perm5 = perm4;
	perm4.invert();

	std::cout << "The inverse of " << perm4 << " is " << perm5 << std::endl;
	std::cout << "The former maps 3 to " << perm4[3] << " and the latter maps that back to " << perm5[perm4[3]]
			  << std::endl;

	return 0;
}
