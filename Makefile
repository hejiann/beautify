# Copyright (C) 2012 hejian <hejian.he@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC = gcc

GIMPTOOL = gimptool-2.0

GIMP_LIBS = `$(GIMPTOOL) --libs`
GIMP_CFLAGS = `$(GIMPTOOL) --cflags`

LIBS = $(GIMP_LIBS) -lm
CFLAGS = $(GIMP_CFLAGS)

GDK_PIXBUF_CSOURCE = gdk-pixbuf-csource

all: beautify rip-border skin-whitening

install: beautify rip-border skin-whitening
	$(GIMPTOOL) --install-bin beautify
	$(GIMPTOOL) --install-bin rip-border
	$(GIMPTOOL) --install-bin skin-whitening

beautify: beautify.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

beautify.o: beautify.c beautify-textures.h
	$(CC) $(CFLAGS) -c beautify.c -o beautify.o

beautify-textures.h: beautify-textures.list
	$(GDK_PIXBUF_CSOURCE) --raw --build-list `cat beautify-textures.list` > $(@F)

rip-border: rip-border.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

rip-border.o: rip-border.c rip-border-textures.h
	$(CC) $(CFLAGS) -c rip-border.c -o rip-border.o

rip-border-textures.h: rip-border-textures.list
	$(GDK_PIXBUF_CSOURCE) --raw --build-list `cat rip-border-textures.list` > $(@F)

skin-whitening: skin-whitening.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f *.o beautify rip-border skin-whitening

