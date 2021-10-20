all: build

GENHDR_DIR := ../../genhdr

RIOT:
	@if [ ! -e RIOT ]; then \
		git clone https://github.com/RIOT-OS/RIOT; fi

build: RIOT
	mkdir -p $(GENHDR_DIR) && touch $(GENHDR_DIR)/qstrdefs.generated.h
	RIOTBASE=$(CURDIR)/RIOT make -C main

clean:
	rm -rf main/bin $(GENHDR_DIR) ../../frozen.c
