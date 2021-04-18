#include <stdio.h>	  /* printf */
#include <unistd.h>	  /* fork */
#include <sys/wait.h> /* wait */
#include <sys/shm.h>  /* shmget shmat shmdt shmctl */

int allocate_shared_memory(size_t size)
{
	return shmget(IPC_PRIVATE,
				  size,
				  IPC_CREAT | SHM_R | SHM_W);
}
void *attach_pointer_to_shared_memory(int shared_memory_id)
{
	return shmat(shared_memory_id, NULL, 0);
}
int detach_pointer_to_shared_memory(const void *pointer)
{
	return shmdt(pointer);
}
int free_shared_memory(int shared_memory_id)
{
	return shmctl(shared_memory_id, IPC_RMID, NULL);
}

int main()
{
	int shared_memory_id;
	int process_id;
	int *pointer;

	shared_memory_id = allocate_shared_memory(sizeof(int));

	process_id = fork();
	if (process_id > 0)
	{
		wait(&process_id);
		pointer = (int *)attach_pointer_to_shared_memory(shared_memory_id);
		printf("Parent: %d\n", *pointer);
		detach_pointer_to_shared_memory(pointer);
		free_shared_memory(shared_memory_id);
	}
	else if (process_id == 0)
	{
		pointer = (int *)attach_pointer_to_shared_memory(shared_memory_id);
		*pointer = 123;
		printf("Child: %d\n", *pointer);
		detach_pointer_to_shared_memory(pointer);
		_exit(0);
	}
	else
	{
		printf("fork failed");
	}
	return 0;
}
