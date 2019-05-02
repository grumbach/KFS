# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/04 17:08:23 by agrumbac          #+#    #+#              #
#    Updated: 2018/12/17 02:39:54 by agrumbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## VAR #############################################

NAME = Kagrum

AS = nasm

ASFLAGS = -f elf

CC = i686-elf-gcc

CFLAGS = -fno-builtin -fno-exceptions -fno-stack-protector \
	-nostdlib -nodefaultlibs \
	-std=gnu99 -ffreestanding -O2 -Wall -Wextra

LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

############################## SRC #############################################

SRC = boot/boot.s \
	kernel/kernel.c

COBJ = $(SRC:.c=.o)
OBJ = $(COBJ:.s=.o)

############################## COLORS ##########################################

R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
X = "\033[0m"

############################## BUILD ###########################################

#.SILENT:

all: ${NAME}

${NAME}: ${OBJ}
	i686-elf-gcc -T linker.ld ${LDFLAGS} -o ${NAME} ${OBJ}

############################## MORE ############################################

check:
	@grub-file --is-x86-multiboot ${NAME} && echo multiboot OK || echo NOT multiboot

iso: ${NAME}
	$(shell printf 'menuentry "${NAME}" {\n\
	\tmultiboot /boot/${NAME}\n\
	}\n' > grub.cfg)
	/bin/mkdir -p isodir/boot/grub
	/bin/cp Kagrum isodir/boot/Kagrum
	/bin/cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ${NAME}.iso isodir

run: ${NAME}
	qemu-system-i386 -kernel Kagrum -curses

clean:
	@echo ${R}Cleaning"  "[objs]...${X}
	/bin/rm -f ${OBJ}
	@echo ${R}Cleaning"  "[iso]...${X}
	/bin/rm -rf isodir
	/bin/rm -f grub.cfg
	/bin/rm -f ${NAME}.iso

fclean: clean
	@echo ${R}Cleaning"  "[${NAME}]...${X}
	/bin/rm -f ${NAME}

re:
	$(MAKE) fclean
	$(MAKE) all

############################## ART #############################################

art:
	@echo ASCII_ART
