#include <stdio.h>
#include "../cells.h"

int main(){
	LinkedList* list = create_cells();
	*list->cells[list->curr_ind] = 10000;
	list->curr_ind++;
	*list->cells[list->curr_ind] = -5;
	list->curr_ind--;
	printf("First value: %ld\n", *list->cells[list->curr_ind]);
	list->curr_ind++;
	printf("Second value: %ld\n", *list->cells[list->curr_ind]);
	reset_list(list);	
	printf("Second value: %ld\n", *list->cells[list->curr_ind]);
	list->curr_ind = 0;
	for(int i = 0; i < INITIAL_CAPACITY+RESIZE_FACTOR+66; i++){
		*list->cells[i] = i+1;
		printf("Index %d: %ld\n", i, *list->cells[i]);
		if(list->curr_ind == list->capacity-1){
			resize_list(list);
		}
		list->curr_ind++;
	}
}
