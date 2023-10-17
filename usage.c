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

#include <stdio.h>

void print_usage(void)
{
	printf("keycounts - record keyboard keypress statistics\n");
	printf("           stores individual keypress counts, but no ordering or timing information\n\n");
	printf("keycounts record [-f data-file] [-v]         # record keypress statistics to data-file, or the\n");
	printf("                                             # default /var/lib/keycounts.dat. Requires root access\n");
	printf("                                             # -v prints key presses as they are made, useful for\n");
	printf("                                             # figuring out which key is which.\n");
	printf("keycounts report [-f data-file]              # report keypress statistics from data-file\n");
	printf("keycounts image  [-f data-file] [-l layout]  # print svg keyboard representation of keypresses to stdout\n");
}
