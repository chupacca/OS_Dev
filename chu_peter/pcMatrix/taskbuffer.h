/**
Peter Chu
*/


#define MAX_SIZE 250

typedef struct __bounded_buff {

	void *buffer[MAX_SIZE];
	int count;
	int fill_ptr;
	int use_ptr;

	pthread_mutex_t lock;
	pthread_cond_t fill;
	pthread_cond_t empty;

} bounded_buff;

void getBuffer(bounded_buff b);

