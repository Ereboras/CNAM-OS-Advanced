#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define SIZE (int) 1e8 // 100 000 000

struct args_struct {
	int start;
	int end;
};

struct result_struct {
	int min;
	int max;
};

struct result_struct results;

int myArray[SIZE];
int random_max, max;
struct timeval begin, end;

// Search the minimum value of the array myArray
void * search_min(void* args) {
	struct args_struct *arguments = (struct args_struct *) args;

	for(int i=arguments->start;i<arguments->end;i++) {
		if(results.min > myArray[i]) {
			results.min = myArray[i];
		}
	}

	return NULL;
}

// Search the maximum value of the array myArray
void * search_max(void* args) {

	for(int i=0;i<SIZE;i++) {
		if(results.max < myArray[i]) {
			results.max = myArray[i];
		}
	}

	return NULL;
}

// Creates the threads & assign equals portion of array to each
int createThreads(int nbThreads, void *(fct) (void *)) {
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

		if (pthread_create(&threads[i], NULL, search_min, (void *) &args)) {
			return EXIT_FAILURE;
		}
	}
	for(int i = 0; i < nbThreads; i++) {
		if(pthread_join(threads[i], NULL)) {
			return EXIT_FAILURE;
		}
	}
	return 0;
}

void calculateProcess(int nbThreads, char *fctSearch) {
	void *fct;
	if(strcmp(fctSearch, "min") == 0) {
		fct = search_min;
	} else {
		fct = search_max;
	}

	if(nbThreads > 0) {
		gettimeofday (&begin, NULL);
		createThreads(nbThreads, fct);
		gettimeofday (&end, NULL);
	} else {
		// Search with 0 threads
		struct args_struct args;
		args.start = 0;
		args.end = SIZE;

		gettimeofday (&begin, NULL);
		search_min((void *) &args);
		search_max((void *) &args);
		gettimeofday (&end, NULL);
	}

	printf("search process time of %s (%d threads): %fs\n",fctSearch, nbThreads, (end.tv_sec - begin.tv_sec) + ((end.tv_usec - begin.tv_usec)/1000000.0));
}

int main(){

	random_max = 1e9; // 1 000 000 000
	results.min = random_max;
	results.max = 0;

	// Array initialisation
	srand(time(NULL));
	for(int i=0;i < SIZE; i++) {
		myArray[i] = rand() % random_max;
	}

	// Search with 2 / 4 / 8 threads
	calculateProcess(0, "min");
	calculateProcess(0, "max");
	printf("--------------------\n");
	calculateProcess(2, "min");
	calculateProcess(2, "max");
	printf("--------------------\n");
	calculateProcess(4, "min");
	calculateProcess(4, "max");
	printf("--------------------\n");
	calculateProcess(8, "min");
	calculateProcess(8, "max");

	return EXIT_SUCCESS;
}
