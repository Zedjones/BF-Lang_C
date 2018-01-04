#include <stdio.h>
#include "../cells.h"

int main(){
	LinkedList* list = create_cells();
	list->cells[list->curr_ind] = (char)10000;
	list->curr_ind++;
	list->cells[list->curr_ind] = (char)-5;
	list->curr_ind--;
	printf("First value: %c\n", list->cells[list->curr_ind]);
	list->curr_ind++;
	printf("Second value: %c\n", list->cells[list->curr_ind]);
	reset_list(list);	
	printf("Second value: %c\n", list->cells[list->curr_ind]);
	list->curr_ind = 0;
	for(int i = 0; i < INITIAL_CAPACITY+RESIZE_FACTOR+66; i++){
		list->cells[i] = (char)i+1;
		printf("Index %d: %c\n", i, list->cells[i]);
		if(list->curr_ind == list->capacity-1){
			resize_list(list);
		}
		list->curr_ind++;
	}
}
