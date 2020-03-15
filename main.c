#include "headers.h"

int main(int argc, char **argv){
	
		if(argc != 2){
			printf(" Error in args!\n");
			return -1;
		}

		int status;
		int shmid, shmid1, shmid2, semid;

		union semnum arg;

		//Create shared memory
		if ((shmid = shmget (SHMKEY, SHMSIZE, PERMS | IPC_CREAT)) < 0) 
		{
			perror("shmem_ConstructionToPainter in main ");
			exit(1);
		}

		if ((shmid1 = shmget (SHMKEY1, SHMSIZE, PERMS | IPC_CREAT)) < 0) 
		{
			perror("shmem_PainterToChecking in main ");
			exit(1);
		}

		if ((shmid2 = shmget (SHMKEY2, SHMSIZE, PERMS | IPC_CREAT)) < 0) 
		{
			perror("shmem_CheckingToCombine in main ");
			exit(1);
		}


		// create 8 semaphores
		if ((semid = semget(SEMKEY, 8, PERMS  | IPC_CREAT)) <0) 
		{
			perror("Semaphores in main ");
			exit(1);
		}

		Components *shmem_ConstructionToPainter, *shmem_PainterToChecking, *shmem_CheckingToCombine;
	
		shmem_ConstructionToPainter=shmat(shmid,NULL,0);
		shmem_PainterToChecking=shmat(shmid1,NULL,0);
		shmem_CheckingToCombine=shmat(shmid2,NULL,0);

		// initialize each semaphore	
		// Readers : inisialized down
		arg.val=0;
		semctl(semid, 0, SETVAL, arg);

		semctl(semid, 3, SETVAL, arg);

		semctl(semid, 4, SETVAL, arg);

		semctl(semid, 5, SETVAL, arg);

		semctl(semid, 7, SETVAL, arg);


		//Writers : initialized up
		arg.val=1;
		semctl(semid, 1, SETVAL, arg);

		semctl(semid, 2, SETVAL, arg);

		semctl(semid, 6, SETVAL, arg);

		pid_t childPID;
		int i;

		//Creating all the levels ( Construction, Painter, Checking Level and Combine)
	
		for( i = 0; i < PROCNUM; i++){				//PROCNUM is defined as 8 
			childPID = fork();												
			if(childPID == 0){
				switch(i){
					case 0:
					execlp("./construction","./construction.c", "1", argv[1], (char *) NULL);
					break;
					case 1:
					execlp("./construction","./construction.c", "2", argv[1], (char *) NULL);
					break;
					case 2:
					execlp("./construction","./construction.c", "3", argv[1], (char *) NULL);
					break;
					case 3:
					execlp("./painter","./painter.c", argv[1], (char *) NULL);
					break;
					case 4:
					execlp("./checkinglevel","./checkinglevel.c", "1", argv[1], (char *) NULL);
					break;
					case 5:
					execlp("./checkinglevel","./checkinglevel.c", "2", argv[1], (char *) NULL);
					break;
					case 6:
					execlp("./checkinglevel","./checkinglevel.c", "3", argv[1], (char *) NULL);
					break;
					case 7:
					execlp("./combine","./combine.c", argv[1], (char *) NULL);
					break;
				}		
				break;
			}
		}

		for(i = 0; i < PROCNUM; i++){
			wait(&status);
		}

		//Detach shared memories
		shmdt(shmem_ConstructionToPainter);

		if (shmctl(shmid, IPC_RMID, (struct shmid_ds *)0 ) <0) 
		{
			perror("Error with semctl");
			return -1;
		}

		shmdt(shmem_PainterToChecking);

		if (shmctl(shmid1, IPC_RMID, (struct shmid_ds *)0 ) <0) 
		{
			perror("Error with semctl");
			return -1;
		}

		shmdt(shmem_CheckingToCombine);

		if (shmctl(shmid2, IPC_RMID, (struct shmid_ds *)0 ) <0) 
		{
			perror("Error with semctl");
			return -1;
		}

		// destroy semaphore set 
		if (semctl(semid, 8, IPC_RMID, 0) <0 ) 
		{ 
			perror("Error with semctl");
			return -1;
		}
		return 0;
}
