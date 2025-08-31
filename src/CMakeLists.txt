# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

target_sources(libperm
	PRIVATE
		"AbstractPermutation.cpp"
		"AbstractPermutationGroup.cpp"
		"Cycle.cpp"
		"DiminoAlgorithm.cpp"
		"DisjointCycles.cpp"
		"ExplicitPermutation.cpp"
		"PrimitivePermutationGroup.cpp"
		"Rank.cpp"
		"SchreierSims.cpp"

		"details/SignedPermutation.cpp"
		"details/Math.cpp"
)

include(GenerateExportHeader)

generate_export_header(libperm
	EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/libperm/libperm_macros.hpp"
	BASE_NAME LIBPERM
)

target_sources(libperm
	PUBLIC
		FILE_SET headers TYPE HEADERS
		BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/include"
		FILES "${CMAKE_CURRENT_BINARY_DIR}/include/libperm/libperm_macros.hpp"
)
