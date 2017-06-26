/*
 * pagesim.cpp
 *
 * Student Name: Nikolaas Lam
 * Student Number: 10037136
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

	// Errro check arguments
	if(argc != 2) {
		printf("invalid arguments\n");
		return 1;
	}

	int numFrames = atoi(argv[1]);		// Number of frames
	int i = 0;							// Loop count
	int pages[5000];					// Page array
	int frames[20];						// Frame array
	int faults = 0;						// Fault counter
	int numPages = 0;					// Number of pages

	// Read from stdin, insert to pages array, count number of pages
	while(1) {
		if(1 != scanf("%d", &pages[i])) break;
		i++;
		numPages++;
	}

	// initialize array elements to non-possible value
	for(i = 0; i < numFrames; i++) {
		frames[i] = -1;		
	}

	int opt[numPages-numFrames];				// Array for checking future array elements 

	/// OPT algorithm
	// Loop through pages
	for(i = 0; i < numPages; i++) {
		int emptyFrame = 0, mismatchFrame = 0, j;					// Condition flags

		// On hit, iterate to next page
		for(j = 0; j < numFrames; j++) {
			if(frames[j] == pages[i]) {
				emptyFrame = 1;								// Set condition flags
				mismatchFrame = 1;
				break;
			}
		}

		// If emptyFrame not set, check if frame is empty
		if(emptyFrame == 0) {
			for(j = 0; j < numFrames; j++) {
				if(frames[j] == -1) {
					faults++;							// If empty, fault and insert current page
					frames[j] = pages[i];
					mismatchFrame = 1;							// Set mismatchFrame
					break;
				}
			}
		}

		// If mismatchFrame not set, page miss, frame not empty
		if(mismatchFrame == 0) {								
			int notUsed = 0;									// notUsed flag, check if not most used page
			for(j = 0; j < numFrames; j++) {					// Loop through n frames, check if hit for current pages
				opt[j] = -1;
				int k;
				for(k = i + 1; k < numPages; k++) {
					if(frames[j] == pages[k]) {
						opt[j] = k;							// If current frames used in upcoming pages, keep frame
						break;
					}
				}
			}

			int current;									// Index position

			// Check future pages, if not set, then set
			for(j = 0; j < numFrames; j++) {
				if(opt[j] == -1) {							// If no page in opt, remember position
					current = j;
					notUsed = 1;							// Set flag
					break;
				}
			}

			// If flag not set, find most used
			if(notUsed == 0) {
				int max = opt[0];
				current = 0;

				for(j = 1; j < numFrames; j++) {
					if(opt[j] > max) {
						max = opt[j];
						current = j;
					}
				}
			}

			frames[current] = pages[i];						// replace current frame with page
			faults++;										// Increase fault
		}
	}
	
	// Print current frames and faults
	printf("OPT:\n");
	printf("\tFrames: ");
	for(i = 0; i < numFrames; i++) {
		printf("%d ", frames[i]);
	} printf("\n");

	printf("\tTotal faults: %d\n", faults);

	// LRU algorithm
	faults = 0;
	int count = 0, usedCounter[numFrames];					// total loop counter, array for time of last used frame

	// Initialize values of arrays
	for(i = 0; i < numFrames; i++) {
		frames[i] = -1;
		usedCounter[i] = 0;
	}

	// Loop through each page
	for(i = 0; i < numPages; i++) {							
		int emptyFrame = 0, mismatchFrame = 0, j;

		// If frame matches, go to next page
		for(j = 0; j < numFrames; j++) {
			if(frames[j] == pages[i]) {
				count++;
				usedCounter[j] = count;
				emptyFrame = 1;
				mismatchFrame = 1;
				break;
			}
		}

		// Check if frame empty
		if(emptyFrame == 0) {
			for(j = 0; j < numFrames; j++) {
				if(frames[j] == -1) {					// If empty, insert page
					faults++;
					count++;							// Increment loop count
					frames[j] = pages[i];
					usedCounter[j] = count;				// Store current loop count for frame
					mismatchFrame = 1;					// Set flag to go to next page
					break;
				}
			}
		}

		// Page miss
		if(mismatchFrame == 0) {
			int leastUsed = usedCounter[0], current = 0;
			for(int k = 1; k < numFrames; k++) {				// Find the least recently used frame
				if(usedCounter[k] < leastUsed) {
					leastUsed = usedCounter[k];
					current = k;
				}
			}

			count++;
			faults++;
			frames[current] = pages[i];							// Replace least used with current page
			usedCounter[current] = count;
		}
	}

	// Print values
	printf("LRU:\n");
	printf("\tFrames: ");
	for(i = 0; i < numFrames; i++) {
		printf("%d ", frames[i]);
	} printf("\n");

	printf("\tTotal faults: %d\n", faults);

	// CLOCK

	faults = 0;
	int clockHand = 0, clock[numFrames];						// Clock hand is index of clock
																// clock array for single bit reference
	for(i = 0; i < numFrames; i++) {
		frames[i] = -1;
		clock[i] = 0;
	}

	// Loop through pages
	for(i = 0; i < numPages; i++) {
		int j, emptyFrame = 0, mismatchFrame = 0;

		// If page hit, set reference bit to 1, go to next page
		for(j = 0; j < numFrames; j++) {
			if(pages[i] == frames[j]) {
				clockHand = j;									// Update clock hand to current frame
				clock[clockHand] = 1;
				emptyFrame = 1;
				mismatchFrame = 1;
				break;
			}
		}

		// Check if empty frame
		if(emptyFrame == 0) {
			for(j = 0; j < numFrames; j++) {
				if(frames[j] == -1) {							// Replace with current page
					frames[j] = pages[i];
					faults++;
					clockHand = j;								// update clock hand
					clock[clockHand] = 1;						// Set bit to 1
					mismatchFrame = 1;
				}
			}
		}

		// Page miss
		if(mismatchFrame == 0) {
			while(mismatchFrame != 1) {							// While flag is not set
				if(clockHand > numFrames) clockHand = 0;		// If end of array, go to beginning
				if(clock[clockHand] == 0) {						// If refernce bit is 0, replace current frame
					faults++;
					frames[clockHand] = pages[i];
					clock[clockHand] = 1;						// Set refrence bit to 1
					mismatchFrame = 1;
				}
				else {
					clock[clockHand] = 0;						// If reference bit is 1, set to 0
					clockHand++;								// Go to next frame
				}
			}
		}
	}

	// Print values
	printf("CLOCK:\n");
	printf("\tFrames: ");
	for(i = 0; i < numFrames; i++) {
		printf("%d ", frames[i]);
	} printf("\n");

	printf("\tTotal faults: %d\n", faults);

	return 0;
}
