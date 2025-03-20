include(GNUInstallDirs)

set(LIBPERM_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/libperm-${PROJECT_VERSION_MAJOR}"
	CACHE FILEPATH "Directory into which cmake related files (e.g. libperm-config.cmake) are installed")

set(LIBPERM_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}"
	CACHE FILEPATH "Directory into which to install libPerm executables")

set(LIBPERM_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}"
	CACHE FILEPATH "Directory into which libPerm libraries (except DLLs on Windows) are installed")

set(LIBPERM_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}"
	CACHE FILEPATH "Directory into which the 'libperm' directory with all libPerm header files is installed")
