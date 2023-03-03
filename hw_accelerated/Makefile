############################## Setting up Project Variables ##############################
PWD = $(shell readlink -f .)
PROJ_ROOT = $(PWD)

PLATFORM ?= xilinx_u280_xdma_201920_3
include ./utils.mk

TEMP_DIR := ./_x.$(TARGET).$(XSA)
BUILD_DIR := ./build_dir.$(TARGET).$(XSA)

EXECUTABLE = ./csat
KERNEL = solve

KERNEL_BIN = $(BUILD_DIR)/$(KERNEL).xclbin
EMCONFIG_DIR = $(TEMP_DIR)

LINK_OUTPUT := $(BUILD_DIR)/$(KERNEL).link.xclbin
PACKAGE_OUT = ./package.$(TARGET)

############################## Setting up Flags ##############################
CXX_FLAGS += -I$(XILINX_XRT)/include -I$(XILINX_VIVADO)/include -I$(PROJ_ROOT)/includes/xcl2 -Wall -std=c++17 -fmessage-length=0
CXX_DEBUG_FLAGS += -O0 -g
CXX_PRODUCTION_FLAGS += -O3
LDFLAGS += -L$(XILINX_XRT)/lib -pthread -lOpenCL -lrt -lstdc++ 

VPP_FLAGS += -t $(TARGET) --platform $(PLATFORM) --save-temps --hls.pre_tcl $(PROJ_ROOT)/config/hls_config.tcl
VPP_DEBUG_FLAGS = -g --profile.data all:all:all
VPP_PRODUCTION_FLAGS = -O3

COMMONFLAGS = -I$(PROJ_ROOT)/../shared/src  -Wunused-variable
CXX_FLAGS += $(COMMONFLAGS)
VPP_FLAGS += $(COMMONFLAGS)

DEBUG := no
ifeq ($(DEBUG), yes)
	CXX_FLAGS += $(CXX_DEBUG_FLAGS)
	VPP_FLAGS += $(VPP_DEBUG_FLAGS)
#VPP_LDFLAGS += --debug.list_ports
else
	CXX_FLAGS += $(CXX_PRODUCTION_FLAGS)
	VPP_FLAGS += $(VPP_PRODUCTION_FLAGS)
endif

############################## Setting up Files ##############################
HOST_SRCS += $(PROJ_ROOT)/includes/xcl2/xcl2.cpp ./src/host.cpp 
HW_HEADERS := $(wildcard src/*.hpp)
SHARED_HEADERS := $(wildcard ../shared/src/*.hpp)
HEADERS := $(HW_HEADERS) $(SHARED_HEADERS)

############################## Setting Targets ##############################
.PHONY: all clean cleanall docs emconfig
all: check-platform check-vitis $(EXECUTABLE) $(BUILD_DIR)/$(KERNEL).xclbin emconfig

.PHONY: host
host: $(EXECUTABLE)

############################## Setting Rules for Binary Containers (Building Kernels) ##############################
$(TEMP_DIR)/$(KERNEL).xo: src/$(KERNEL).cpp $(HEADERS)
	mkdir -p $(TEMP_DIR)
	v++ $(VPP_FLAGS) -c -k $(KERNEL) --temp_dir $(TEMP_DIR) -I'$(<D)' -o'$@' '$<'

$(BUILD_DIR)/$(KERNEL).xclbin: $(TEMP_DIR)/$(KERNEL).xo
	mkdir -p $(BUILD_DIR)
	v++ $(VPP_FLAGS) -l $(VPP_LDFLAGS) --temp_dir $(TEMP_DIR) -o'$(LINK_OUTPUT)' $(<)
	v++ -p $(LINK_OUTPUT) $(VPP_FLAGS) --package.out_dir $(PACKAGE_OUT) -o $(BUILD_DIR)/$(KERNEL).xclbin

############################## Setting Rules for Host (Building Host Executable) ##############################
$(EXECUTABLE): $(HOST_SRCS) $(HEADERS) | check-xrt
	g++ -o $@ $(HOST_SRCS) $(CXX_FLAGS) $(LDFLAGS)

emconfig:$(EMCONFIG_DIR)/emconfig.json
$(EMCONFIG_DIR)/emconfig.json:
	emconfigutil --platform $(PLATFORM) --od $(EMCONFIG_DIR)

############################## Setting Essential Checks and Running Rules ##############################
run: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	cp -rf $(EMCONFIG_DIR)/emconfig.json .
	XCL_EMULATION_MODE=$(TARGET) $(EXECUTABLE) $(KERNEL_BIN) $(CIRCUIT) $(SAT)
else
	$(EXECUTABLE) $(KERNEL_BIN) $(CIRCUIT) $(SAT)
endif

.PHONY: test
test: $(EXECUTABLE)
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=$(TARGET) $(EXECUTABLE) $(KERNEL_BIN) $(CIRCUIT) $(SAT)
else
	$(EXECUTABLE) $(KERNEL_BIN) $(CIRCUIT) $(SAT)
endif

############################## Cleaning Rules ##############################
# Cleaning stuff
clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} 
	-$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	-$(RMDIR) src/*.ll *v++* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb

cleanall: clean
	-$(RMDIR) build_dir*
	-$(RMDIR) package.*
	-$(RMDIR) _x* *xclbin.run_summary qemu-memory-_* emulation _vimage pl* start_simulation.sh *.xclbin
	-$(RMDIR) .run

############################## Help Section ##############################
help:
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw>"
	$(ECHO) "      Command to generate the design for specified target."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
	$(ECHO) "  make test TARGET=<sw_emu/hw_emu/hw(default)> CIRCUIT=<.eqn file path> SAT=<output to satisfy>"
	$(ECHO) "      Command to run the application. This is same as 'run' target but does not have any makefile dependency."
	$(ECHO) ""
	$(ECHO) "  make run TARGET=<sw_emu/hw_emu/hw(default)> CIRCUIT=<.eqn file path> SAT=<output to satisfy>"
	$(ECHO) "      Command to build the host and kernel and test it."
	$(ECHO) ""
	$(ECHO) "  make host"
	$(ECHO) "      Command to build host application."
	$(ECHO) ""

############################## License Section ##############################
#
# Copyright 2019-2021 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# makefile-generator v1.0.3
#