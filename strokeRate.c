/*
Stroke Rate

linux
#include <iostream>
#include <sys/time.h>                // for gettimeofday()
using namespace std;

int main()
{
    timeval t1, t2;
    double elapsedTime;

    // start timer
    gettimeofday(&t1, NULL);

    // do something
    // ...

    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    cout << elapsedTime << " ms.\n";

    return 0;
}

*/


//Pseudocode
//constantly read filtered knee X data
//find max above threshold
//start timer
//wait for 5 more maxes
//stop timer
//print 5/time

/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


FILE *fp;
float prev;
float current;
double timer;
double timerBetweenMax;
double strokeRate;
int close;
int i;
int start;
int maxCounter;
int dataPoints;
int foundMax;
static double THRESHOLD = 0.03;
static double MIN_TIME_BETWEEN_STROKES = 0.5;
static double TIMER_INTERVAL = 0.005;

int main(void){

	//initialize variables
	close = 0;
	i = 0;
	start = 0;
	maxCounter = 0;
	dataPoints = 0;
	foundMax = 0;
	prev = 0;
	current = 0;
	timer = 0;
	timerBetweenMax = 0;

	fp = fopen("kneefiltX.txt", "r");

	// test for files not existing. 
    if (fp == NULL) 
    {   
		printf("Error! Could not open file\n"); 
		exit(0);
    }
	printf("opened\n");

	//find first max
	while(start == 0){
		prev = current;
		fscanf(fp,"%f", &current);
		if(prev > THRESHOLD){
			if(current < prev){
				start = 1;
				printf("Max:%.4f\n",prev);
			}
		}	
	}
	
	for(i=0; i<3; i++){
		while(maxCounter < 5){
			prev = current;
			//while(waiting);
			fscanf(fp,"%f",&current);
			//waiting = 1;
			dataPoints++;
			timer += TIMER_INTERVAL;
			timerBetweenMax += TIMER_INTERVAL;
			if(foundMax == 0){
				if(prev > THRESHOLD){
					if(current < prev){
						if(timerBetweenMax > MIN_TIME_BETWEEN_STROKES){
							maxCounter++;
							foundMax = 1;
							printf("Max:%.4f Time:%.2f\n",prev, timerBetweenMax);
							timerBetweenMax = 0;
						}
					}
				}	
			}
			if(prev < THRESHOLD){
				foundMax = 0;
			}
		}

		strokeRate = (5 / timer) * 60;
		printf("Stroke Rate:%.2f strokes/min Points:%d\n",strokeRate, dataPoints);
		dataPoints = 0;
		timer = 0;
		maxCounter = 0;
	}

	//Keep console open
	scanf("%d", &close);
}

*/