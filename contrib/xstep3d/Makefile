# XSTEP3D - 3D extension for XSTEP
# Copyright (C) 2000 by Marcelo Samsoniuk
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

 CFLAGS = -Wall -O3 -funroll-loops -fomit-frame-pointer -pipe -ffast-math
LDFLAGS = -L/usr/local/lib -lxstep -L/usr/X11R6/lib -lX11 -lXext -lm 

all: xstep3d circle

clean:
	-rm xstep3d circle
install: all
	cp xstep3d /usr/local/bin
uninstall:
	-rm /usr/local/bin/xstep3d
