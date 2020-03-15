#include "headers.h"

void Sem_Up(struct sembuf oper,int semNum, int semid){
	oper.sem_flg  = 0;
	oper.sem_num  = semNum;
	oper.sem_op   = 1;				// This value is the one that turn on the semaphore
	if(semop(semid, &oper, 1) == -1)
	{
		perror("semop1");
		exit(1);
	}
}

void Sem_Down(struct sembuf oper,int semNum, int semid){
	oper.sem_flg  = 0;
	oper.sem_num  = semNum;
	oper.sem_op   = -1;			// This tells us that the semaphore is down
	if(semop(semid, &oper, 1) == -1)
	{
		perror("semop1");
		exit(1);
	}
}

int intN(int n) { return rand() % n; }

char *randomString(int len) {
	const char alphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	char *rstr = malloc((len + 1) * sizeof(char));
	char *s;
	int i;
	for (i = 0; i < len; i++) {
	    rstr[i] = alphabet[intN(strlen(alphabet))];
	}
	rstr[len] = '\0';
	return rstr;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
