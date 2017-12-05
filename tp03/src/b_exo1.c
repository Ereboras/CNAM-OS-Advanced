#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE (int) 1e8 // 100 000 000

struct args_struct {
	int start;
	int end;
};

int myArray[SIZE];
int random_max, min, max;
struct timeval begin, end;

void * search_min(void* args) {
	struct args_struct *arguments = (struct args_struct *) args;
	min = random_max;

	for(int i=arguments->start;i<arguments->end;i++) {
		if(min > myArray[i]) {
			min = myArray[i];
		}
	}
}

void * search_max(void* args) {

	max = 0;

	for(int i=0;i<SIZE;i++) {
		if(max < myArray[i]) {
			max = myArray[i];
		}
	}
}

void createThreads(int nbThreads) {
	pthread_t threads[nbThreads];
	int sizeThread = SIZE / nbThreads;
	int rest = SIZE % nbThreads;
	struct args_struct args;

	for(int i = 0; i < nbThreads; i++) {
		args.start = sizeThread*i;
		args.end = args.start + sizeThread;
		if(i == nbThreads - 1 && rest > 0) {
			args.end += 1;
		}
		//printf("%d: %d - %d\n", i, args.start, args.end);
		pthread_create(&threads[i], NULL, search_min, (void *) &args);
	}
	for(int i = 0; i < nbThreads; i++) {
		pthread_join(threads[i], NULL);
	}
}

int calculateProcess(int nbThreads) {

	gettimeofday (&begin, NULL);

	createThreads(nbThreads);

	gettimeofday (&end, NULL);

	printf("search process time (%d threads): %fs\n", nbThreads, (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0));
}

int main(){

	random_max = 1e9; // 1 000 000 000

	// Array initialisation
	srand(time(NULL));
	for(int i=0;i < SIZE; i++) {
		myArray[i] = rand() % random_max;
	}

	struct args_struct args;
	args.start = 0;
	args.end = SIZE;

	// Search with 0 threads
	gettimeofday (&begin, NULL);

	search_min((void *) &args);
	search_max((void *) &args);

	gettimeofday (&end, NULL);

	printf("search process time (0 threads): %fs\n", (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0));

	// Search with 2 / 4 / 8 threads
	calculateProcess(2);
	calculateProcess(4);
	calculateProcess(8);

	return EXIT_SUCCESS;
}
