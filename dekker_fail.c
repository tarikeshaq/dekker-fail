#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
int want_to_enter[2] = {0, 0};
int turn = 0;
int inside[2] = {0, 0};

/*
* Dekker's algorithm, to be run on a two threaded process
* @param vargp: a pointer to a number, that is either 0, or 1
*		identifying the thread
* 
* The algorithm has been proven to provide mutual exclusion over a critical section
* assuming seqencial consistency.
* However, real computer do not gaurentee sequencial consistency
* And as such, we should expect that mutual exclusion is not preserved here
*/
void *dekker(void *vargp) {
	int *arg = vargp;
	int num = *arg;
	int other = (num + 1) % 2;
	// We repeat the algorithm a bunch of times
	// so that we are more likely to be running the multiple threads on
	// multiple cores, and so that the chance of breaking sequencial consistency
	// is higher.
	for (int i = 0; i < 10000; i++) {
		want_to_enter[num] = 1;
		while (want_to_enter[other]) {
			if (turn != num) {
				want_to_enter[num] = 0;
				while (turn != num);
				want_to_enter[num] = 1;
			}
		}
		// Critical section
		inside[num] = 1;
		// We spin to allow time for the other to enter the critical
		// section, and the checker to report it
		// Also gives more time that could exceed the quanta to switch the core
		// to a different thread (if only one core exists)
		// we could also sleep here.... but meh
		for (int i = 0; i < 10000; i++);
		inside[num] = 0;
		turn = other;
		want_to_enter[num] = 0;
	}
	return NULL;
}


/*
* A checker that will run infinitely, checking if both threads in dekker algorithm were
* in the critical section at the same time, violating mutual exclusion
*
* It will print "BOTH WERE INSIDE!!!!" to stdout if the mutual exclusion is violated
*/
void *checker(void *vargs) {
	while (1) {
		if (inside[0] && inside[1]) {
			printf("BOTH WERE INSIDE!!!!\n");
			exit(1);
			return NULL;
		}
	}
	return NULL;
}

int main() {
	pthread_t t1;
	pthread_t t2;
	int num1 = 0;
	int num2 = 1;
	pthread_t t_check;
	pthread_create(&t_check, NULL, checker, NULL);
	pthread_create(&t1, NULL, dekker, &num1);
	pthread_create(&t2, NULL, dekker, &num2);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("WE ARE OKAY, MUTUAL EXLUSION PRESERVED\n");
	exit(0);
}
