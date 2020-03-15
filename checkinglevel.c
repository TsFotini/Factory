#include "headers.h"

int main(int argc, char **argv){

	int shmid, shmid1, semid ;
	struct sembuf oper;

	int type=atoi(argv[1]);
	int Y=atoi(argv[2]);
	int i = 0;
	int size=type-1;
	int resultSem_checking , semaphore_checking;
	union semnum arg;

	int sem_check, timeToSleep;

	//According to the type of the component
	if(type==1){
		sem_check=3;
		timeToSleep=1;
	}
	else if(type==2){
		sem_check=4;
		timeToSleep=2;
	}
	else{
		sem_check=5;
		timeToSleep=3;
	}

	//Create shared memory
	if ((shmid = shmget (SHMKEY1, SHMSIZE, PERMS)) < 0) 
	{
		perror("shmem_PainterToChecking in checking level ");
		exit(1);
	}

	if ((shmid1 = shmget (SHMKEY2, SHMSIZE, PERMS)) < 0) 
	{
		perror("shmem_CheckingToCombine in checking level");
		exit(1);
	}

	Components *shmem_PainterToChecking;
	shmem_PainterToChecking=shmat(shmid,NULL,0);

	Components *shmem_CheckingToCombine;
	shmem_CheckingToCombine=shmat(shmid1,NULL,0);

	if ((semid = semget(SEMKEY, 4, PERMS)) <0) { 
		perror(" semaphores in checking level");
		exit(1);
	}


	for(i = 0; i < Y; i++){
		Sem_Down(oper,sem_check,semid);							//Depending on the type of the item that was received from painter, we set the semaphore down

		struct timeval  tv1, tv2;							//Measuring the time that the component is produced on this level
		gettimeofday(&tv1, NULL);

		Components comp;
		memcpy(&comp,shmem_PainterToChecking,sizeof(Components));
		comp.checked=1;
		usleep(timeToSleep);

		gettimeofday(&tv2, NULL);
		comp.timer=shmem_PainterToChecking->timer + (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
		if ( DEBUGFLAG == 1 )
			printf(" %s component was checked in %f time \n",comp.id, comp.timer);

		Sem_Up(oper,2,semid);								//Request another component from painter 

		Sem_Down(oper,6,semid);								//Checking Level will write the information in shared memory, therefore will be occupied 

		memcpy(shmem_CheckingToCombine, &comp,sizeof(Components));

		Sem_Up(oper,7,semid);								//Checking Level is finished writing so now the final level is ready for reading
		
	}

	exit(EXIT_SUCCESS);
}
