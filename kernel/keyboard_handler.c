/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 23:57:13 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 00:49:45 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "keyboard.h"
#include "terminal.h"   /* TMP: for terminal_putchar */

static const unsigned char	keyboard_map[128] =
{
	0,							// Undefined
	0,							// Escape
	'1','2','3','4','5','6','7','8','9','0','-','=',	//
	'\b',							// Backspace
	'\t',							// Tab
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',	//
	0,							// Ctrl
	'a','s','d','f','g','h','j','k','l',';','\'','`',	//
	0,							// Left shift
	'\\','z','x','c','v','b','n','m',',','.','/',		//
	0,							// Right shift
	'*',							// (Keypad *)
	0,							// Left Alt
	' ',							// Space
	0,							// Caps lock
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				// <F1> -> <F10>
	0,							// Num lock
	0,							// Scroll Lock
	0,							// Home key
	0,							// Up Arrow
	0,							// Page Up
	'-',							//
	0,							// Left Arrow
	0,							//
	0,							// Right Arrow
	'+',							//
	0,							// End key
	0,							// Down Arrow
	0,							// Page Down
	0,							// Insert Key
	0,							// Delete Key
	0, 0, 0,						//
	0,							// <F11>
	0,							// <F12>
	0,                                                      // ... Undefined
};

static const unsigned char	shift_map[] =
{
	0,
	0,
	'!','@','#','$','%','^','&','*','(',')','_','+',
	'\b',
	'\t',
	'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
	0,
	'A','S','D','F','G','H','J','K','L',':','\"','~',
	0,
	'|','Z','X','C','V','B','N','M','<','>','?'
};

#define STATUS_ALT		(1 << 0) /* keycode: 57    */
#define STATUS_CONTROL		(1 << 1) /* keycode: 30    */
#define STATUS_SHIFT		(1 << 2) /* keycode: 43 55 */
#define STATUS_CAPSLOCK		(1 << 3) /* keycode: 59    */
#define STATUS_NUMLOCK		(1 << 4) /* keycode: 70    */
#define STATUS_SCROLLLOCK	(1 << 5) /* keycode: 71    */

static const u8			keystatus_map[128] =
{
	[57] = STATUS_ALT,
	[30] = STATUS_CONTROL,
	[43] = STATUS_SHIFT,
	[55] = STATUS_SHIFT,
	[59] = STATUS_CAPSLOCK,
	[70] = STATUS_NUMLOCK,
	[71] = STATUS_SCROLLLOCK
};

static u8			keystatus;

static void	set_keystatus(const u8 keycode)
{
	const u8	masked_keycode = keycode & ~KEYCODE_RELEASE;
	const u8	status_key     = keystatus_map[masked_keycode];

	if (keycode & KEYCODE_RELEASE)
		keystatus &= ~status_key;
	else
		keystatus |= status_key;
}

void		keyboard_handler(void)
{
	write_port(PIC_BASE_PORT, PIC_EOI); /* useless without idt */

	const u8	status = read_port(KEYBOARD_STATUS_PORT);

	if (status & 0x01)
	{
		const u8	keycode = read_port(KEYBOARD_DATA_PORT);
		char		key;

		set_keystatus(keycode);

		if (keycode >= 128) return;

		if (keystatus & STATUS_SHIFT && keycode < sizeof(shift_map))
			key = shift_map[keycode];
		else
			key = keyboard_map[keycode];

		terminal_putchar(key);
	}
}

void		keyboard_init(void)
{
	for (;;)
		keyboard_handler();
}
