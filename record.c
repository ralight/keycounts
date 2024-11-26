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
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>

#include "keycounts.h"

#define MAX_EVENTS 100

extern const char *map[KEY_CNT];
static int modifiers = 0;
static int run = 1;


static void sigh(int signal)
{
	run = 0;
}


static void add_to_epoll(int epollfd, const char *path)
{
	int fd = open(path, O_RDONLY);
	if(fd < 0){
		fprintf(stderr, "Unable to open %s\n", path);
		exit(1);
	}
	printf("Opened %s\n", path);

    struct epoll_event ev;
	memset(&ev, 0, sizeof(struct epoll_event));
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}


static void open_input_devices_proc(int epollfd)
{
	char buf[1024];
	FILE *fptr;
	size_t len;
	bool have_kbd = false;

	fptr = fopen("/proc/bus/input/devices", "rb");
	if(!fptr){
		fprintf(stderr, "Unable to open /proc/bus/input/devices\n");
		exit(1);
	}

	len = strlen("H: Handlers=");
	while(!feof(fptr) && fgets(buf, sizeof(buf), fptr)){
		if(!strncmp(buf, "H: Handlers=", len)){
			char *token = NULL;
			bool kbd = false;
			char *event = NULL;

			buf[strlen(buf)-1] = '\0';
			token = strtok(buf+len, " ");
			while(token){
				if(!strcmp(token, "kbd")){
					kbd = true;
				}else if(!strncmp(token, "event", strlen("event"))){
					event = strdup(token);
				}
				token = strtok(NULL, " ");
			}
			if(kbd && event != NULL){
				snprintf(buf, sizeof(buf), "/dev/input/%s", event);
				add_to_epoll(epollfd, buf);
				have_kbd = true;
			}
		}
	}
	fclose(fptr);

	if(!have_kbd){
		fprintf(stderr, "No keyboards found\n");
		exit(1);
	}
}


static void open_input_devices_dev(int epollfd)
{
	char buf[1024];
	bool have_kbd = false;
	DIR *dir;
	struct dirent *ent;

	dir = opendir("/dev/input/by-path/");
	if(!dir){
		fprintf(stderr, "Unable to open /dev/input/by-path/\n");
		exit(1);
	}

	while((ent = readdir(dir))){
		char *s = strstr(ent->d_name, "-kbd");
		if(s && strlen(s) == strlen("-kbd")){
			snprintf(buf, sizeof(buf), "/dev/input/by-path/%s", ent->d_name);
			add_to_epoll(epollfd, buf);
			have_kbd = true;

		}
	}
	closedir(dir);

	if(!have_kbd){
		fprintf(stderr, "No keyboards found\n");
		exit(1);
	}
}


static void handle_read(const char *datafile, int fd, bool verbose)
{
	struct input_event ev;

	if(read(fd, &ev, sizeof(ev)) != sizeof(ev)){
		fprintf(stderr, "Error reading from input, exiting.\n");
		save_counts(datafile);
		exit(1);
	}
	if(ev.type != EV_KEY){
		return;
	}
	if(ev.value == 1 && ev.code < KEY_CNT){
		modifiers = set_modifiers(modifiers, ev.code);
		if(is_modifier(ev.code)){
			counts[ev.code]++;
			if(verbose) report_key(ev.code);
		}else{
			counts[ev.code + KEY_MAX*modifiers]++;
			if(verbose) report_key(ev.code + KEY_MAX*modifiers);
		}
	}else if(ev.value == 0 && ev.code < KEY_CNT){
		modifiers = clear_modifiers(modifiers, ev.code);
	}
}


static void periodic_save(const char *datafile)
{
	static time_t last_save = 0;
	time_t now = time(NULL);

	if(last_save + 60 < now){
		save_counts(datafile);
		last_save = now;
	}
}


static void record_loop(const char *datafile, bool verbose)
{
	int epollfd;
	struct epoll_event ep_events[MAX_EVENTS];

	epollfd = epoll_create(MAX_EVENTS);
	if(getenv("SNAP_NAME")){
		open_input_devices_dev(epollfd);
	}else{
		open_input_devices_proc(epollfd);
	}

	while(run){
		int event_count = epoll_wait(epollfd, ep_events, MAX_EVENTS, 10000);
		if(event_count > 0){
			for(int i=0; i<event_count; i++){
				if(ep_events[i].events & EPOLLIN){
					handle_read(datafile, ep_events[i].data.fd, verbose);
				}
			}
		}
		periodic_save(datafile);
	}

	close(epollfd);
}


int record(const char *datafile, bool verbose)
{
	signal(SIGTERM, sigh);
	signal(SIGINT, sigh);

	load_counts(datafile);
	record_loop(datafile, verbose);
	save_counts(datafile);

	return 0;
}
