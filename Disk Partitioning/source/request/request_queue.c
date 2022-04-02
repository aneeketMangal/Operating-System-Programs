// implementation and helper functions for request_queue data structure
// linked list implementation
struct RequestBody dummy;


struct RequestQueueNode{
    struct RequestBody key;
    struct RequestQueueNode* next;
};

struct RequestQueue{
    struct RequestQueueNode *front, *tail;
};

struct RequestQueue* buildQueue(){
    dummy.valid = 0;
    struct RequestQueue* queue = (struct RequestQueue*)malloc(sizeof(struct RequestQueue));
    queue->front = NULL;
    queue->tail = NULL;
    return queue;
}

int enqueue(struct RequestQueue* queue, struct RequestBody newRequest){
    struct RequestQueueNode* temp = (struct RequestQueueNode*)malloc(sizeof(struct RequestQueueNode));
    temp->key = newRequest;
    temp->next = NULL;
    if(queue-> tail == NULL){
        queue->front = temp;
        queue->tail = temp;
        return 1;
    }
    queue->tail->next = temp;
    queue->tail = temp;
}
struct RequestBody dequeue(struct RequestQueue* queue){
    if(queue->front == NULL){
        return dummy;
    }
    else{
        struct RequestQueueNode* temp = queue->front;
        queue->front = queue->front->next;
        if(queue->front == NULL) queue->tail = NULL;
        struct RequestBody currRequest = temp->key;
        free(temp);
        return currRequest;
    }
}

struct RequestBody * heapRequest(struct RequestBody currRequest){
    struct RequestBody * requestArgs = malloc(sizeof(struct RequestBody));
    *requestArgs = currRequest;
    return requestArgs;
}
