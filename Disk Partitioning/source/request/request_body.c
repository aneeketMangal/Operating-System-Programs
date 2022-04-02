struct RequestBody {
	int size; // memory size requested;
	int duration; // duration for which process will run
	int alloc_s; // index of starting block where the process has been allocated
	int valid;
	long int time_s;
};

