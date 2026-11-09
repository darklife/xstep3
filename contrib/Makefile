# XSTEP 3.5.1 - Toolkit for X Window System
# Copyright (C) 1996-2001 by Marcelo Samsoniuk
#
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

SYSTEM := `uname -s`

all install uninstall clean:
	cd demos 	&& make $@
	cd xstep3d 	&& make $@
	cd files 	&& make $@
	cd academia 	&& make $@
	cd cadastro	&& make $@
	cd yerkilt	&& make $@
	cd seth		&& make $@
	cd xstep68k	&& make $@
	cd xstep-shell	&& make $@
	test "$(SYSTEM)" = "Linux" && cd linuxtools && make $@
