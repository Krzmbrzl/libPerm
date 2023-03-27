// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/ExplicitPermutation.hpp>
#include <libperm/Utils.hpp>

#include <iostream>
#include <string>
#include <vector>


// Support for printing vectors to the console
template< typename T > std::ostream &operator<<(std::ostream &stream, const std::vector< T > &vec) {
	stream << "{ ";

	for (std::size_t i = 0; i < vec.size(); ++i) {
		stream << vec[i];
		if (i + 1 < vec.size()) {
			stream << ",";
		}

		stream << " ";
	}

	return stream << "}";
}


int main() {
	std::vector< std::string > myVec = { "I", "am", "a", "sentence" };

	std::cout << "Original:\n" << myVec << "\n";

	// The permutation describes what element of the original vector shall be placed at position i of the new vector
	perm::ExplicitPermutation yodaPerm({ 2, 3, 1, 0 });
	perm::applyPermutation(myVec, yodaPerm);
	std::cout << "As Yoda would say:\n" << myVec << "\n";

	// The inverse permutation (naturally) contains the recipe to undo these changes
	perm::ExplicitPermutation reverseYoda = yodaPerm;
	reverseYoda.invert();
	perm::applyPermutation(myVec, reverseYoda);
	std::cout << "Restored original:\n" << myVec << "\n\n";


	// Now some random scrambling
	// This will be applied by two consecutively applied permutations

	perm::ExplicitPermutation first  = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 0, 1, 2 }));
	perm::ExplicitPermutation second = perm::ExplicitPermutation::fromCycle(perm::Cycle({ 2, 3 }));

	perm::applyPermutation(myVec, first);
	std::cout << "After first permutation:" << std::endl;
	std::cout << myVec << std::endl;

	perm::applyPermutation(myVec, second);
	std::cout << "After second permutation:" << std::endl;
	std::cout << myVec << std::endl;

	// In order to reverse the action of both permutations at once, we have to create
	// the combined inverse
	// Note: When applying permutations to lists, the order of composition reverses and therefore
	// we have to read our resulting "expression" from right-to-left rather than left-to-right.
	// See the manual for why this is the case.
	perm::ExplicitPermutation undo = first;
	undo.preMultiply(second);
	undo.invert();
	perm::applyPermutation(myVec, undo);
	std::cout << "Undoing the previous permutations:" << std::endl;
	std::cout << myVec << std::endl;

	return 0;
}
