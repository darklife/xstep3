# XSTEP 3.5.1 - Toolkit for X Window System
# Copyright (C) 1996-2003 by Marcelo Samsoniuk and contributors
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc., 59
# Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
# debug options:
# 
# 	-DSHOWKEYSYM		show X11 KEYSYMs
# 	-DSHOWEVENT		show X11 events
# 	-DSHOWCACHEEXPOSE	show cached expose requests
# 	-DSHOWFLUSHEXPOSE	show flushed expose requests
# 	-DKEYPADBUG		keypad bug, aways enabled!
#	-DTREEWINDOW		display the widget tree

       CC = gcc
       AR = ar rcs
       LD = ld -shared
       RM = -rm -f
 LDCONFIG = /sbin/ldconfig
    CCOPS = -Wall -O3 -funroll-loops -fomit-frame-pointer
   INCDIR = -I/usr/X11R6/include -I/usr/X11/include
   LIBDIR = -L/usr/X11R6/lib -L/usr/X11/lib
     LIBS = -lX11
     DEFS = -DKEYPADBUG
   CFLAGS = $(CCOPS) $(INCDIR) $(DEFS)
  LDFLAGS = -shared $(LIBDIR) $(LIBS)
     OBJS = util.o 		\
            window.o 		\
            button.o 		\
            radio.o 		\
            check.o 		\
            edit.o 		\
            label.o   		\
            box.o 		\
            scroll.o 		\
            main.o 		\
            scroll_buttons.o 	\
            scroll_lists.o 	\
            menu.o 		\
            popup.o		\
            group.o		\
	    xyscroll.o		\
	    scan.o		\
	    fine_tune.o
  LIBRARY = xstep
  LIBNAME = libxstep
  VERSION = 3
  RELEASE = 5.1
   PREFIX = /usr/local
  INSTALL = install -c -o root -m 644 
 BINSTALL = install -c -o root -m 755 -s
  TARGETS = $(LIBNAME).so.$(VERSION).$(RELEASE) $(LIBNAME).a

all:: $(TARGETS)
	@echo "wow! done! type 'make install' to install."

$(LIBNAME).so.$(VERSION).$(RELEASE):: $(OBJS)
	$(LD) -soname $(LIBNAME).so.$(VERSION) $(LDFLAGS) $(OBJS) -o $(LIBNAME).so.$(VERSION).$(RELEASE)

$(LIBNAME).a:: $(OBJS)
	$(AR) $(LIBNAME).a $(OBJS)

install: $(TARGETS)
	
	$(INSTALL)  $(LIBRARY).h $(PREFIX)/include
	$(BINSTALL) $(LIBNAME).a $(PREFIX)/lib
	$(BINSTALL) $(LIBNAME).so.$(VERSION).$(RELEASE) $(PREFIX)/lib
	cd $(PREFIX)/lib && ln -sf $(LIBNAME).so.$(VERSION).$(RELEASE) $(LIBNAME).so.$(VERSION)
	cd $(PREFIX)/lib && ln -sf $(LIBNAME).so.$(VERSION).$(RELEASE) $(LIBNAME).so
	uname -s | awk '/Linux|NetBSD/ { system("grep $(PREFIX)/lib /etc/ld.so.conf || echo $(PREFIX)/lib >> /etc/ld.so.conf; $(LDCONFIG)") }'
	uname -s | awk '/FreeBSD/ { system("$(LDCONFIG) -m $(PREFIX)/lib/")}'

uninstall:
	$(RM) $(PREFIX)/lib/$(LIBNAME).so.$(VERSION).$(RELEASE)
	$(RM) $(PREFIX)/lib/$(LIBNAME).so.$(VERSION)
	$(RM) $(PREFIX)/lib/$(LIBNAME).so
	$(RM) $(PREFIX)/include/$(LIBRARY).h

clean: 
	$(RM) $(OBJS) $(TARGETS) core
