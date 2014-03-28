// -- prod1.c -- Programme "producteur" qui écrit dans le segment partage
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>

typedef struct snombre
{
	float x;
	float y;
	float z;
} SNOMBRE;

union semun 
{
	int val;
	struct semid_ds* buf;
	ushort array[1];
};

void SIGINT_handler (int num_signal);
typedef int SEMAPHORE;

int globale = 1;

int main (void)
{
	union semun arg;
	key_t cle, cle_2;
	int id;
	SNOMBRE *commun;
	pid_t pid, pid2;
	int valsem, res_semctl;
	struct sembuf sb[1];
	
	signal (SIGINT, SIGINT_handler);
	
	//Création des clés
	cle = ftok ("./pointrdv", 'A');
	cle_2 = ftok ("./pointrdv", 'B');
	
	if (cle == -1)
	{
		printf ("erreur de la creation de la cle\n"); exit (1);
	}
	
	printf ("La cle 1 vaut %d %x \n", cle, cle);
	
	if (cle_2 == -1)
	{
		perror("ftok sem");
		exit(1);
	}
	
	printf("cle sempaphore %d cree \n", cle_2);
	
	//Création de l'indentificateur du segment de mémoire partagée
	id = shmget(cle, sizeof(SNOMBRE), IPC_CREAT | IPC_EXCL | 0666);
	if (id == -1)
	{
		switch (errno) 
		{
			case EEXIST :
					printf ("erreur ; l'indentificateur existe deja \n");
					break;
					
			default : 
					printf ("erreur de creation de l'indentificateur \n");
					break;
		}
		
		exit (2);
	}
	printf ("L'indentificateur vaut : %d %x \n", id, id);
	
	//Attachement du segment de mémoire partagée à un pointeur
	commun = (SNOMBRE*) shmat(id, NULL, SHM_R | SHM_W);
	if (commun == NULL)
	{
		printf ("erreur de  lors de l'attachement du segment \n");
		exit (3);
	}
	
	printf ("Le pointeur vaut %d, %x \n", commun, commun);
	
	//Création du sémaphore
	SEMAPHORE sem;
	sem = semget(cle_2, 1, IPC_CREAT | 0666);
	
	if (sem < 0)
	{
		perror("creer_sem");
		exit (EXIT_FAILURE);
	}
	
	printf ("Semaphore cree ; sem = %d \n", sem);
	
	//Initialisation du sémaphore
	arg.val = 1;
	res_semctl = semctl(sem, 0, SETVAL, arg);
	
	if (res_semctl == -1)
	{
		perror ("initialisation sémaphore ratée");
		exit (EXIT_FAILURE);
	}
	
	printf ("Sémaphore initialisé \n");
	
	if((valsem=semctl(sem, 0, GETVAL)) == -1)
	{
		perror("smctl");
		exit(33);
	}
	
	printf ("Valeur du sémaphore après initialisation : %d \n", valsem);
	
	pid = fork(); //Duplication du contexte
	
	if (pid > 0) 
	{
		pid2 = fork(); //Création 2ème fils
		
		if (pid2 > 0) //Père
		{
			while (globale)
			{
				sb[0].sem_num = 0;
				sb[0].sem_flg = 0;
				sb[0].sem_op = -1;
				
				if (semop(sem, sb, 1) != 0)
				{
					perror("Erreur de modification du semaphore");
					exit(EXIT_FAILURE);
				}
				
				//printf ("Le père modifie les valeurs \n\n");
				commun->x = 2;
				sleep(1);
				commun->y = 2;
				sleep(1);
				commun->z = 2;
				
				
			
				sb[0].sem_num = 0;
				sb[0].sem_flg = 0;
				sb[0].sem_op = +1;
				
				if (semop(sem, sb, 1) != 0)
				{
					perror("Erreur de modification du semaphore");
					exit(EXIT_FAILURE);
				}
				
				sleep(5);
			}
			
			printf ("[%d] Je suis le père, je meurs... \n\n", getpid());
		
		}
		
		else //2ème fils
		{
			int x = 2, y = 2, z = 2; //Locale
			
			while(globale)
			{

				sb[0].sem_num = 0;
				sb[0].sem_flg = 0;
				sb[0].sem_op = -1;
				
				if (semop(sem, sb, 1) != 0)
				{
					perror("Erreur de modification du semaphore");
					exit(EXIT_FAILURE);
				}
				
				//printf ("Le fils2 lit les valeurs \n\n");
				
				
				if (x != commun->x || y != commun->y || z != commun->z)
				{
					x = commun->x;
					y = commun->y;
					z = commun->z;
					
					printf ("Valeur de commun : x = %d  y = %d  z = %d  \n", x, y , z);
				}
				
				sb[0].sem_num = 0;
				sb[0].sem_flg = 0;
				sb[0].sem_op = +1;
				
				if (semop(sem, sb, 1) != 0)
				{
					perror("Erreur de modification du semaphore");
					exit(EXIT_FAILURE);
				}
			}
			
			printf ("[%d] Je suis le fils2, je meurs... \n\n", getpid());
		}		
	} 
	
	else if (pid == 0) //1er fils
	{
		while (globale)
		{
			sb[0].sem_num = 0;
			sb[0].sem_flg = 0;
			sb[0].sem_op = -1;
			
			if (semop(sem, sb, 1) != 0)
			{
				perror("Erreur de modification du semaphore");
				exit(EXIT_FAILURE);
			}
			
			//printf ("Le fils1 modifie les valeurs \n\n");
			commun->x = 10;
			sleep(1);
			commun->y = 10;
			sleep(1);
			commun->z = 10;
			
			
		
			sb[0].sem_num = 0;
			sb[0].sem_flg = 0;
			sb[0].sem_op = +1;
			
			if (semop(sem, sb, 1) != 0)
			{
				perror("Erreur de modification du semaphore");
				exit(EXIT_FAILURE);
			}
			
			sleep(1);
		}
		
		printf ("[%d] Je suis le fils1, je meurs... \n\n", getpid());
	} 
	
	//Détachement
	if (shmdt((char*)commun) == -1)
	{
		printf ("erreur lors du détachement du segment \n");
		exit (4);
	}
	
	
	if (pid > 0 && pid2 > 0)
	{
		waitpid (pid, 0, WNOHANG);
		waitpid (pid2, 0, WNOHANG);
		
		//Suppression
		if (shmctl(id, IPC_RMID, NULL) == -1)
		{
			printf ("erreur lors de la suppression du segment \n");
			exit (5);
		} 
	}
	
	return 0;
}

void SIGINT_handler (int num_signal)
{
	globale = 0;
	printf ("[%d] J'ai recu le signal ! \n\n", getpid());
}

	
