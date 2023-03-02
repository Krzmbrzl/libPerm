// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_PRIMITIVEPERMUTATIONGROUP_HPP_
#define LIBPERM_PRIMITIVEPERMUTATIONGROUP_HPP_

#include "libperm/AbstractPermutation.hpp"
#include "libperm/AbstractPermutationGroup.hpp"
#include "libperm/Permutation.hpp"

#include <iosfwd>
#include <iterator>
#include <type_traits>
#include <vector>

namespace perm {

class PrimitivePermutationGroup : public AbstractPermutationGroup {
public:
	PrimitivePermutationGroup();
	PrimitivePermutationGroup(std::vector< Permutation > generators);

	template< typename Iterator > PrimitivePermutationGroup(Iterator begin, Iterator end) {
		static_assert(std::is_same_v< typename std::iterator_traits< Iterator >::value_type, Permutation >,
					  "Can only construct PrimitivePermutationGroup with generators of type perm::Permutation");

		std::vector< Permutation > generators;
		generators.reserve(std::distance(begin, end));

		while (begin != end) {
			generators.push_back(*begin);
			begin++;
		}

		PrimitivePermutationGroup(std::move(generators));
	}

	PrimitivePermutationGroup(const PrimitivePermutationGroup &) = default;
	PrimitivePermutationGroup(PrimitivePermutationGroup &&)      = default;

	PrimitivePermutationGroup &operator=(const PrimitivePermutationGroup &) = default;
	PrimitivePermutationGroup &operator=(PrimitivePermutationGroup &&) = default;


	virtual std::vector< AbstractPermutation::value_type >
		orbit(AbstractPermutation::value_type point) const override final;

	virtual std::size_t order() const override final;

	virtual bool contains(const AbstractPermutation &perm) const override final;

	virtual bool addGenerator(Permutation perm) override final;

	virtual void setGenerators(std::vector< Permutation > generators) override final;

	virtual const std::vector< Permutation > &getGenerators() const override final;

	virtual void getElementsTo(std::vector< Permutation > &permutations) const override final;

	virtual Permutation getCanonicalCosetRepresentative(const AbstractPermutation &perm) const override;

	virtual const Permutation &getCanonicalCosetRepresentative() const override;

	friend std::ostream &operator<<(std::ostream &stream, const PrimitivePermutationGroup &group);

protected:
	std::vector< Permutation > m_generators;
	std::vector< Permutation > m_elements;

	void regenerateGroup();
	void sortRepresentation();
};

} // namespace perm

#endif // LIBPERM_PRIMITIVEPERMUTATIONGROUP_HPP_
