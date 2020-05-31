PRJ_NAME    := icp
PRJ_BRANCH  := 1.0
PRJ_VERSION := $(PRJ_BRANCH).0
PRJ_TYPE    := lib

PE_ROOT     := ../pe
PE_LIB_PATH := $(PE_ROOT)/build/lib/release/libpe-1.0.so

TEST_INCLUDES = -I$(PE_ROOT)/src
TEST_EXTRA_DEPENDENCY := $(PE_LIB_PATH)
TEST_EXTRA_LINK_LIBS = -L$(dir $(PE_LIB_PATH)) -l$(patsubst lib%.so,%,$(notdir $(PE_LIB_PATH)))
TEST_EXTRA_LD_PATH :=$(dir $(PE_LIB_PATH))

include mfile/Makefile

$(PE_LIB_PATH):
	$(SUBMAKE) -C $(PE_ROOT)
