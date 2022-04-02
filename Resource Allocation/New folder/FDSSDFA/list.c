typedef struct linkedList {
    time_t data;
    struct linkedList *next;
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

void delete(List** head)
{
   List* curr = *head;
   List* next;
   while (curr)
   {
       next = curr->next;
       free(curr);
       curr = next;
   }
   *head = NULL;
}

double calculateAverage(List * head){
    double start = (double) (head-> data);
    head = head->next;
    double ans = 0;
    int count = 1;
    double temp;
    while (head)
    {
        temp = (double) (head-> data); 
        ans += (start-temp);
        ++count;
        start = temp;
        head = head->next;
    }
    return ans/count;
}