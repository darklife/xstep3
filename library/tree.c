/*

XSTEP 3.3 - Toolkit for X-Window System
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

struct xtree *structlist[1000];
char *treelist[1000];
int treelistptr,treelistmax,treetime;

void button_expose	(struct xtree *treeptr);
void check_expose	(struct xtree *treeptr);
void edit_expose	(struct xtree *treeptr);
void label_expose	(struct xtree *treeptr);
void menu_expose	(struct xtree *treeptr);
void popup_expose	(struct xtree *treeptr);
void radio_expose	(struct xtree *treeptr);
void scroll_expose	(struct xtree *treeptr);
void upbutton_expose	(struct xtree *treeptr);
void downbutton_expose	(struct xtree *treeptr);
void scrollbar_expose	(struct xtree *treeptr);
void list_expose	(struct xtree *treeptr);

char *widgetname(struct xtree *t) {

	if(t->expose==button_expose)	return("button.");
	if(t->expose==check_expose)	return("check.");
	if(t->expose==edit_expose)	return("edit.");
	if(t->expose==label_expose)	return("label.");
	if(t->expose==menu_expose)	return("menu");
	if(t->expose==popup_expose)	return("popup.");
	if(t->expose==radio_expose)	return("radio.");
	if(t->expose==scroll_expose)	return("scroll");
	if(t->expose==upbutton_expose)	return("upbutton");
	if(t->expose==downbutton_expose)return("downbutton");
	if(t->expose==scrollbar_expose)	return("scrollbar");
	if(t->expose==list_expose)	return("list");

	if(t->t) return("window.");
	return("group");
}

void scantree(struct xtree *t) {

	struct xtree *aux;
	char buffer[256]="";

	if(!t) return;
	if(t->t) 
		if(!strcmp(t->t,"widget tree")) {
	
			scantree(t->next);
			return;
		}

	strcat(buffer,application);
	strcat(buffer,":");
	
	for(aux=t->parent;aux;aux=aux->parent) {
	
		strcat(buffer,"/");
		strcat(buffer,widgetname(aux));
		if(aux->t) {
		
			strcat(buffer,"\"");
			strcat(buffer,aux->t);
			strcat(buffer,"\"");
		}
		if(aux->b) {
			
			strcat(buffer,"\"");
			strcat(buffer,aux->b);
			strcat(buffer,"\"");
		}
	}
	strcat(buffer,"/");
	strcat(buffer,widgetname(t));
	if(t->t) {
	
		strcat(buffer,"\"");
		strcat(buffer,t->t);
		strcat(buffer,"\"");
	}
	if(t->b) {
	
		strcat(buffer,"\"");
		strcat(buffer,t->b);
		strcat(buffer,"\"");
	}

	treelist[treelistmax]=(char *)malloc(1+strlen(buffer));
	strcpy(treelist[treelistmax],buffer);
	structlist[treelistmax]=t;
	treelistmax++;

	scantree(t->treestk);
	scantree(t->next);
}

void treegenerator(struct xtree *t) {

	int c;

	if(treetime==time(0)) return;
	treetime=time(0);

	for(c=0;c!=treelistmax;c++) free(treelist[c]);
	treelistmax=0;
	scantree(winstk);
}

void treequitf(struct xtree *t) {

	exit(0);
}

void tree(void) {

	animate=1;

	window_create(0,0,400,300,"widget tree");
	treestk->broadcast=treegenerator;

	scroll_create(8,8,-8,-40,&treelistptr,&treelistmax,treelist,0);
	button_create( -8,-8,72,24,"Quit",treequitf);
}
