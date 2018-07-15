KANET_SRC_PATH = ./kanet-src
# OBJ_PATH = ./obj
KANET_BUILD_PATH = ./bin

CC = g++
CFLARGS = -std=c++11 -g -O2 -Wall 
LDFLARGS = -pthread

KANET_SRC = $(wildcard $(KANET_SRC_PATH)/*.cc)
# INCLUDE = $(wildcard ${DIR_INC}/*.h)
# OBJ = $(patsubst %.cc,${DIR_OBJ}/%.o,$(notdir ${SRC}))

BUILD_PATH = $(KANET_BUILD_PATH)

# dump:
# 	@echo $(KANET_SRC)

all : \
	$(BUILD_PATH)\
	$(KANET_BUILD_PATH)/kanet

$(BUILD_PATH):
	mkdir $(foreach v, $(BUILD_PATH), $(v)) -p

# BUILD_PATH:
# 	@if [ ! -d $(DIR_BIN) ]; then mkdir ${DIR_BIN}; fi;
# OBJ_PATH:
# 	@if [ ! -d $(DIR_OBJ) ]; then mkdir ${DIR_OBJ}; fi;

$(KANET_BUILD_PATH)/kanet : $(KANET_SRC) | $(BUILD_PATH)
	$(CC) $(CFLARGS) $(LDFLARGS) -Ikanet-src -o $@ $^


.PHONY:clean
clean:
	rm -rf \
	$(foreach v, $(BUILD_PATH), $(v)/kanet)