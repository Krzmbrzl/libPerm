// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/AbstractPermutation.hpp>
#include <libperm/Cycle.hpp>
#include <libperm/ExplicitPermutation.hpp>
#include <libperm/Permutation.hpp>

#include <iostream>
#include <vector>

/*
 * Suppose you want to write a function that takes two permutations and returns the result of this multiplication.
 * If you want the function to work, regardless of what exact type the input permutations are, then the logical idea
 * would be to simply use the interface type instead.
 * That does indeed allow you to pass any kind of permutation into your function, but as it turns out, you can't make
 * your function work in this way, because in order for your function to work, you require a so-called value-type (aka:
 * a type that behaves similar to e.g. a plain int)
 */

// Uncomment this implementation to observe the compiler errors this produces
/*
perm::AbstractPermutation multiplyWrong(const perm::AbstractPermutation &perm1, const perm::AbstractPermutation &perm2)
{ perm::AbstractPermutation result = perm1; result *= perm2;

	return result;
}
*/


/*
 * In order to solve this problem, libPerm provides the perm::Permutation wrapper type. The details of its
 * implementation are not important. Just remember that whenever you get errors because you tried to e.g.
 * copy or return a permutation, you should be able to simply use perm::Permutation instead.
 */

perm::Permutation multiply(const perm::Permutation &perm1, const perm::AbstractPermutation &perm2) {
	// Note: We could also have changed perm2 to be a perm::Permutation, but in this case, we
	// don't have to do anything with perm2 that is not possible through the perm::AbstractPermutation
	// interface, so we left it as is (to demonstrate that it's possible).
	perm::Permutation result = perm1;
	result *= perm2;

	return result;
}

int main() {
	perm::Permutation perm1(perm::ExplicitPermutation(perm::Cycle({ 1, 3 })));
	perm::ExplicitPermutation perm2 = perm::ExplicitPermutation(perm::Cycle({ 2, 0, 1 }));

	perm::Permutation result = multiply(perm1, perm2);

	std::cout << perm1 << " * " << perm2 << " = " << result << std::endl;


	/*
	 * When trying to store an arbitrary permutation somewhere (e.g. in a vector), this might APPEAR to
	 * work as the compiler won't necessarily generate any errors right away. However, this will
	 * BREAK YOUR CODE. (And once the compiler does generate errors, those usually are really nasty)
	 * The reason for this is a phenomenon called "slicing". The gist of it is: You can't store an
	 * object in a variable or container of its base type without messing things up.
	 */
	// std::vector< perm::AbstractPermutation > permutations;

	/*
	 * Again, the solution is to use perm::Permutation instead of perm::AbstractPermutation
	 */
	std::vector< perm::Permutation > permutations;

	permutations.push_back(result);

	std::cout << "Above result recovered from vector: " << permutations[0] << std::endl;


	/*
	 * As a general rule of thumb, it is recommended to simply represent all permutations at all times
	 * as perm::Permutation, unless you have a reason not to.
	 */
}
