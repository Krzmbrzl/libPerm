// This file is part of libPerm. Use of this source code is
// governed by a BSD-style license that can be found in the
// LICENSE file at the root of the libPerm source tree or at
// <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

#include <libperm/Cycle.hpp>

#include <gtest/gtest.h>

TEST(Cycle, construction) {
	perm::Cycle c1(4);
	// (1,2)
	perm::Cycle c2(4, { 1, 2 });
	// (1,2)(3,4)
	perm::Cycle c3(4, { { 1, 2 }, { 3, 4 } });

	ASSERT_EQ(c1.begin(), c1.end());
	ASSERT_EQ(*c2.begin(), *c3.begin());
}

TEST(Cycle, toImage) {
	// This simply represents the identity ()
	perm::Cycle c1(4);
	perm::Cycle c2(4, { 1, 2 });
	perm::Cycle c3(4, { 3, 2, 4 });
	perm::Cycle c4(4, { { 1, 2 }, { 3, 4 } });
	// This is a quirky (read: non-disjoint) way of writing down (1,2,3)
	perm::Cycle c5(4, { { 1, 2 }, { 2, 3 } });

	std::vector< unsigned int > expectedImage = { 0, 1, 2, 3 };
	ASSERT_EQ(c1.toImage< unsigned int >(), expectedImage);

	expectedImage = { 1, 0, 2, 3 };
	ASSERT_EQ(c2.toImage< unsigned int >(), expectedImage);

	expectedImage = { 0, 3, 1, 2 };
	ASSERT_EQ(c3.toImage< unsigned int >(), expectedImage);

	expectedImage = { 1, 0, 3, 2 };
	ASSERT_EQ(c4.toImage< unsigned int >(), expectedImage);

	expectedImage = { 1, 2, 0, 3 };
	ASSERT_EQ(c5.toImage< unsigned int >(), expectedImage);
}

TEST(Cycle, equality) {
	ASSERT_EQ(perm::Cycle(4), perm::Cycle(4));
	ASSERT_NE(perm::Cycle(3), perm::Cycle(4));
	ASSERT_NE(perm::Cycle(4), perm::Cycle(5));

	perm::Cycle c1(4, { 1, 2 });
	perm::Cycle c2(4, { 2, 1 });
	ASSERT_EQ(c1, c2);

	c1 = perm::Cycle(4, { { 1, 2 }, { 2, 3 } });
	c2 = perm::Cycle(4, { 1, 2, 3 });
	ASSERT_EQ(c1, c2);
}
