// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/AbstractPermutationGroup.hpp>
#include <libperm/Cycle.hpp>
#include <libperm/ExplicitPermutation.hpp>
#include <libperm/Permutation.hpp>
#include <libperm/PrimitivePermutationGroup.hpp>

#include <iostream>
#include <vector>

/*
 * In analogy to perm::AbstractPermutation, there is a general interface for permutation groups:
 * perm::AbstractPermutationGroup
 */

void printGroup(const perm::AbstractPermutationGroup &group) {
	// A group allows explicit access to its elements by providing a function
	// to copy those elements into a given vector of perm::Permutation objects.
	std::vector< perm::Permutation > elements;
	group.getElementsTo(elements);

	// Getting a group's order is easy
	std::cout << "Permutation group of order " << group.order() << ":" << std::endl;

	for (std::size_t i = 0; i < elements.size(); ++i) {
		std::cout << i << ". " << elements[i] << std::endl;
	}

	std::cout << std::endl;
}

int main() {
	// Together these two generators generate Sym(4) - the symmetric group of degree 4
	perm::Permutation generator1(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2, 3 })));
	perm::Permutation generator2(perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 3 })));

	std::cout << std::boolalpha;

	/*
	 * libPerm allows permutation groups to be represented in different ways. Each of these representations is covered
	 * in a different class, but all class implement the perm::AbstractPermutationGroup interface. Currently, the
	 * following representations are implemented:
	 * - perm::PrimitivePermutationGroup - represents a group by explicitly storing the group's elements
	 */

	// By default a group only contains the identity permutation
	perm::PrimitivePermutationGroup group;
	std::cout << "== Default constructed ==" << std::endl;
	printGroup(group);


	// Groups are constructed/defined in terms of their generators. These can be added one after the other...
	group.addGenerator(generator1);
	std::cout << "== With generator " << generator1 << " ==" << std::endl;
	printGroup(group);

	group.addGenerator(generator2);
	std::cout << "== With additional generator " << generator2 << " ==" << std::endl;
	printGroup(group);


	// ...or by specifying the generators all together in one go
	// Note that setGenerators wipes the previous state of the group and starts from scratch
	group.setGenerators({ generator2, generator1 });
	std::cout << "== All generators at once ==" << std::endl;
	printGroup(group);



	// Compute the orbit of a point under the action of a group
	std::cout << "The orbit of the point 2 under above group is" << std::endl << "{ ";
	for (perm::AbstractPermutation::value_type point : group.orbit(2)) {
		std::cout << point << ", ";
	}
	std::cout << "}" << std::endl;


	// Check membership of a given permutation
	perm::ExplicitPermutation perm1 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ { 1, 2 }, { 0, 3 } }));
	perm::ExplicitPermutation perm2 = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 1, 4 }));
	std::cout << "Group contains " << perm1 << ": " << group.contains(perm1) << std::endl;
	std::cout << "Group contains " << perm2 << ": " << group.contains(perm2) << std::endl;


	// Groups also support retrieving a "canonical coset representative". For more details on this, see the
	// documentation
	std::cout << "Canonical coset representative: " << group.getCanonicalCosetRepresentative() << std::endl;
	std::cout << "Canonical coset representative for coset with " << perm2 << ": "
			  << group.getCanonicalCosetRepresentative(perm2) << std::endl;
}
