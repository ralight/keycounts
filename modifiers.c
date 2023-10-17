/*
 * Copyright (C) 2023 Roger Light <roger@atchoo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <linux/input.h>

#include "keycounts.h"

int set_modifiers(int modifiers, int code)
{
	if(code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT){
		return modifiers | MOD_SHIFT;
	}else if(code == KEY_LEFTALT || code == KEY_RIGHTALT){
		return modifiers | MOD_ALT;
	}else if(code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL){
		return modifiers | MOD_CTRL;
	}else if(code == KEY_LEFTMETA || code == KEY_RIGHTMETA){
		return modifiers | MOD_META;
	}else{
		return modifiers;
	}
}


int clear_modifiers(int modifiers, int code)
{
	if(code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT){
		return modifiers & ~MOD_SHIFT;
	}else if(code == KEY_LEFTALT || code == KEY_RIGHTALT){
		return modifiers & ~MOD_ALT;
	}else if(code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL){
		return modifiers & ~MOD_CTRL;
	}else if(code == KEY_LEFTMETA || code == KEY_RIGHTMETA){
		return modifiers & ~MOD_META;
	}else{
		return modifiers;
	}
}


bool is_modifier(int code)
{
	if(code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT){
		return true;
	}else if(code == KEY_LEFTALT || code == KEY_RIGHTALT){
		return true;
	}else if(code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL){
		return true;
	}else if(code == KEY_LEFTMETA || code == KEY_RIGHTMETA){
		return true;
	}
	return false;
}
