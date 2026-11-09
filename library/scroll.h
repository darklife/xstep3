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

#include "xstep.h"

void upbutton_buttonrepeat(struct xtree *treeptr);
void upbutton_buttonpress(struct xtree *treeptr);
void upbutton_buttonrelease(struct xtree *treeptr);
struct xtree *upbutton_create(int x,int y,int *q,int *m);
void downbutton_buttonrepeat(struct xtree *treeptr);
void downbutton_buttonpress(struct xtree *treeptr);
void downbutton_buttonrelease(struct xtree *treeptr);
struct xtree *downbutton_create(int x,int y,int *q,int *m);
void scrollbar_broadcast(struct xtree *treeptr);
void scrollbar_expose(struct xtree *treeptr);
void scrollbar_buttonmotion(struct xtree *treeptr);
struct xtree *scrollbar_create(int x,int y,int h,int *q,int *m,int *n);
void list_expose(struct xtree *treeptr);
void list_broadcast(struct xtree *treeptr);
void list_buttonrepeat(struct xtree *treeptr);
void list_buttonpress(struct xtree *treeptr);
void list_buttonrelease(struct xtree *treeptr);
struct xtree *list_create(int x,int y,int w,int h,int *q,int *m,int *n,char *z[],void (*f)(struct xtree *),void (*dc)(struct xtree *),void (*rc)(struct xtree *));
void scroll_configure(struct xtree *treeptr);
void scroll_expose(struct xtree *treeptr); 
