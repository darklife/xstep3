
# XSTEP 3.3 - Toolkit for X-Window System
# Copyright (C) 1996,1997,1998,1999 by Marcelo Samsoniuk
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
# 
# Please read the COPYING and README file!!!

          CC  = gcc
      CFLAGS  = -Wall -O3 -fomit-frame-pointer -funroll-loops -pipe
      CFLAGS += -I/usr/X11R6/include -I/usr/X11/include
      CFLAGS += -I/usr/local/include
      CFLAGS += -I/usr/local/mysql/include
      CFLAGS += -L/usr/X11R6/lib -L/usr/X11/lib -lX11
      CFLAGS += -L/usr/local/lib -lxstep

all: academia


academia:
	gcc $(CFLAGS) academia.c -o academia -L/usr/local/mysql/lib -lmysqlclient

clean:
	-rm -f academia

install: academia
	cp academia /usr/local/bin

uninstall:
	rm /usr/local/bin/academia

