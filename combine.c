#include "headers.h"

int main(int argc, char **argv){

	int shmid, semid;
	struct sembuf oper;

	int Y=atoi(argv[1]);
	int i;


	if ((shmid = shmget (SHMKEY2, SHMSIZE, PERMS)) < 0) 
	{
		perror("shmem_CheckingToCombine in combine ");
		exit(1);
	}

	Components *shmem_CheckingToCombine;
	shmem_CheckingToCombine=shmat(shmid,NULL,0);

	if ((semid = semget(SEMKEY, 2, PERMS)) <0) {
		perror("semaphores in combine");
		exit(1);
	}

	ListaD listType1, listType2, listType3;							//Creating three lists in order to save the item depending on their type

	CreateList(&listType1);
	CreateList(&listType2);
	CreateList(&listType3);

	for(i = 0; i < Y*3; i++){

		Sem_Down(oper,7,semid);								//Busy now , cannot accept another component while doing work

		struct timeval  tv1, tv2;							//Measuring the time that the component is produced on this level
		
		Components comp;
		memcpy(&comp,shmem_CheckingToCombine,sizeof(Components));

		if(comp.type == 1){
			InsertList(listType1, comp);
		}
		else if(comp.type == 2){
			InsertList(listType2, comp);
		}
		else{
			InsertList(listType3, comp);
		}

		if(SizeList(listType1) > 0 && SizeList(listType2) > 0 && SizeList(listType3) > 0){		//The lists are not empty so we are ready to create the product
			Components temp1, temp2, temp3;

			temp1=HeadValue(listType1);								//We take the first elements from each of them
			temp2=HeadValue(listType2);
			temp3=HeadValue(listType3);

			DeleteFirst(listType1);									//Delete that particular element 
			DeleteFirst(listType2);
			DeleteFirst(listType3);

			char *result1, *finalResult;
			result1 = concat(temp1.id, temp2.id);							//Forming the ids of the three components 
			finalResult = concat(result1, temp3.id);
			usleep(1);										//Specific time required to do the combination 
			printf(" A product with id : %s has been created successfully !\n", finalResult);

		}
		
		Sem_Up(oper,6,semid);
	}
	
	DestroyList(&listType1);
	DestroyList(&listType2);
	DestroyList(&listType3);

	exit(EXIT_SUCCESS);
}
