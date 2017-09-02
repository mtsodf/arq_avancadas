
#ifndef UTILS_H
#define UTILS_H

#define SIZE_COUNTER 100
#define MAX_THREADS  16

//Medicao de Tempo
double get_clock_sec( void );
double get_clock_msec( void );


//Print Paths
void PrintPath(Path* path);
void PrintPath(int iter, Path* path);
void PrintPath(char* filename, Path* path);



enum sections { initSection = 0, findMinimumSection, barrierSection, endParallelSection, swapSection, totalOptSection, criticalSection};


void initTimers(int id);
void initTimers();

void getRunId(char* run_id);


class TimeCounter{
    public:

        char sectionsNames[SIZE_COUNTER][30];
        double times[SIZE_COUNTER][2];

        int size;
        int current;

        TimeCounter();
        void registerTimer(char* sectionName, int id);
        void startTimer(int id);
        void endTimer(int id);
        void endStartTimer(int endId, int startId);
        void startTimer(int id, double timeNow);
        void endTimer(int id, double timeNow);
        void endStartTimer(int endId, int startId, double timeNow);
        void printAllSections();
        double getTotalTime(int id);

    private:

        int positions[SIZE_COUNTER];
};

TimeCounter* getTimeCounter(int thread_id);

#endif
