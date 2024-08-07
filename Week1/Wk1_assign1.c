#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define NUM_THREADS 1

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];


void *counterThread(void *threadp)
{
    int sum=0, i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=1; i < (threadParams->threadIdx)+1; i++)
        sum=sum+i;
 
    printf("Thread idx=%d, sum[0...%d]=%d\n", 
           threadParams->threadIdx,
           threadParams->threadIdx, sum);
    openlog(NULL, threadParams->threadIdx, LOG_USER);
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Thread!");
    closelog();
}


int main (int argc, char *argv[])
{
   int rc;
   int i;
   char uname[512];
   char command[20];
    /* Run "uname -a" and save to Log */
    sprintf(command, "uname -a"); // save the command to execute, to command
    FILE * pf = popen(command, "r"); // open a pipe for the command to run
    fgets(uname, sizeof(uname), pf); // read from pipe to uname
    pclose(pf); // close pipe
    /* Log uname -r and other details*/

    openlog(NULL, 0, LOG_USER);
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] %s\n", uname);
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Main!");
    closelog();

   for(i=0; i < NUM_THREADS; i++)
   {
       threadParams[i].threadIdx=i;

       pthread_create(&threads[i],   // pointer to thread descriptor
                      (void *)0,     // use default attributes
                      counterThread, // thread function entry point
                      (void *)&(threadParams[i]) // parameters to pass in
                     );

   }

   for(i=0;i<NUM_THREADS;i++)
       pthread_join(threads[i], NULL);

   printf("TEST COMPLETE\n");
}
