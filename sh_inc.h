#ifndef FUNC_H
#define FUNC_H
#include <stdio.h>
#include <unistd.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
//#include "binary_sems.h"
//#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
union semun {
int val;
struct semid_ds * buf;
unsigned short * array;
struct shmsegm {
int cnt;
int buf;
};
#define perms (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#if defined(__linux__)
struct seminfo * __buf;
#endif
};
 int   sh_int_inc(key_t sem_key,key_t sh_key,int count);

#endif
