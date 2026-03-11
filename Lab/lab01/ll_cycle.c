#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* your code here */
	if(head == NULL){
		return 0;
	} 
		
	node *slow = head;
	node *fast = head;
	while(fast != NULL && fast->next != NULL){
		slow = slow->next;
		fast = fast->next->next;
		if(slow == fast){
			return 1;
		}
	}
    return 0;
}

// node* find_cycle_start(node* head){
// 	node *meet = ll_has_cycle(head);
// 	if(meet == NULL) return NULL;
// 	node *p1 = head;
// 	node *p2 = meet;
// 	//p1 == p2时就是环的起点
// 	while(p1 != p2){
// 		p1 = p1->next;
// 		p2 = p2->next;
// 	}
// 	return p1;
// }

// int cycle_length(node *head){
// 	node* meet = ll_has_cycle(head);
// 	if(meet == NULL) return 0;

// 	int length = 1;
// 	node *current = meet->next;
// 	//从相遇点绕一圈再到相遇点
// 	while(current != meet){
// 		length++;
// 		current = current->next;
// 	}
// 	return length;
// }