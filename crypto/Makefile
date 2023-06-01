CC = gcc

CFLAGS = -Wall -Werror -Wextra -pedantic -g

SRC = ec_create.c ec_from_pub.c ec_load.c ec_save.c ec_sign.c ec_to_pub.c ec_verify.c sha256.c

OBJ = $(SRC:.c=.o)

LIB = libhblk_crypto.a

all = $(LIB)

.PHONY: all clean

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB)

