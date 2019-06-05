SRC		=	printk.c memchr.c memcmp.c memcpy.c memmove.c memset.c			\
			strchr.c strcmp.c strcpy.c strlen.c strnlen.c

OBJ		=	$(addsuffix .o, $(basename $(SRC)))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) -rcs $@ $(OBJ)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
