#ifndef __JOBS_H
#define __JOBS_H

#define FIFO_BUFFER_SIZE 32

// job struct
typedef struct 
{
  volatile unsigned long run_every;
  volatile unsigned long run_last;
  volatile void (*job_ptr) (void);
} job;

// FIFO access routines
         void fifo_init(fifo *_fifo);
          int fifo_getc(fifo *_fifo); 
          int fifo_putc(fifo *_fifo, int c);
unsigned char fifo_size(fifo *_fifo);

#endif 
