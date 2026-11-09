/*

XSTEP 3.2 - Toolkit for X-Window System
Copyright (C) 1996,1997,1998 by Marcelo Samsoniuk

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Please read the COPYING and README file!!!

*/

char *eventname[]={

	"Null Event",
	"*",
	"Key Press",
	"Key Release",
	"Button Press",
	"Button Release",
	"Motion Notify",
	"Enter Notify",
	"Leave Notify",
	"Focus In",
	"Focus Out",
	"Keymap Notify",
	"Expose",
	"Graphics Expose",
	"No Expose",
	"Visibility Notify",
	"Create Notify",
	"Destroy Notify",
	"Unmap Notify",
	"Map Notify",
	"Map Request",
	"Reparent Notify",
	"Configure Notify",
	"Configure Request",
	"Gravity Notify",
	"Resize Request",
	"Circulate Notify",
	"Circulate Request",
	"Property Notify",
	"Selection Clear",
	"Selection Request",
	"Selection Notify",
	"Colormap Notify",
	"Client Message",
	"Mapping Notify",
};

char *geteventname(int i) {

	if(i<=34) 	return(eventname[i]);
	else 		return(eventname[1]);
}
