# This file is part of libPerm. Use of this source code is
# governed by a BSD-style license that can be found in the
# LICENSE file at the root of the libPerm source tree or at
# <https://github.com/Krzmbrzl/libPerm/blob/develop/LICENSE>.

cmake_minimum_required(VERSION 3.23)

# Default to a release build
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build to produce")


project(libperm,
	VERSION "2.0.0"
	LANGUAGES "CXX"
)


include("${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/compiler.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/install_dirs.cmake")
include(CTest)

if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
endif()

add_library(libperm)
set_target_properties(libperm PROPERTIES PREFIX "")
set_internal_build_flags(libperm)

add_library(libperm::libperm ALIAS libperm)

file(GLOB_RECURSE LIBPERM_HEADER_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/libperm/*.hpp")
target_sources(libperm
	PUBLIC
		FILE_SET headers TYPE HEADERS
		BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
		FILES "${LIBPERM_HEADER_FILES}"
)
target_include_directories(libperm SYSTEM PUBLIC "$<INSTALL_INTERFACE:${LIBPERM_INSTALL_INCLUDEDIR}>")

set_target_properties(libperm
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
		EXPORT_NAME libperm
)

target_link_libraries(libperm PUBLIC polymorphic_variant)

enable_testing()

add_subdirectory(src)

add_subdirectory(tests)

add_subdirectory(examples)

option(LIBPERM_INSTALL "Whether to create an install target for libPerm" ON)

if (LIBPERM_INSTALL)
	include(CMakePackageConfigHelpers)
	configure_package_config_file(
		"${PROJECT_SOURCE_DIR}/cmake/libperm-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/libperm-config.cmake"
		INSTALL_DESTINATION "${LIBPERM_INSTALL_CMAKEDIR}"
	)
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/libperm-config-version.cmake"
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY SameMajorVersion
	)

	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/libperm-config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/libperm-config-version.cmake"
		DESTINATION "${LIBPERM_INSTALL_CMAKEDIR}"
	)

	install(
		TARGETS libperm
		EXPORT libperm-targets
		RUNTIME DESTINATION "${LIBPERM_INSTALL_BINDIR}"
			COMPONENT libperm_runtime
		LIBRARY DESTINATION "${LIBPERM_INSTALL_LIBDIR}"
			COMPONENT libperm_runtime
			NAMELINK_COMPONENT libperm_development
		ARCHIVE DESTINATION "${LIBPERM_INSTALL_LIBDIR}"
		COMPONENT libperm_development
		FILE_SET headers
	)
	install(
		EXPORT libperm-targets
		DESTINATION "${LIBPERM_INSTALL_CMAKEDIR}"
		FILE libperm-targets.cmake
		NAMESPACE libperm::
		COMPONENT libperm_development
	)
endif()

# Make lib consumable from build directory without installing
export(TARGETS libperm
	NAMESPACE libperm::
	FILE "${CMAKE_CURRENT_BINARY_DIR}/libperm-targets.cmake"
)
