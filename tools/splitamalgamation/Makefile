
BITMAN=1
ENABLE_MATH=1
UNTRUST=1

SRC = ph7.c vm.c vfs.c parse.c oo.c memobj.c
SRC += lib.c lex.c hashmap.c constant.c 
SRC += compile.c builtin.c api.c


CC = clang
CFLAGS =  -W -Wunused -Wall -I. -Os 
LDFLAGS += -pie -s

ifeq ($(ENABLE_MATH),1)
CFLAGS += -D PH7_ENABLE_MATH_FUNC
LDFLAGS += -lm
endif

ifeq ($(BITMAN),1)
CFLAGS += -D BITMAN
SRC += bitman.ph7.c
endif

ifeq ($(UNTRUST),1)
CFLAGS += -D UNTRUST
endif

SRC += ph7i.c
OBJ = ${SRC:.c=.o}


OUT = ph7

$(OUT): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(SRC): ph7.h ph7int.h
