
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
      CFLAGS += -L/usr/X11R6/lib -L/usr/X11/lib -lX11
      CFLAGS += -L/usr/local/lib -lxstep

	 CXX  = g++ $(CFLAGS)

APPLICATIONS  = box_create 		\
		box_create_xpm		\
		button_create		\
		button_create_xpm	\
		animation		\
		check_and_radio_create	\
		edit_create		\
		label_create		\
		label_create_advanced	\
		mscroll_create		\
		popup_create		\
		resizable_window	\
		scroll_create		\
		window_create		\
		window_gc		\
		dialog_create		\
		group			\
		group_gc		\
		xyscroll_create		\
		xscroll_create         	\
		yscroll_create         	\
		window_object		\
		finetune_create		\
		file_sel

XSTEP=/usr/local/lib/libxstep.so

all: $(XSTEP) $(APPLICATIONS)
	strip $(APPLICATIONS)
$(XSTEP):
	make -C ../library
clean:
	rm $(APPLICATIONS)
show:
	for i in $(APPLICATIONS); do echo $$i; ./$$i; done
