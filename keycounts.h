#ifndef KEYCOUNTS_H
#define KEYCOUNTS_H

#include <linux/input.h>
#include <stdbool.h>

#define MOD_SHIFT 1
#define MOD_ALT 2
#define MOD_CTRL 4
#define MOD_META 8
#define MOD_ALL (MOD_SHIFT | MOD_ALT | MOD_CTRL | MOD_META)

#define PREFIX "/usr"

extern const char *map[KEY_CNT];
extern int counts[KEY_CNT*MOD_ALL];

void report_counts(const char *datafile);
void report_key(int code);

void load_counts(const char *datafile);
void save_counts(const char *datafile);

int set_modifiers(int modifiers, int code);
int clear_modifiers(int modifiers, int code);
bool is_modifier(int code);

int record(const char *datafile, bool verbose);

void merge_counts(const char *datafile, const char *newfile);

void print_usage(void);

void output_image(const char *datafile, const char *layout);

#endif
