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
#include <stdlib.h>

#include "keycounts.h"

void merge_counts(const char *datafile, const char *newfile)
{
	int new_counts[KEY_CNT*MOD_ALL];

	load_counts(datafile);

	FILE *fptr = fopen(newfile, "rb");
	if(fptr){
		if(fread(new_counts, 1, sizeof(new_counts), fptr) != sizeof(new_counts)){
			fprintf(stderr, "File %s is not the correct size\n", newfile);
			fclose(fptr);
			exit(1);
		}
		fclose(fptr);

		for(int i=0; i<KEY_CNT*MOD_ALL; i++){
			counts[i] += new_counts[i];
		}
	}

	save_counts(datafile);
}
