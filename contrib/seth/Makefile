
# XSTEP 3.4 - Toolkit for X-Window System
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
      CFLAGS  = -Wall -O3 -funroll-loops -fomit-frame-pointer -pipe 
      CFLAGS += -I/usr/X11R6/include -I/usr/X11/include
      CFLAGS += -I/usr/local/include
     LDFLAGS += -L/usr/X11R6/lib -L/usr/X11/lib -lX11
     LDFLAGS += -L/usr/local/lib -lxstep

	 CXX  = g++ $(CFLAGS)

APPLICATIONS  = xstepseth

all: $(APPLICATIONS)

xstepseth: seth.o
	$(CC) $(CFLAGS) seth.o -o xstepseth $(LDFLAGS)

install: $(APPLICATIONS)
	cp $(APPLICATIONS) /usr/local/bin

clean:
	-rm $(APPLICATIONS) seth.o
