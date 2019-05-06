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
	-std=gnu99 -ffreestanding  -Wall -Wextra

LDFLAGS = -ffreestanding  -nostdlib -lgcc

############################## SRC #############################################

SRC = boot/boot.s \
	kernel/kernel.c \
	kernel/keyboard_handler.c \
	kernel/ports.s \
	kernel/interrupts.s \
	kernel/init_keyboard.c \
	kernel/init_idt.c \
	kernel/keyboard_loop.s #tmp

COBJ = $(SRC:.c=.o)
OBJ = $(COBJ:.s=.o)

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

all: dragon ${NAME}

${NAME}: ${OBJ}
	i686-elf-gcc -T linker.ld ${LDFLAGS} -o ${NAME} ${OBJ}

############################## MORE ############################################

check:
	@grub-file --is-x86-multiboot ${NAME} && echo ${B}[${NAME}] ${G}Multiboot OK!${X} || echo ${B}[${NAME}] ${R}NOT Multiboot${X}

iso: unicorn ${NAME}
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
	@echo ${R}Cleaning...${X}
	/bin/rm -f ${OBJ}
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

unicorn:
	@echo "                              ,|"
	@echo "                             //|                              ,|"
	@echo "                           //,/                             -~ |"
	@echo "                         // / |                         _-~   /  ,"
	@echo "                       /'/ / /                       _-~   _/_-~ |"
	@echo "                      ( ( / /'                   _ -~     _-~ ,/'"
	@echo "                       \\~\\/'/|             __--~~__--\\ _-~  _/,"
	@echo "               "${BB}","${BC}","${BG}")"${BY}")"${BR}")"${BM}")"${BB}")"${BC}")"${BG}")"${BY}";"${BR}", "${X}"\\/~-_     __--~~  --~~  __/~  _-~ /"
	@echo "            __"${BB}")"${BC}")"${BG}")"${BY}")"${BR}")"${BM}")"${BB}")"${BC}")"${BG}")"${BY}")"${BR}")"${BM}")"${BB}")"${BC}")"${BG}";"${BW}",>/\\   /        __--~~  \\-~~ _-~"
	@echo "           "${BW}"-\\\\"${BB}"("${BC}"("${BG}"("${BY}"("${BR}"("${BM}" "${BB}"'"${BC}"'"${BG}"'"${BY}"'"${BR}"("${BM}"("${BB}"("${BC}"("${BG}"("${BY}"("${BR}"("${BM}"( "${BW}">~\\/     --~~   __--~' _-~ ~|"
	@echo "  "${BW}""${BM}"-"${BW}"-"${BM}"="${BW}"=/"${BM}"/"${BW}"/"${BM}"/"${BW}"/"${BM}"/"${BB}"("${BC}"("${BG}""${BY}"'"${BR}"'  "${BW}".     "${BB}"\`"${BC}")"${BG}")"${BY}")"${BR}")"${BM}")"${BB}")"${BC}", "${BW}"/     ___---~~  ~~\\~~__--~"
	@echo "          "${BB}")"${BG}")"${BY}"| "${BW}"(@    ;-.    "${BR}"("${BM}"("${BB}"("${BC}"("${BG}"("${BW}"/           __--~~~'~~/"
	@echo "          "${BG}"( "${BW}"\`|    /  )      "${BB}")"${BC}")"${BG}")"${BW}"/      ~~~~~__\\__---~~/ "${BM}","${BB}"("${BC}"("${BG}"("${BY}"("${BR}"("${BB}"("${BC}"("
	@echo "             "${BW}"|   |   |       "${BG}"("${BW}"/      ---~~~/__-----~~\\\\"${BM}")"${BB}")"${BC}")"${BG}")"${BY}")"${BR}")"${BM}")"${BB}")"${BC}")"${BG}")"${BY}")"
	@echo "             "${BW}"o_);   ;        /      ----~~/           \\\\"${BC}"("${BG}"("${BY}"("${BR}"("${BM}"("${BB}"("${BC}"("${BG}"("${BY}"("${BR}"("${BM}"("
	@echo "                   "${BW}";        (      ---~~/         \`:::|     "${BC}")"${BG}")"${BY}")"${BR}")"${BM}")"${BB}")"${BC}";"${BG}","${BY}"."
	@echo "                  "${BW}"|   _      \`----~~~~'      /      \`:|      "${BY}"("${BR}"("${BM}"("${BB}"("${BC}"("${BG}"("${BY}"("${BR}"("${BM}"("${BB}"("
	@echo "            "${BW}"______/\\/~    |                 /        /         "${BB}")"${BC}")"${BG}")"${BY}")"${BR}")"${BM}")"${BB}")"${BC}")"${BG}")"
	@echo "          "${BW}"/~;;.____/;;'  /          ___----(   \`;;;/                "${BB}")"${BC}")"${BG}")"${BY}")"
	@echo "         "${BW}"/ //  _;______;'------~~~~~    |;;/\\    /                    "${BY}")"${BR}")"
	@echo "        "${BW}"//  | |                        /  |  \\;;,\\"
	@echo "       (<_  | ;                      /',/-----'  _>"
	@echo "        \\_| ||_                     //~;~~~~~~~~~"
	@echo "            \`\\_|                   (,~~"
	@echo "                                    \\~\\"
	@echo "                                     ~~"
	@echo ""${X}

dragon:
	@echo ""
	@echo ""${X}${BG}
	@echo "	                         "${BB}"     _"${BG}   "K"${R}"A"${BY}"G"${BG}"R"${BY}"U"${BR}"M"${BG}
	@echo "	                          "${BB}" "${BW}"=="${BB}"(W"${BW}"{==========-   "${BG}"   /="${BW}"==-"
	@echo "	                           "${BB}"  ||  (.--.)        "${BG}" /="${BW}"==-"${BG}"_---~~~~~~~----__"
	@echo "	                            "${BB}" | \\_,|"${WR}"**"${W}${BB}"|,__   "${BG}"   |="${BW}"==-"${BG}"~___            _,_'"${BW}"'>"
	@echo "	                -=="${BG}"\\\\\\\\      "${BB}"  \`\\ ' \`--'   ),  "${BG}"  \`//~\\\\\\\\   ~~~~\`--._.--"${BW}">"${BG}""
	@echo "	            ______"${BW}"-=="${BG}"|       "${BB}" /\`\\_. .__/\\ \\   "${BG}" | |  \\\\\\\\          _-~\`"
	@echo "	      __--~~~  ,-/"${BW}"-=="${BG}"\\\\\\\\    "${BB}"  (   | .  "${BW}"|~~~~|   "${BG}"| |   \`\\       ,'"
	@echo "	   _-~       /'    |  \\\\\\\\   "${BB}"  )__/==0==-"${BW}"\\\\42/ "${BG}"  / /      \\     /"
	@echo "	 .'        /       |   \\\\\\\\    "${BB}"  /~\\___/~~"${BW}"\\/ "${BG}" /' /        \\   /"
	@echo "	/  ____  /         |    \\\`\\.__/-~~   \\  |_/'  /          \\/'"
	@echo "	"${BW}"V"${BG}"-'~   "${BW}"v"${BG}"~~~~---__  |     ~-/~         ( )   /'        _--"${BW}"v"${BG}"\`"
	@echo "	                 \\_|      /        _) | ;  ),   __--~~"
	@echo "	                   '~~--_/      _-~/- |/ \\   '-~ \\"
	@echo "	                  "${BW}"{\\\\"${BG}"__~~"${BW}"_/}"${BG}"    / \\\\\\\\_>-|)<__\\      \\"
	@echo "	                  /'   "${BW}"(_/"${BG}"  _-~  | |__>--<__|      |"
	@echo "	                 |   _/"${BR}"-"${BG}"))-~     | |__>--<__|      |"
	@echo "	                 / /~ ,_/       / /__>---<__/      |"
	@echo "	                o-o _//        /-~_>---<__-~      /"
	@echo "	                (^(~          /~_>---<__-      _-~"
	@echo "	               "${WR}","${WY}"/"${WR}"|"${W}"           /__>--<__/     _-~"
	@echo "	            "${WR}",//"${WY}"("${WR}"'("${W}"          |__>--<__|     /                  .--_"
	@echo "	           "${WR}"( "${WY}"( '"${WR}"))"${W}"          |__>--<__|    |                 /' _-_~\\"
	@echo "	          "${WR}"\`-))"${WY}" )"${WR}") ("${W}"         |__>--<__|    |               /'  /   ~\\\`\\"
	@echo "	       "${WR}",/,'"${WY}"//"${WR}"( ("${W}"             \\__>--<__\\    \\            /'  //      ||"
	@echo "	  "${WR}"(( ,( ( "${WY}"((, "${WR}"))"${W}"              ~-__>--<_~-_  ~--__---~'/'/  /'       "${BW}"VV"${BG}
	@echo "	 "${WR}"\\ \`~/  "${WY}")\` )"${WR}" ,/|"${W}"                 ~-_~>--<_/-__      __-~ _/"
	@echo "	 "${WR}"._-~//( "${WY}")/ "${WR}")) \`"${W}"                    ~~-'_/_/ /~~~~~__--~"
	@echo "	  "${WR}" '"${WY}"( ')"${WR}"/ ,)("${W}"                              ~~~~~~~~"
	@echo "	 "${WR}"   ) '( (/"${W}
	@echo ""${X}

.PHONY: all clean fclean re iso check dragon unicorn
