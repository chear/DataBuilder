CURRENT_DIR=$(shell pwd)
BIN=$(shell cd ~/;pwd)
PARSER_PATH=$(BIN)/.vim_runtime/ctags_parser
CC = gcc -std=c99

##########################################################################
PIC_FLAGS = -fPIC
R_CFLAGS = $(PIC_FLAGS) -pedantic -Wall -Werror


##########################################################################

CJSON_TEST = test
CJSON_TEST_SRC = cJSON.c main.c
$(CJSON_TEST): $(CJSON_TEST_SRC) cJSON.h
		$(CC) $(CJSON_TEST_SRC)  -o $@ $(LDLIBS) -I.

##########################################################################

all: $(CJSON_TEST)
	./test

ctags:
	ctags --options=NONE --options=$(PARSER_PATH)/linux.ctags --langmap=c:+.h --extras=+q --if0=no -o c_tags -R;



clean:
	rm -f $(CJSON_TEST);
