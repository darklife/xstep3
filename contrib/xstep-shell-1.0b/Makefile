# XSTEP 3.5 - Toolkit for X Window System
# Copyright (C) 1996-2000 by Marcelo Samsoniuk
# XSTEP-SHELL 0.14
# Copyright (C) 2001-2002 by Tito Ragusa
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

      CC  = gcc 
      CFLAGS  =  -Wall -pedantic -O3 -funroll-loops -fomit-frame-pointer -pipe
      CFLAGS += -I/usr/X11R6/include
      #CFLAGS += -I/usr/local/include -I../lib
      LDFLAGS  = -L/usr/X11R6/lib -L/usr/X11/lib -lX11 -lXext -L../lib -lxstep -lm
      
APPLICATIONS  =	xstep-shell



all: $(APPLICATIONS); strip $(APPLICATIONS)
     
install:
	cp $(APPLICATIONS) /usr/bin

clean:
	-rm -f $(APPLICATIONS)
uninstall:
	-for i in $(APPLICATIONS); do rm -f /usr/bin/$$i; done


























