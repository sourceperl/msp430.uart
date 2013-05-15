#ifndef __JOB_H
#define __JOB_H

// job struct
typedef struct 
{
  volatile unsigned long run_every;
  volatile unsigned long run_last;
  void (*job_ptr) (void);
} job;

// job routines
void job_init(job *_job, unsigned long every_ms, void (*job_ptr) (void));
void job_update(job *_job); 

#endif
