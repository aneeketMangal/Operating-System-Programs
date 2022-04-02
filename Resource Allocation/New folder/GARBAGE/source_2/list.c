typedef struct Node
{
    time_t data;
    struct Node *next;
} List;

void push(List** head, time_t newData)
{
	List* new_node = (List*) malloc(sizeof(List));
	new_node->data = newData;
	new_node->next = *head;
	*head = new_node;
}

void printList(List * head){
    while (head)
    {
        printf("%ld\t", head->data);
        fflush(stdout);
        head = head->next;            
    }
    printf("\n");
}