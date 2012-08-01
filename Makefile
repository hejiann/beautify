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

all: beautify skin-whitening border

install: beautify skin-whitening border
	# need fix: --install-admin-bin has issue in x86_64,
	# it install the plug-in into /usr/lib/gimp/2.0/plug-ins/
	# but the correct directory is /usr/lib64/gimp/2.0/plug-ins/
	$(GIMPTOOL) --install-admin-bin beautify
	$(GIMPTOOL) --install-admin-bin skin-whitening
	$(GIMPTOOL) --install-admin-bin border
	ln -sf /usr/lib/gimp/2.0/plug-ins/beautify /usr/lib64/gimp/2.0/plug-ins/beautify
	ln -sf /usr/lib/gimp/2.0/plug-ins/skin-whitening /usr/lib64/gimp/2.0/plug-ins/skin-whitening
	ln -sf /usr/lib/gimp/2.0/plug-ins/border /usr/lib64/gimp/2.0/plug-ins/border

uninstall:
	$(GIMPTOOL) --uninstall-admin-bin beautify
	$(GIMPTOOL) --uninstall-admin-bin skin-whitening
	$(GIMPTOOL) --uninstall-admin-bin border
	$(GIMPTOOL) --uninstall-admin-bin rip-border
	$(GIMPTOOL) --uninstall-admin-bin texture-border
	rm -f /usr/lib64/gimp/2.0/plug-ins/beautify
	rm -f /usr/lib64/gimp/2.0/plug-ins/skin-whitening
	rm -f /usr/lib64/gimp/2.0/plug-ins/border
	rm -f /usr/lib64/gimp/2.0/plug-ins/rip-border
	rm -f /usr/lib64/gimp/2.0/plug-ins/texture-border

userinstall: beautify skin-whitening border
	$(GIMPTOOL) --install-bin beautify
	$(GIMPTOOL) --install-bin skin-whitening
	$(GIMPTOOL) --uninstall-bin rip-border
	$(GIMPTOOL) --uninstall-bin texture-border
	$(GIMPTOOL) --install-bin border

useruninstall:
	$(GIMPTOOL) --uninstall-bin beautify
	$(GIMPTOOL) --uninstall-bin skin-whitening
	$(GIMPTOOL) --uninstall-bin border
	$(GIMPTOOL) --uninstall-bin rip-border
	$(GIMPTOOL) --uninstall-bin texture-border

beautify: beautify.o beautify-effect.o
	$(CC) -o $@ $^ $(LIBS)

beautify.o: beautify.c
	$(CC) $(CFLAGS) -c beautify.c -o beautify.o

beautify-effect.o: beautify-effect.c beautify-textures.h
	$(CC) $(CFLAGS) -c beautify-effect.c -o beautify-effect.o

beautify-textures.h: beautify-textures.list
	$(GDK_PIXBUF_CSOURCE) --raw --build-list `cat beautify-textures.list` > $(@F)

skin-whitening: skin-whitening.o skin-whitening-effect.o
	$(CC) -o $@ $^ $(LIBS)

skin-whitening.o: skin-whitening.c skin-whitening-images.h
	$(CC) $(CFLAGS) -c skin-whitening.c -o skin-whitening.o

skin-whitening-images.h: skin-whitening-images.list
	$(GDK_PIXBUF_CSOURCE) --raw --build-list `cat skin-whitening-images.list` > $(@F)

skin-whitening-effect.o: skin-whitening-effect.c skin-whitening-effect.h
	$(CC) $(CFLAGS) -c skin-whitening-effect.c -o skin-whitening-effect.o

border: border.o
	$(CC) -o $@ $^ $(LIBS)

border.o: border.c border-textures.h
	$(CC) $(CFLAGS) -c border.c -o border.o

border-textures.h: border-textures.list
	$(GDK_PIXBUF_CSOURCE) --raw --build-list `cat border-textures.list` > $(@F)

clean:
	rm -f *.o beautify beautify-textures.h skin-whitening skin-whitening-images.h border border-textures.h rip-border rip-border-textures.h texture-border texture-border-textures.h

