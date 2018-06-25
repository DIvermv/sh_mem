
#include "sh_inc.h"

int main (int argc, char *argv[])
{
	
	key_t SHM_KEY;
        key_t SEM_KEY;
if((SHM_KEY = ftok("/initrd.img",0)) < 0){// генерируем ключ
       printf("Can\'t generate shm key\n");
        return -1;
    }

if((SEM_KEY = ftok("/initrd.img.old",0)) < 0){// генерируем ключ
       printf("Can\'t generate sem key\n");
        return -1;
    }

	int count=12; // сколько будет инкриментов переменной
    sh_int_inc(SHM_KEY,SEM_KEY,count);
  return 0;
}
