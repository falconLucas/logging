CC := gcc
CFLAGS += -Wall -Werror -g
CPPFLAGS += 

OUT_FMT := exe

LIB := ../src
BUILD_DIR ?= build
INC := 	$(LIB) .
SRC := $(LIB)/logging.c logging-conf.c 
SRC_DIR := $(LIB)

INCFLAGS := $(addprefix -I, $(INC))
CFLAGS += $(INCFLAGS)

all: test_logging

test_logging: $(SRC)
	$(CC) $^ $(CFLAGS) $(CPPFLAGS) -o $@.$(OUT_FMT) $@.c $(LDFLAGS)

.PHONY: clean debug all

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(wildcard *.$(OUT_FMT))

debug:
	$(info SRC=$(SRC), OBJ=$(OBJ), SRC_DIR=$(SRC_DIR), BUILD_DIR=$(BUILD_DIR), LDFLAGS=$(LDFLAGS))
