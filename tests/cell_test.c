#include <stdio.h>
#include "../cells.h"

int main(){
	LinkedList* list = create_cells();
	Node* current = get_head(list);
	current->data = 10000;
	current->next->data = -5;
	printf("First value: %ld\n", current->data);
	current = current->next;
	printf("Second value: %ld\n", current->data);
	reset_list(list);	
	printf("Second value: %ld\n", current->data);
	current = get_head(list);
	for(int i = 0; i < INITIAL_CAPACITY+RESIZE_FACTOR+66; i++){
		current->data = i+1;
		printf("Index %d: %ld\n", i, current->data);
		if(current->next == NULL){
			resize_list(list);
		}
		current = current->next;
		list->size++;
	}
	while(current != NULL){
		printf("Index %d: %ld\n", list->size, current->data);
		current = current->prev;
		list->size--;
	}
}
