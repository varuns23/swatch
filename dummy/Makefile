SWATCH_ROOT= $(shell pwd)/..

BUILD_HOME = $(SWATCH_ROOT)
include ${SWATCH_ROOT}/config/mfCommonDefs.mk

Project = swatch
Package = swatch/dummy

PackagePath = $(SWATCH_ROOT)/dummy
PackageName = cactuscore-swatch-dummy

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 1
PACKAGE_VER_MINOR = 6
PACKAGE_VER_PATCH = 2
PACKAGE_RELEASE ?= 1

Library = cactus_swatch_dummy

Includes =	\
		include  \
		${SWATCH_ROOT}/core/include \
		${SWATCH_ROOT}/action/include \
		${SWATCH_ROOT}/processor/include \
		${SWATCH_ROOT}/dtm/include \
		${SWATCH_ROOT}/system/include \
		${CACTUS_ROOT}/include \
		$(XDAQ_ROOT)/include \
		$(XDAQ_ROOT)/include/linux


LibraryPaths = \
		${SWATCH_ROOT}/core/lib \
		${SWATCH_ROOT}/action/lib \
		${SWATCH_ROOT}/processor/lib \
		${SWATCH_ROOT}/dtm/lib \
		${SWATCH_ROOT}/system/lib \
		${CACTUS_ROOT}/lib \
		$(XDAQ_ROOT)/lib


Libraries =	\
		cactus_swatch_core \
		cactus_swatch_action \
		cactus_swatch_processor \
		cactus_swatch_dtm \
		cactus_swatch_system \
		protobuf \

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
