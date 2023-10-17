OBJS=keycounts.o image.o logfile.o map.o merge.o modifiers.o record.o report.o usage.o

CC=gcc
CXX=g++
CFLAGS=-Wall -ggdb -O3
LDFLAGS=-lm
prefix=/usr

all : keycounts

$(OBJS) : %.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

keycounts : $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

install : keycounts
	install -d $(DESTDIR)$(prefix)/bin
	install keycounts $(DESTDIR)$(prefix)/bin

	install -d $(DESTDIR)$(prefix)/share/keycounts
	install keyboards/uk-header.svg $(DESTDIR)$(prefix)/share/keycounts/
	install keyboards/uk-footer.svg $(DESTDIR)$(prefix)/share/keycounts/

memtest : keycounts
	valgrind -v --show-reachable=yes --leak-check=full ./$^

clean :
	-rm -f keycounts *.o
