# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/04 17:08:23 by agrumbac          #+#    #+#              #
#    Updated: 2019/06/06 00:50:46 by agrumbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## VAR #############################################

ifeq ($(shell uname -s), Darwin)
	CC = i386-elf-gcc
	AR = i386-elf-ar
	GRUB = i386-elf-grub
else
	CC = i686-elf-gcc
	AR = i686-elf-ar
	GRUB = grub
endif

NAME = kfs

AS = nasm

ASFLAGS = -f elf

CFLAGS = -fno-builtin -fno-exceptions -fno-stack-protector \
	-nostdlib -nodefaultlibs \
	-std=gnu99 -ffreestanding  -Wall -Wextra -Iincludes

LDFLAGS = -ffreestanding -nostdlib -lgcc

############################## SRC #############################################

SRC = boot/boot.s                 \
	kernel/gdt_flush.s        \
	kernel/gdt.c              \
	kernel/idt_flush.s        \
	kernel/idt.c              \
	kernel/isr.s              \
	kernel/kernel.c           \
	kernel/keyboard_handler.c \
	kernel/keyboard.s         \
	kernel/terminal.c

COBJ = $(SRC:.c=.o)
OBJ = $(COBJ:.s=.o)

libkfs = libkfs/libkfs.a

############################## COLORS ##########################################

BY = "\033[33;1m"
BR = "\033[31;1m"
BG = "\033[32;1m"
BB = "\033[34;1m"
BM = "\033[35;1m"
BC = "\033[36;1m"
BW = "\033[37;1m"
Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
M = "\033[35m"
C = "\033[36m"
W = "\033[0m""\033[32;1m"
WR = "\033[0m""\033[31;5m"
WY = "\033[0m""\033[33;5m"
X = "\033[0m"

############################## BUILD ###########################################

all: art ${NAME}

libkfs/%:
	make -C libkfs CFLAGS+="$(CFLAGS)" AR=$(AR) CC+=$(CC) TARGET=../$@

${OBJ}: ${libkfs}

${NAME}: ${libkfs} ${OBJ}
	$(CC) -T linker.ld ${LDFLAGS} -o ${NAME} ${OBJ}  -Llibkfs -lkfs

############################## MORE ############################################

check:
	@$(GRUB)-file --is-x86-multiboot ${NAME} && echo ${B}[${NAME}] ${G}Multiboot OK!${X} || echo ${B}[${NAME}] ${R}NOT Multiboot${X}

iso: art ${NAME}
	$(shell printf 'menuentry "${NAME}" {\n\
	\tmultiboot /boot/${NAME}\n\
	}\n' > grub.cfg)
	/bin/mkdir -p isodir/boot/grub
	/bin/cp ${NAME} isodir/boot/${NAME}
	/bin/cp grub.cfg isodir/boot/grub/grub.cfg
	$(GRUB)-mkrescue -o ${NAME}.iso isodir

run: ${NAME}
	qemu-system-i386 -kernel ${NAME} -curses

debug: ${NAME}
	qemu-system-i386 -kernel ${NAME} -curses -s -S

clean:
	@echo ${R}Cleaning libkfs objs...${X}
	make -C libkfs fclean
	@echo ${R}Cleaning...${X}
	/bin/rm -f ${OBJ}
	/bin/rm -rf isodir
	/bin/rm -f grub.cfg
	/bin/rm -f ${NAME}.iso

fclean: clean
	@echo ${R}Cleaning libkfs...${X}
	make -C libkfs fclean
	@echo ${R}Cleaning"  "[${NAME}]...${X}
	/bin/rm -f ${NAME}

re:
	$(MAKE) fclean
	$(MAKE) all

############################## ART #############################################

art:
	@echo ${BB}
	@echo "     ,--.     .--."
	@echo "    /    \\\\"${WR}". ."${X}${BB}"/    \\"
	@echo "   /  /\\ / \" \\ /\\  \\"
	@echo "  / _/  {~~v~~}  \\_ \\"
	@echo " /     {   |   }     \\"
	@echo ";   /\\{    |    }/\\   \\"
	@echo "| _/  {    |    }  \\_  :"
	@echo "|     {    |    }      |  "${BG}" _  _______ ____"${BB}
	@echo "|    /{    |    }\\     | "${BG}" | |/ /  ___/ ___| "${BB}
	@echo "|   / {    |    } \\    | "${BG}" | ' /| |_  \___ \ "${BB}
	@echo "|  /  {    |    }  \\   | "${BG}" | . \|  _|  ___) |"${BB}
	@echo "|  \\  \\    |    /  /   |"${BG}"  |_|\_\_|   |____/ "${BB}
	@echo "|   \\  \\   |   /  /    |"
	@echo "\\    \\  \\  |  /  /     /"
	@echo " \\   /   ~~~~~   \\    /"${X}

.PHONY: all clean fclean re iso check art
