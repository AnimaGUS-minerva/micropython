all: build

GENHDR_DIR := ../../genhdr

RIOT:
	git -c advice.detachedHead=false \
		clone --branch 2021.04 https://github.com/RIOT-OS/RIOT

build: RIOT
	mkdir -p $(GENHDR_DIR) && touch $(GENHDR_DIR)/qstrdefs.generated.h
	RIOTBASE=$(CURDIR)/RIOT make -C main

clean:
	rm main/bin \
		-rf $(GENHDR_DIR) ../../frozen.c \
