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
#include <string.h>

#include "keycounts.h"

int counts[KEY_CNT*MOD_ALL];

int main(int argc, char *argv[])
{
	memset(counts, 0, sizeof(counts));

	if(argc == 1){
		print_usage();
		return 0;
	}

	const char *datafile = NULL;
	const char *layout = NULL;
	bool verbose = false;

	for(int i=2; i<argc; i++){
		if(!strcmp(argv[i], "-f")){
			if(i < argc-1){
				datafile = argv[i+1];
				i++;
			}else{
				fprintf(stderr, "Error: '-f' requires a filename.\n");
				exit(1);
			}
		}else if(!strcmp(argv[i], "-l")){
			if(i < argc-1){
				layout = argv[i+1];
				i++;
			}else{
				fprintf(stderr, "Error: '-l' requires a layout.\n");
				exit(1);
			}
		}else if(!strcmp(argv[i], "-v")){
			verbose = true;
		}
	}
	if(!datafile) datafile = "/var/lib/keycounts.dat";
	if(!layout) layout = "uk";

	if(!strcmp(argv[1], "record")){
		printf("Using data file %s\n", datafile);
		record(datafile, verbose);
	}else if(!strcmp(argv[1], "report")){
		report_counts(datafile);
	}else if(!strcmp(argv[1], "image")){
		if(strchr(layout, '/')){
			fprintf(stderr, "Error: Keyboard layout cannot contain '/'.\n");
			return 1;
		}
		output_image(datafile, layout);
	}else{
		print_usage();
	}

	return 0;
}
