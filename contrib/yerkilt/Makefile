# Yerkilt v0.2 - A wee program to generate Scottish Tartans
# Copyright (C) 2000 by Marcello Barisonzi
#
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

          CC = gcc
          RM = -rm -f
       CCOPS = -Wall -O3 -funroll-loops -fomit-frame-pointer -pipe 
      INCDIR = -I/usr/X11R6/include -I/usr/X11/include -I/usr/local/include
      CFLAGS = $(CCOPS) $(INCDIR)
     LDFLAGS = -L/usr/X11R6/lib -L/usr/X11/lib -lX11 -L/usr/local/lib -lxstep

	 CXX  = g++ $(CFLAGS)

 APPLICATION  = yerkilt           
        OBJS  = tartan.o color_change.o saveload.o

all: $(APPLICATION)

$(APPLICATION): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(APPLICATION) $(LDFLAGS)

install: all
	cp yerkilt /usr/local/bin

clean:
	$(RM) $(APPLICATION) $(OBJS) core

