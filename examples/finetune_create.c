/* USAGE:

	finetune_create(int x, int y, int *q, int *a, int,b)
   
	x = horizontal position of the buttons
	y = vertical position of the buttons
	q = pointer to your integer variable
	a = (pointer to the) lowest number allowed by variable *q
	b = (pointer to the) highest number allowed by variable *q
                  
	you need to create a label or printf the variable in order to see its value
	maybe I will implement a built-in label... Allah knows! :)
*/
      
      
#include <xstep.h>

char buffer[64];

int value,min,max;

void calcf(struct xtree *w) {

	sprintf(buffer,"value from fine tune is %d (min %d, max %d)",
		value,min,max);
}

void xmain(int n,char *p[]) {

	struct xtree *win;

	min=-100;
	max=100;

        win=window_create(0,0,400,180,"finetune_create");
	win->broadcast=calcf;

	label_create(0,0,0,0,buffer,invisible,center);
	finetune_create(8,8,&value,&min,&max);
}
                       