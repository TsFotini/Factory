#include "headers.h"

int main(int argc, char **argv){

	int shmid, shmid1, semid;
	struct sembuf oper;

	int Y=atoi(argv[1]);
	int i;

	struct timeval  tv1, tv2;							//Measuring the time that the component is produced on this level
	gettimeofday(&tv1, NULL);

	if ((shmid = shmget (SHMKEY, SHMSIZE, PERMS)) < 0) 
	{
		perror("shmem_ConstructionToPainter in painter ");
		exit(1);
	}
	if ((shmid1 = shmget (SHMKEY1, SHMSIZE, PERMS)) < 0) 
	{
		perror("shmem_PainterToChecking in painter ");
		exit(1);
	}


	Components *shmem_ConstructionToPainter, *shmem_PainterToChecking;
	shmem_ConstructionToPainter=shmat(shmid,NULL,0);
	shmem_PainterToChecking=shmat(shmid1,NULL,0);

	if ((semid = semget(SEMKEY, 6, PERMS)) <0) {
		perror("semaphores in painter ");
		exit(1);
	}

	
		
	for(i=0;i<Y*3;i++){
		Sem_Down(oper,0,semid);								//The painter is occupied so the semaphore is down

		Components comp;
		shmem_ConstructionToPainter->painted=1;

		struct timeval  tv1, tv2;							//Measuring the time that the component is produced on this level
		gettimeofday(&tv1, NULL);
		
		memcpy(&comp,shmem_ConstructionToPainter,sizeof(Components));			//Copy the contents of shered memory to a temporary Component
		if(comp.type == 1){
			usleep(1);								//The items of different types are painted in different times as well
			gettimeofday(&tv2, NULL);
			shmem_ConstructionToPainter->timer=shmem_ConstructionToPainter->timer + (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
			if ( DEBUGFLAG == 1 )
				printf(" %s component was painted in %f time \n",shmem_ConstructionToPainter->id, shmem_ConstructionToPainter->timer);
			
		}
		else if(comp.type == 2){
			usleep(2);
			gettimeofday(&tv2, NULL);
			shmem_ConstructionToPainter->timer=shmem_ConstructionToPainter->timer + (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
			if ( DEBUGFLAG == 1 )
				printf(" %s component was painted in %f time \n",shmem_ConstructionToPainter->id, shmem_ConstructionToPainter->timer);
		}
		else{
			usleep(3);
			gettimeofday(&tv2, NULL);
			shmem_ConstructionToPainter->timer=shmem_ConstructionToPainter->timer + (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
			if ( DEBUGFLAG == 1)
				printf(" %s component was painted in %f time \n",shmem_ConstructionToPainter->id, shmem_ConstructionToPainter->timer);
		}

		Sem_Up(oper,1,semid);								//Painter is finished now and expects another component, so the construction level should send it

		Sem_Down(oper,2,semid);								//Painter writing to the shared memory the information that he has , in order to pass it to the checking level

		memcpy(shmem_PainterToChecking,&comp,sizeof(Components));			

		if(shmem_PainterToChecking->type == 1){
			Sem_Up(oper,3,semid);							//Painter checks the type of the object and than sends it to the checking level that it belongs to
		}
		else if(shmem_PainterToChecking->type == 2){
			Sem_Up(oper,4,semid);
		}
		else{
			Sem_Up(oper,5,semid);
		}

	}
	gettimeofday(&tv2, NULL);
	double dTimeStart = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
	
	printf("Painter was occupied for : %f seconds \n",dTimeStart);

	exit(EXIT_SUCCESS);

}
