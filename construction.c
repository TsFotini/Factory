#include "headers.h"

int main(int argc, char **argv){

	int shmid, semid;
	struct sembuf oper;

	int type=atoi(argv[1]);
	int Y=atoi(argv[2]);
	int i;
	char finalID[50];

	if ((shmid = shmget (SHMKEY, SHMSIZE, PERMS)) < 0) 
	{
		perror("shmem_ConstructionToPainter in construction ");
		exit(1);
	}

	Components *shmem_ConstructionToPainter;
	shmem_ConstructionToPainter=shmat(shmid,NULL,0);

	srand(time(NULL));

	if ((semid = semget(SEMKEY, 2, PERMS)) <0) {		//Two semaphores required
		perror("semaphores in construction ");
		exit(1);
	}
	for(i=0;i<Y;i++){
		Sem_Down(oper,1,semid);				//Firstly, semaphore down in order to write to the shared memory the component that is produced

		struct timeval  tv1, tv2;			//Measuring the time that the component is produced on this level
		gettimeofday(&tv1, NULL);

		Components comp;
		comp.type=type;

		char str[4];					// Creating ids
		strcpy(str,randomString(4));
		char hold_id[6];

		if(type == 1){
			strcpy(hold_id,"type1");
			strcpy(finalID,concat(hold_id,str));
		}
		else if (type == 2){
			strcpy(hold_id,"type2");
			strcpy(finalID,concat(hold_id,str));
		}
		else{
			strcpy(hold_id,"type3");
			strcpy(finalID,concat(hold_id,str));
		}
		strcpy(comp.id,finalID);
		if(DEBUGFLAG == 1)
			printf("Construcrtion id is : %s of type %d \n",comp.id,type);
		comp.painted=0;
		comp.checked=0;
		gettimeofday(&tv2, NULL);
		comp.timer=(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
		if( DEBUGFLAG == 1)
			printf(" %s component was produced in %f time \n",comp.id,comp.timer);

		memcpy(shmem_ConstructionToPainter,&comp,sizeof(Components));
		
		Sem_Up(oper,0,semid);				//Ready to read the infromation from the shared memory in next level
	}

	exit(EXIT_SUCCESS);

}
