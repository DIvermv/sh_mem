#include "sh_inc.h"
 int   sh_int_inc(key_t sem_key,key_t sh_key,int count)
{


int semid, shmid;
int sh_mem;
struct shmsegm *shmp;
union semun arg;
struct sembuf sop;// один семафор
struct semid_ds ds;
semid = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | perms); // создаем семафор, обязательно должен отсутсвовать в ядре
if(semid!=-1) // семафор создан - инициализируем
{
  printf("semid create  = %d\n",semid);
arg.val = 0;
if((semctl(semid, 0, SETVAL, arg))==-1)// иницмализируем семафор в  0 - свободным
// errExit("semctl");
    printf("setval error \n");


  sop.sem_num = 0; // Operate on semaphore 0 
  sop.sem_op = 0; // Wait for value to equal 1 
  sop.sem_flg = 0;
if (semop(semid, &sop, 1) == -1)// ждем освобождения семафора
    printf("semop error\n");

    printf("sem free\n");
}
else
{

  printf("semid existed\n");
  semid = semget(sem_key, 1, perms);// получаем существующий ID
   if (semid == -1)
    printf("semget error\n");
   else
   {
   
    printf("sem open. ID %d\n",semid);
    arg.buf = &ds;
    for (int j = 0; j < 10; j++) { // ждем не более 10 секунд, пока второй процесс проинициализирует семафор
      if (semctl(semid, 0, IPC_STAT,arg)==-1)
          printf("semctl error\n");// ошибка получения arg
      if (ds.sem_otime != 0) // если семафор инициализирован - покидаем цикл
         break;
     sleep(1);
     } 
     if (ds.sem_otime == 0)
       printf("Existing semaphore not initialized");
   }   
}


// инициализируем общую память
shmid = shmget(sh_key, sizeof(int), IPC_CREAT | perms);// в данном случае размером int
if (shmid == -1)
printf("shmget error");
else
{
printf("shmid %d\n",shmid);
}

shmp = shmat(shmid, NULL, 0);// получили адрес памяти
    sh_mem = shmp->buf;// считываем значение переменной из памяти
    shmp->buf+=1;
if (shmp == (void *) -1)
printf("shmat error\n");
// основная часть
int start=sh_mem;
while(shmp->buf<(count+start)) // вот здесь он берет значение из общей памяти и ее надо блокировать на это время? или это локальное значение?
{	
  sop.sem_op = 1;
if (semop(semid, &sop, 1) == -1)// блокируем семафор
    printf("sem block error\n");
    printf("sem blocked\n");
if (sh_mem == shmp->buf)
	sleep(1);
	else
   {
    shmp->buf++;
    sh_mem = shmp->buf;
    printf("%i\n",shmp->buf);
   }
	
    //  printf(" считано %i\n",sh_mem);
//shmp->cnt=     // запись в общую память


    sop.sem_op =-1;
if (semop(semid, &sop, 1) == -1)// освобождаем семафор
    printf("sem free error\n");
    printf("sem free\n");
//
}
return 0;
}
