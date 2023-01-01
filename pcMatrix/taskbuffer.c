#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <unistd.h>
#include "taskbuffer.h"

bounded_buff getBuffer(buffer) {

    buffer.fill_ptr = 0;
    buffer.use_ptr = 0;
    buffer.count = 0;
    
    assert(pthread_mutex_init(&buffer.lock, NULL) == 0);
    assert(pthread_cond_init(&buffer.fill, NULL) == 0 &&
    	pthread_cond_init(&buffer.empty, NULL) == 0);

    return buffer;
}
