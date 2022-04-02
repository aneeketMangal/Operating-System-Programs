// implementation and helper functions for request_queue data structure
struct RequestBody dummy;


struct RequestQueue{
    int front, tail;
    int size;
    struct RequestBody* Queue;
};



struct RequestQueue* buildQueue(){
    dummy.dll_name = NULL;
    struct RequestQueue* queue = (struct RequestQueue*)malloc(sizeof(struct RequestQueue));
    queue->front = 0;
    queue->size = 0;
    queue->tail = DEFAULT_SIZE-1;
    queue->Queue = (struct RequestBody*)malloc(DEFAULT_SIZE*sizeof(struct RequestBody));
    return queue;
}

int empty(struct RequestQueue* queue){
    return (queue->size ==0);
}

int full(struct RequestQueue* queue){
    return (queue->size ==DEFAULT_SIZE);
}
int enqueue(struct RequestQueue* queue, struct RequestBody newRequest){
    if(!full(queue)){
        queue->size = queue->size+1;
        queue->tail = (queue->tail+1)%DEFAULT_SIZE;
        queue->Queue[queue->tail] = newRequest;
        return 1;
    }
    return 0;
}
struct RequestBody dequeue(struct RequestQueue* queue){
    if(!empty(queue)){
        queue->size = queue->size-1;
        struct RequestBody request = queue->Queue[queue->front];
        queue->front = (queue->front+1)%DEFAULT_SIZE;
        // struct responsePointer* response;
        return request;
    }
    return dummy;
}