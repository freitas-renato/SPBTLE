THIS_PATH := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

C_INCLUDES +=                                                          \
	-I$(CUBE_DIR)/Middlewares/ST/BlueNRG-MS/includes                   \
	-I$(CUBE_DIR)/Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic  \
	-I$(CUBE_DIR)/Middlewares/ST/BlueNRG-MS/utils                      \
	-I$(THIS_PATH)/inc                                                 \

SUBM_SOURCES += $(shell find $(THIS_PATH) -name "*.c")

undefine THIS_PATH
