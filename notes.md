https://www.codequoi.com/threads-mutex-et-programmation-concurrente-en-c/

https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2

https://m4nnb3ll.medium.com/the-dining-philoshophers-an-introduction-to-multitasking-a-42-the-network-project-34e4141dbc49

https://github.com/48d31kh413k/1337-Philosopher-42

https://github.com/pasqualerossi/Philosophers/




1 or more philo autour d'une table.

1 bowl de spaghetti au centre.

les philos peuvent soit manger, penser ou dormir.

ils ne peuvent faire qu'une seule des 3 choses a la fois.

il y a des fourchettes, il y en a autant qu'il y a de philos. (6 philos 6 forks).

pour manger un philo doit utiliser 2 forks, celle a sa gauche et sa droite.

1 fois qu un philo a fini de manger, il repose les forks et dort.

des qu'il se reveille, il se met a penser.

tout s'arrete quand un philo meurt de faim.

tous les philos doivent manfer et ne devraient jamais mourir de faim.

ils ne se parlent pas, ils ne savent pas si l un d entre eux va mourir.

ils doivent eviter de mourir.


s'il y a 5 philo, seuls 2 peuvent manger en meme temps. (2x2 = 4 donc 1 restante inutile)



global variables interdites !


./philo number_of_philosophers time_to_die (in milliseconds) time_to_eat (in milliseconds) time_to_sleep (in milliseconds) number_of_times_each_philosopher_must_eat (optional argument)


number_of_philosophers					nbre de philos et donc de forks
time_to_die (in milliseconds)			le temps entre chaque repas
time_to_eat (in milliseconds)			la duree d'un repas, durant lequel il a 2 forks
time_to_sleep (in milliseconds)			la duree durant laquelle un philo dort
number_of_times_each_philosopher_must_eat (optional argument)
										nombre de fois min que chaque philo mange avant que la simu s'arrete
										si pas indique, la simu s arrete quand un philo meurt

chaque philo a un numero entre 1 et total philos

philo 1 est assis a cote de philo max\
philo n est assis entre philo n-1 et n+1


les logs :

timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died

X is the philo number

les logs ne doivent pas se mixer donc mutex

l annocne de la mort d un philo doit se faire au max 10ms apres la mort du philo

no data races in the program

chaque philo doit etre un thread

proteger les fork state avec un mutex



threads :


#include <pthread.h> + ajouter le flag -pthread


pour creer un thread :

on utilise la fonction pthread_create


```int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg);```

thread : un pointeur vers une variable de type pthread_t pour stocker l’identifiant du thread qu’on va créer.
attr : un argument qui permet de changer les attributs par défaut du nouveau thread lors de sa création. (On indique NULL ici)
start_routine : la fonction par laquelle le thread commence son exécution. Cette fonction doit avoir pour prototype void *nom_de_fonction_au_choix(void *arg);. Lorsque le thread arrive à la fin de cette fonction, il aura terminé toutes ses tâches.
arg : le pointeur vers un argument à transmettre à la fonction start_routine du thread. Si l’on souhaite passer plusieurs paramètres à cette fonction, il n’y a pas d’autre choix que de lui renseigner ici un pointeur vers une structure de données.





