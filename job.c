/* 
Simple job management for MSP430


First parameter of every function is a pointer to the job struct (see job.h).

In your init main program you must allocate a job struct and call 
job_init(&jobstruct) for them.

In your main loop, you must call regulary job_update(&jobstruct) for shedule 
the task (call the job when it's time).

Code under GPLv2.
Share, it's happiness !
*/

#include <msp430.h> 
#include "millis.h"
#include "job.h"

/**
 * Init job struct.
 *
 */
void job_init(job *_job, unsigned long every_ms, void (*job_ptr) (void))
{
  _job->run_every = every_ms;
  _job->run_last  = millis();
  _job->job_ptr   = job_ptr;
}

/**
 * Job update routines.
 * Must be call in main loop.
 */
void job_update(job *_job)
{
  unsigned long now = millis();
  if ((now - _job->run_last) >= _job->run_every) {
    _job->run_last = now;
    if(_job->job_ptr != 0L)
      (_job->job_ptr)();
  }
}
