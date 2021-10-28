#
# set up some of mkenv.py's settings
all:

.PHONY: all main

BUILD ?= $(CURDIR)/../..
CROSS_COMPILE = $(PREFIX)

# re-use RIOT's variant of verbose build
ifeq ($(QUIET),1)
BUILD_VERBOSE = 0
else
BUILD_VERBOSE = 1
endif

include $(BUILD)/py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk

INC =  -I$(TOP)/ports/riot
INC += -I$(TOP)/shared/readline
#INC += -I../stmhal
INC += -I$(BUILD)
INC += $(INCLUDES)

Wno = -Wno-vla -Wno-unused-parameter -Wno-pedantic

CFLAGS += -std=gnu99 -DNDEBUG $(INC) $(Wno)

FROZEN_DIR = scripts

SRC_C = $(wildcard *.c) \
	shared/runtime/pyexec.c \
	shared/readline/readline.c \
	ports/unix/gccollect.c \
	shared/runtime/gchelper_generic.c \
	$(BUILD)/frozen.c \

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C)

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o) $(SRC_S:.s=.o))

BUILD_MAIN ?= 1
ifeq ($(BUILD_MAIN),1)
ALL = main

GENHDR_DIR = ../../genhdr

main:
	@if [ ! -e RIOT ]; then git clone https://github.com/RIOT-OS/RIOT; fi
	mkdir -p $(GENHDR_DIR) && touch $(GENHDR_DIR)/qstrdefs.generated.h
	RIOTBASE=$(CURDIR)/RIOT make -C main

clean:
	rm -rf main/bin $(GENHDR_DIR) ../../frozen.c ../../media
else
ALL = $(BINDIR)/micropython.a

$(BINDIR)/micropython.a: $(OBJ)
	$(AD)$(AR) $(ARFLAGS) $@ $?

include $(TOP)/py/mkrules.mk
endif

all: $(ALL)
