
#include <sys/time.h>
#include <time.h>
#include "Path.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


TimeCounter* timers[MAX_THREADS];
bool started = false;

void initTimers(int rank){
	TimeCounter *t = getTimeCounter(rank);

	t->registerTimer("Inicializacao", initSection);
	t->registerTimer("Achar Minimo", findMinimumSection);
	t->registerTimer("Barreira", barrierSection);
	t->registerTimer("Swap", swapSection);
	t->registerTimer("Algoritmo Total", totalOptSection);
	t->registerTimer("Secao critica", criticalSection);
}

void initTimers(){
	initTimers(0);
}


double get_clock_sec( void ) {

	struct timeval t;
	struct timezone tz;
	gettimeofday(&t,&tz);
	return (double) t.tv_sec + (double) t.tv_usec * 1E-6;

}

double get_clock_msec( void ) {

	struct timeval t;
	struct timezone tz;
	gettimeofday(&t,&tz);
	return (double) t.tv_sec * 1E+3 + (double) t.tv_usec * 1E-3;

}


void PrintPath(Path* path){
	for (size_t i = 0; i < path->size; i++)
	{
		printf("%d %f %f\n", path->cities[i]->id, path->cities[i]->x, path->cities[i]->y);
	}
}


void PrintPath(int iter, Path* path){
	char filename[100];
	sprintf(filename, "path_%d.txt", iter);
	PrintPath(filename, path);
}


void PrintPath(char* filename, Path* path){
	FILE* out = fopen(filename, "w");

	fprintf(out, "%d\n", path->size);

	for (size_t i = 0; i < path->size; i++)
	{
		fprintf(out, "%d %f %f\n", path->cities[i]->id, path->cities[i]->x, path->cities[i]->y);
	}
	fclose(out);
}

TimeCounter* getTimeCounter(int thread_id){
	if(timers[thread_id] == NULL){
		timers[thread_id] = new TimeCounter();
	}
	return timers[thread_id];
}

TimeCounter::TimeCounter(){
	for (size_t i = 0; i < SIZE_COUNTER; i++)
	{
		times[i][0] = 0;
	}
}

void TimeCounter::registerTimer(char* sectionName, int id){
	strcpy(sectionsNames[id], sectionName);
	positions[size] = id;
	size++;
}

void TimeCounter::startTimer(int id, double time){
	if(!started) {
		started=true;
		initTimers();
	}
	times[id][1] = time;
}

void TimeCounter::startTimer(int id){
	startTimer(id, get_clock_msec());
}

void TimeCounter::endTimer(int id, double time){
	times[id][0] += time - times[id][1];
}

void TimeCounter::endTimer(int id){
	endTimer(id, get_clock_msec());
}

void TimeCounter::endStartTimer(int endId, int startId){
	endStartTimer(endId, startId, get_clock_msec());
}

void TimeCounter::endStartTimer(int endId, int startId, double timeNow){
	endTimer(endId, timeNow);
	startTimer(startId, timeNow);
}

void TimeCounter::printAllSections(){
	for (size_t i = 0; i < size; i++)
	{
		int ind = positions[i];
		printf("%30s -> %10.6lf\n", sectionsNames[ind], times[ind][0]);
	}
}

double TimeCounter::getTotalTime(int id){
	return times[id][0];
}

void getRunId(char* run_id){
	sprintf(run_id, "%lu%lu%d", time(NULL), clock(), rand()%100);
}