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
#include <stdio.h>

#include "keycounts.h"

void report_key(int code)
{
	if(!is_modifier(code%KEY_MAX)){
		int modifiers = code/KEY_MAX;
		if(modifiers & MOD_SHIFT){
			printf("SHIFT+");
		}
		if(modifiers & MOD_ALT){
			printf("ALT+");
		}
		if(modifiers & MOD_CTRL){
			printf("CTRL+");
		}
		if(modifiers & MOD_META){
			printf("META+");
		}
	}
	printf("%s\n", map[code%KEY_MAX]);
	fflush(stdout);
}


void report_counts(const char *datafile)
{
	load_counts(datafile);

	for(int i=0; i<KEY_CNT*MOD_ALL; i++){
		if(counts[i] > 0){
			printf("%d ",  counts[i]);
			report_key(i);
		}
	}
}
