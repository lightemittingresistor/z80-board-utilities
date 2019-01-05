SET(LIBUSB_DOWNLOAD https://github.com/libusb/libusb/releases/download/v1.0.22/libusb-1.0.22.tar.bz2)

include(ExternalProject)

ExternalProject_Add(libusb
    PREFIX libusb
    URL ${LIBUSB_DOWNLOAD}
    CONFIGURE_COMMAND ./configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/libusb-install
    WORKING_DIRECTORY libusb-1.0.22
    BUILD_COMMAND ${MAKE_EXE}
    BUILD_IN_SOURCE 1
)

SET(LibUSB_LIBRARIES 
    ${CMAKE_CURRENT_BINARY_DIR}/libusb-install/lib/libusb-1.0${CMAKE_SHARED_LIBRARY_SUFFIX})
SET(LibUSB_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/libusb-install/include/)