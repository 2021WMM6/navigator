#include "func.h"

linked* search(char *name){
	linked *q=start;
	while(q->next!=NULL){
	q=q->next;
	if(strcmp(q->name,name)==0){
		return q;
		}
	}
	return NULL;
	}
	

void get_name(){
	int a;

	while(1){
		clear();
		printw("search word:");
		scanw("%s",name);
		if(search(name)!=NULL)
			cur2=search(name);
		else{
			printw("name not found\n");
	
		}
		refresh();
		a=getch();
		if(a==1){
			break;
		}
	}
}
	
