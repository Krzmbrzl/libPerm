// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#ifndef LIBPERM_PRIMITIVEPERMUTATIONGROUP_HPP_
#define LIBPERM_PRIMITIVEPERMUTATIONGROUP_HPP_

#include "libperm/AbstractPermutation.hpp"
#include "libperm/AbstractPermutationGroup.hpp"
#include "libperm/Permutation.hpp"

#include <initializer_list>
#include <iosfwd>
#include <iterator>
#include <type_traits>
#include <vector>

namespace perm {

class PrimitivePermutationGroup : public AbstractPermutationGroup {
public:
	PrimitivePermutationGroup();
	PrimitivePermutationGroup(std::vector< Permutation > generators);

	template< typename Perm >
	PrimitivePermutationGroup(std::initializer_list< Perm > generators) : PrimitivePermutationGroup() {
		if constexpr (!std::is_same_v< std::remove_const_t< Perm >, Permutation >) {
			static_assert(std::is_base_of_v< AbstractPermutation, Perm >, "Can only use proper permutation classes");
		}

		std::vector< Permutation > transformedGenerators;
		transformedGenerators.reserve(generators.size());

		for (std::size_t i = 0; i < generators.size(); ++i) {
			transformedGenerators.emplace_back(*(std::data(generators) + i));
		}

		setGenerators(std::move(transformedGenerators));
	}

	template< typename Iterator, typename Perm = typename std::iterator_traits< Iterator >::value_type >
	PrimitivePermutationGroup(Iterator begin, Iterator end) : PrimitivePermutationGroup() {
		if constexpr (!std::is_same_v< std::remove_const_t< Perm >, Permutation >) {
			static_assert(std::is_base_of_v< AbstractPermutation, Perm >, "Can only use proper permutation classes");
		}

		std::vector< Permutation > generators;
		generators.reserve(std::distance(begin, end));

		while (begin != end) {
			generators.emplace_back(*begin);
			begin++;
		}

		setGenerators(std::move(generators));
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

	virtual std::vector< Permutation > leftCoset(const AbstractPermutation &perm) const override final;

	virtual std::vector< Permutation > rightCoset(const AbstractPermutation &perm) const override final;

	virtual Permutation leftCosetRepresentative(const AbstractPermutation &perm) const override;

	virtual Permutation rightCosetRepresentative(const AbstractPermutation &perm) const override;

	friend std::ostream &operator<<(std::ostream &stream, const PrimitivePermutationGroup &group);

protected:
	std::vector< Permutation > m_generators;
	std::vector< Permutation > m_elements;

	void regenerateGroup();
	void sortRepresentation();
};

} // namespace perm

#endif // LIBPERM_PRIMITIVEPERMUTATIONGROUP_HPP_
