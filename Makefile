PRJ_NAME    := icp
PRJ_BRANCH  := 1.0
PRJ_VERSION := $(PRJ_BRANCH).0
PRJ_TYPE    := lib

TEST_EXTRA_LINK_LIBS = -L$(dir $(PE_LIB_PATH)) -l$(patsubst lib%.so,%,$(notdir $(PE_LIB_PATH))) -lpthread

PE_PRJ_DIR := ../pe
ifneq ($(wildcard $(PE_PRJ_DIR)),)
    TestPerfIString_EXTRA_CPP_FLAGS  := -I$(PE_PRJ_DIR)/src
    TestPerfIString_EXTRA_LD_PATH    := $(PE_PRJ_DIR)/build/lib/release
    TestPerfIString_EXTRA_LINK_FLAGS := -L$(TestPerfIString_EXTRA_LD_PATH) -lpe-1.0
    TestPerfIString_EXTRA_DEPENDENCY := $(PE_PRJ_DIR)/build/lib/release/libpe-1.0.so
    TestIStringMap_EXTRA_CPP_FLAGS   := $(TestPerfIString_EXTRA_CPP_FLAGS)
    TestIStringMap_EXTRA_LD_PATH     := $(TestPerfIString_EXTRA_LD_PATH)
    TestIStringMap_EXTRA_LINK_FLAGS  := $(TestPerfIString_EXTRA_LINK_FLAGS)
    TestIStringMap_EXTRA_DEPENDENCY  := $(TestPerfIString_EXTRA_DEPENDENCY)
    TestShortIStringMap_EXTRA_CPP_FLAGS  := $(TestPerfIString_EXTRA_CPP_FLAGS)
    TestShortIStringMap_EXTRA_LD_PATH    := $(TestPerfIString_EXTRA_LD_PATH)
    TestShortIStringMap_EXTRA_LINK_FLAGS := $(TestPerfIString_EXTRA_LINK_FLAGS)
    TestShortIStringMap_EXTRA_DEPENDENCY := $(TestPerfIString_EXTRA_DEPENDENCY)
    TestOrders_EXTRA_CPP_FLAGS           := $(TestPerfIString_EXTRA_CPP_FLAGS)
    TestOrders_EXTRA_LD_PATH             := $(TestPerfIString_EXTRA_LD_PATH)
    TestOrders_EXTRA_LINK_FLAGS          := $(TestPerfIString_EXTRA_LINK_FLAGS)
    TestOrders_EXTRA_DEPENDENCY          := $(TestPerfIString_EXTRA_DEPENDENCY)
else
    ifneq ($(findstring test-,$(MAKECMDGOALS))$(findstring check,$(MAKECMDGOALS)),)
        $(warning Performance tests are disabled: visit https://github.com/sashamakarenko/pe)
        $(warning Next to icp: git clone https://github.com/sashamakarenko/pe.git pe)
    endif
    DISABLED_TESTS = PerfIString
endif

#LINK_EXTRA_LIBS = -flto

-include ../makefile/Makefile

../makefile/Makefile:
	@echo "cloning makefile..."
	@cd ..; git clone https://github.com/sashamakarenko/makefile.git makefile

$(TestPerfIString_EXTRA_DEPENDENCY):
	$(SUBMAKE) -C $(PE_PRJ_DIR)

$(PE_LIB_PATH):
	$(SUBMAKE) -C $(PE_ROOT)
