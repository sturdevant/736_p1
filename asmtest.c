#include<stdio.h>

#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>

#define RUN_COUNT 200000
#define TIMER_COUNT 1

#define TIMING_SETUP_OPERATIONS THREAD_CREATE_TIMING_SETUP_OPERATIONS
#define OPERATIONS_TO_TIME THREAD_CREATE_OPERATIONS_TO_TIME





#define THREAD_CREATE_TIMING_SETUP_OPERATIONS \
pthread_t p;\
volatile int x;\

// END OF SETUP OPERATIONS

// Code below this define will be timed.
#define THREAD_CREATE_OPERATIONS_TO_TIME  \
x = pthread_create(&p, NULL, &short_child_thread, NULL);\
pthread_join(p, NULL);
// END OF TIMED OPERATIONS

// Forward declarations for a few basic statistics.
long array_min_index(long* vals, int count);
long array_max(long* arr, int count);
int array_val_occurances(long* arr, int count, int val);
int max_array_val_occurance_gap(long* arr, int count, int val);

// Forward declarations for each of the timer functions.
long asm_timer();
long clk_monotonic_timer();
long clk_cpu_timer();
long clk_thread_timer();
long rusage_timer();

void* short_child_thread() {
	pthread_exit(NULL);
}

int main(int argc, char** argv) {
	
	long t_result[TIMER_COUNT][RUN_COUNT];
	long (*timer_func[TIMER_COUNT])(void);
	timer_func[0] = &asm_timer;
//	timer_func[1] = &clk_monotonic_timer;
//	timer_func[2] = &clk_cpu_timer;
//	timer_func[3] = &clk_thread_timer;
//	timer_func[4] = &rusage_timer;
	int i, j;

	for (i = 0; i < TIMER_COUNT; i++) {
		for (j = 0; j < RUN_COUNT; j++) {
			t_result[i][j] = timer_func[i]();
		}
	}

	printf("trial\tassembly\tmonotonic\tcputime\tthread\trusage\n");
	long* t_asm = t_result[0];
	long asm_min = t_asm[array_min_index(&t_asm[0], RUN_COUNT)];

	printf("Assembly minimum: %ld\n", asm_min);
	printf("Assembly min count: %d\n", array_val_occurances(&t_asm[0], RUN_COUNT, asm_min));
	printf("Assembly max gap: %d\n", max_array_val_occurance_gap(&t_asm[0], RUN_COUNT, asm_min));
	printf("Assembly max: %ld\n", array_max(&t_asm[0], RUN_COUNT));	
	
	return 0;
}
long array_min_index(long* vals, int count) {
	if (count <= 0) {
		return -1;
	}
	long cur_min = 0;
	int i;
	for (i = 1; i < count; i++) {
		if (vals[i] < vals[cur_min]) {
			cur_min = i;
		}
	}
	return cur_min;
}

long array_max(long* arr, int count) {
	if (count <= 0) {
		return -1;
	}
	long cur_max = arr[0];
	int i;
	for (i = 1; i < count; i++) {
		if (arr[i] > cur_max) {
			cur_max = arr[i];
		}
	}
	return cur_max;
}

int array_val_occurances(long* arr, int count, int val) {
	if (count <= 0) {
		return -1;
	}
	int n_found = 0;
	int i;
	for (i = 0; i < count; i++) {
		if (arr[i] == val) {
			n_found++;
		}
	}
	return n_found;
}

int max_array_val_occurance_gap(long* arr, int count, int val) {
	if (count <= 0) {
		return -1;
	}
	int max_gap_len = 0;
	int cur_gap_len = 0;
	int i;
	for (i = 0; i < count; i++) {
		if (arr[i] == val) {
			if (cur_gap_len > max_gap_len) {
				max_gap_len = cur_gap_len;
			}
			cur_gap_len = 0;
		} else {
			cur_gap_len++;
		}
	}
	if (cur_gap_len > max_gap_len) {
		max_gap_len = cur_gap_len;
	}
	return max_gap_len;
}

long asm_timer() {
	int t1, t2, t3, t4;
	TIMING_SETUP_OPERATIONS
	asm("rdtscp; mov %%eax, %0;":"=a"(t1));
	asm(        "mov %%edx, %0;":"=a"(t2));
	OPERATIONS_TO_TIME
	asm("rdtscp; mov %%eax, %0;":"=a"(t3));
	asm(        "mov %%edx, %0;":"=a"(t4));
	return (t4 - t2) * 1000000000 + t3 - t1;
}

long clk_monotonic_timer() {
	struct timespec temp_ts_1, temp_ts_2;
	TIMING_SETUP_OPERATIONS
	clock_gettime(CLOCK_MONOTONIC, &temp_ts_1);
	OPERATIONS_TO_TIME
	clock_gettime(CLOCK_MONOTONIC, &temp_ts_2);
	return (temp_ts_2.tv_sec - temp_ts_1.tv_sec) * 1000000000 + temp_ts_2.tv_nsec - temp_ts_1.tv_nsec;	
}

long clk_cpu_timer() {
	struct timespec temp_ts_1, temp_ts_2;
	TIMING_SETUP_OPERATIONS
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &temp_ts_1);
	OPERATIONS_TO_TIME
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &temp_ts_2);
	return (temp_ts_2.tv_sec - temp_ts_1.tv_sec) * 1000000000 + temp_ts_2.tv_nsec - temp_ts_1.tv_nsec;	
}

long clk_thread_timer() {
	struct timespec temp_ts_1, temp_ts_2;
	TIMING_SETUP_OPERATIONS
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &temp_ts_1);
	OPERATIONS_TO_TIME
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &temp_ts_2);
	return (temp_ts_2.tv_sec - temp_ts_1.tv_sec) * 1000000000 + temp_ts_2.tv_nsec - temp_ts_1.tv_nsec;	
}

long rusage_timer() {
	struct rusage temp_ru_1, temp_ru_2;
	TIMING_SETUP_OPERATIONS
	long temp_t_1, temp_t_2;
	getrusage(RUSAGE_SELF, &temp_ru_1);
	OPERATIONS_TO_TIME
	getrusage(RUSAGE_SELF, &temp_ru_2);
	temp_t_1 = temp_ru_1.ru_utime.tv_sec + temp_ru_1.ru_stime.tv_sec + (temp_ru_1.ru_stime.tv_usec + temp_ru_1.ru_utime.tv_usec) * 1000000;
	temp_t_2 = temp_ru_2.ru_utime.tv_sec + temp_ru_2.ru_stime.tv_sec + (temp_ru_2.ru_stime.tv_usec + temp_ru_2.ru_utime.tv_usec) * 1000000;
	return temp_t_2 - temp_t_1;
}
