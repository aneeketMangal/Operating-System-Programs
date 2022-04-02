#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#include "constant_set.c"
#include "utility.c"
#include "interrupt.c"

#include "allocation_algo/best_fit.c"
#include "allocation_algo/first_fit.c"
#include "allocation_algo/next_fit.c"
#include "allocation_algo/allocate.c"


#include "request/request_body.c"
#include "request/request_queue.c"
#include "request/generate_random_request.c"

#include "performance_metrics/metrics.c"
#include "performance_metrics/update_turn_around.c"
#include "performance_metrics/calc_memory_util.c"
#include "performance_metrics/calc_average_turnaround.c"

#include "deallocate.c"
// #include "process.c"