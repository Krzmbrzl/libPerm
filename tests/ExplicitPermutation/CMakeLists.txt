# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

add_executable(TestExplicitPermutation
	TestExplicitPermutation.cpp
)

target_link_libraries(TestExplicitPermutation PUBLIC libperm)

register_test(TARGETS TestExplicitPermutation)
