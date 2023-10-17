/*
 * Copyright (C) 2023 Roger Light <shift@atchoo.org>
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
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "keycounts.h"

static const char* colormap[] = {
	"#ffffff",
	"#fafea6",
	"#fee79f",
	"#fed968",
	"#feba68",
	"#fea437",
	"#fe8d37",
	"#ff6e00",
	"#ff5000",
	"#ff1400",
};

struct modifier_outputs{
	int range_start;
	const char *prefix;
};

struct modifier_outputs mod_outputs[] =
{
	{0, ""},
	{MOD_SHIFT, "shift_"},
	{MOD_ALT, "alt_"},
	{MOD_SHIFT+MOD_ALT, "shift_alt_"},
	{MOD_CTRL, "ctrl_"},
	{MOD_CTRL+MOD_SHIFT, "ctrl_shift_"},
	{MOD_CTRL+MOD_ALT, "ctrl_alt_"},
	{MOD_CTRL+MOD_ALT+MOD_SHIFT, "ctrl_alt_shift_"},
	{MOD_META, "meta_"},
};


static void print_key_style(int code, double mult, const char *modifier)
{
	if(counts[code] > 0){
		int v = (int)(round(log((double)counts[code]+1.0)*mult)) - 1;
		printf(".key_%s%s { fill:%s } /* %d */\n", modifier, map[code%KEY_MAX], colormap[v], counts[code]);
	}
}


static FILE *open_layout_file(const char *keyboard, const char *label)
{
	char file[1024];
	const char *prefix = getenv("SNAP");
	if(!prefix) prefix = "";
	snprintf(file, sizeof(file), "%s/usr/share/keycounts/%s-%s.svg", prefix, keyboard, label);
	return fopen(file, "rb");
}


static void print_file(FILE *fptr)
{
	if(!fptr) return;

	fseek(fptr, 0, SEEK_END);
	long len = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);
	char *data = calloc(1, len+1);
	if(!data) return;
	if(fread(data, 1, len, fptr) != len){
		fprintf(stderr, "Error: Unable to load file.\n");
		exit(1);
	}
	printf("%s", data);
	free(data);
}


void output_image(const char *datafile, const char *layout)
{
	FILE *header_file;
	FILE *footer_file;

	header_file = open_layout_file(layout, "header");
	if(!header_file){
		fprintf(stderr, "Error: Unable to open %s-header.svg file.\n", layout);
		return;
	}
	footer_file = open_layout_file(layout, "footer");
	if(!footer_file){
		fprintf(stderr, "Error: Unable to open %s-footer.svg file.\n", layout);
		fclose(header_file);
		return;
	}

	load_counts(datafile);

	int count_max = 0;
	double mult;

	for(int i=0; i<KEY_CNT*MOD_ALL; i++){
		if(counts[i] > count_max) count_max = counts[i];
	}

	mult = 10.0/log(count_max+1);

	print_file(header_file);
	fclose(header_file);

	for(int i=0; i<sizeof(mod_outputs)/sizeof(struct modifier_outputs); i++){
		for(int j=mod_outputs[i].range_start*KEY_CNT; j<(mod_outputs[i].range_start+1)*KEY_CNT; j++){
			print_key_style(j, mult, mod_outputs[i].prefix);
		}
	}

	print_file(footer_file);
	fclose(footer_file);
}
