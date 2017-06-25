/*
 * pagesim.cpp
 *
 * Student Name:
 * Student Number:
 *
 * Class: CPSC 457 Spring 2017
 * Instructor: Pavol Federl
 *
 * Copyright 2017 University of Calgary. All rights reserved.
 */


#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char * const argv[]) {

	if(argc != 2) {
		printf("invalid arguments\n");
		return 1;
	}

	int numFrames = atoi(argv[1]);
	int i = 0;
	int pages[5000];
	int frames[20];
	int faults = 0;
	int numPages = 0;

	while(1) {
		if(1 != scanf("%d", &pages[i])) break;
		i++;
		numPages++;
	}

	for(i = 0; i < numFrames; i++) {
		frames[i] = -1;		
	}

	int swap[numPages-numFrames];

	// OPT
	for(i = 0; i < numPages; i++) {
		int flag1 = 0, flag2 = 0, j;

		for(j = 0; j < numFrames; j++) {
			if(frames[j] == pages[i]) {
				flag1 = 1;
				flag2 = 1;
				break;
			}
		}

		if(flag1 == 0) {
			for(j = 0; j < numFrames; j++) {
				if(frames[j] == -1) {
					faults++;
					frames[j] = pages[i];
					flag2 = 1;
					break;
				}
			}
		}

		if(flag2 == 0) {
			int flag3 = 0;
			for(j = 0; j < numFrames; j++) {
				swap[j] = -1;
				int k;
				for(k = i + 1; k < numPages; k++) {
					if(frames[j] == pages[k]) {
						swap[j] = k;
						break;
					}
				}
			}

			int current;

			for(j = 0; j < numFrames; j++) {
				if(swap[j] == -1) {
					current = j;
					flag3 = 1;
					break;
				}
			}

			if(flag3 == 0) {
				int max = swap[0];
				current = 0;

				for(j = 1; j < numFrames; j++) {
					if(swap[j] > max) {
						max = swap[j];
						current = j;
					}
				}
			}

			frames[current] = pages[i];
			faults++;
		}
	}

	printf("OPT:\n");
	printf("\tFrames: ");
	for(i = 0; i < numFrames; i++) {
		printf("%d ", frames[i]);
	} printf("\n");

	printf("\tTotal faults: %d\n", faults);

	// LRU
	faults = 0;
	int count = 0, usedCounter[numFrames];
	for(i = 0; i < numFrames; i++) {
		frames[i] = -1;
		usedCounter[i] = 0;
	}

	for(i = 0; i < numPages; i++) {
		int flag1 = 0, flag2 = 0, j;
		for(j = 0; j < numFrames; j++) {
			if(frames[j] == pages[i]) {
				usedCounter[j] += 1;
				flag1 = 1;
				flag2 = 1;
				break;
			}
		}

		if(flag1 == 0) {
			for(j = 0; j < numFrames; j++) {
				if(frames[j] == -1) {
					faults++;
					frames[j] = pages[i];
					usedCounter[j] = 0;
					flag2 = 1;
					break;
				}
			}
		}

		if(flag2 == 0) {
			int leastUsed = usedCounter[0], current = 0;
			for(int k = 1; k < numFrames; k++) {
				if(usedCounter[k] < leastUsed) {
					leastUsed = usedCounter[k];
					current = k;
				}
			}

			faults++;
			frames[current] = pages[i];
			usedCounter[current] = 0;
		}
	}

	printf("LRU:\n");
	printf("\tFrames: ");
	for(i = 0; i < numFrames; i++) {
		printf("%d ", frames[i]);
	} printf("\n");

	printf("\tTotal faults: %d\n", faults);

	// CLOCK

	faults = 0;
	int clockHand = 0, clock[numFrames];

	for(i = 0; i < numFrames; i++) {
		frames[i] = -1;
		clock[i] = 0;
	}

	for(i = 0; i < numPages; i++) {
		int j, flag1 = 0, flag2 = 0;
		for(j = 0; j < numFrames; j++) {
			if(pages[i] == frames[j]) {
				clockHand = j;
				clock[clockHand] = 1;
				flag1 = 1;
				flag2 = 1;
				break;
			}
		}

		if(flag1 == 0) {
			for(j = 0; j < numFrames; j++) {
				if(frames[j] == -1) {
					frames[j] = pages[i];
					faults++;
					clockHand = j;
					clock[clockHand] = 1;
					flag2 = 1;
				}
			}
		}

		if(flag2 == 0) {
			while(flag2 != 1) {
				if(clockHand > numFrames) clockHand = 0;
				if(clock[clockHand] == 0) {
					faults++;
					frames[clockHand] = pages[i];
					clock[clockHand] = 1;
					flag2 = 1;
				}
				else {
					clock[clockHand] = 0;
					clockHand++;
				}
			}
		}
	}

	
	printf("CLOCK:\n");
	printf("\tFrames: ");
	for(i = 0; i < numFrames; i++) {
		printf("%d ", frames[i]);
	} printf("\n");

	printf("\tTotal faults: %d\n", faults);

	return 0;
}
