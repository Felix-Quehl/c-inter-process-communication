# Example Project for IPC with a Child Process in C

This example contains a single `main.c` file show-casing cloning the execution into a child process via the `fork` function along with shared memory access though the `shmget`, `shmat`, `shmdt` and `shmctl` function.

[Click here to view the source code of the main.c](src/main.c)

Links to the linux function documentation:

* [shmget](https://man7.org/linux/man-pages/man2/shmget.2.html)
* [shmat](https://man7.org/linux/man-pages/man2/shmat.2.html)
* [shmdt](https://man7.org/linux/man-pages/man2/shmdt.2.html)
* [shmctl](https://man7.org/linux/man-pages/man2/shmctl.2.html)

## Build and Run

This repo is ready to compile and features a full example.

```bash
# install packages
sudo apt install make gcc
# use make to build a target
make clean debug
# run the program
./app.debug
```

## How to

### How to fork/clone the main/parent process into a concurrently executed child process

```c
// variable to hold the return value of the fork
// function, which is either the child process id
// or some status code. It depends on the success
// or failure of the fork function and also differs
// in child process execution compared to the
// parent process's execution. 
int process_id;

// call fork() to clone the current process
// into a parent process and a child process
// both processes will continue/start executing
// the very same code base.
// There for we will need to control the
// program flow for both of them individually
// using the return value of fork in
// if/else-if/else tree
process_id = fork();

// in the parent process the process_id variable
// will hold the process id of the child process
if (process_id > 0)
{
    // do something as the parent
}

// in the child process the process_id variable
// will hold a zero
else if (process_id == 0)
{
    // do something as a child process
}

// When the parent process was unable to fork
// into a child process, the fork function
// will return a negative 1 (-1) value
// indicating failure to fork
// the program flow of the parent/main thread
// will just continue but we probably want some
// special error handling when the child process
// was not created
else
{
    // fork failed, do some error handling
}
```

### Inter process communication with a child process

Every process, parent and child, does have its own memory/address-space.  
No matter whether the are stack or heap variables.  
Not even pointer that are targeting the same memory address  
are actually accessing the same psychical memory location.
There are multiple solutions to implement inter process communication.

In this example we are going to use a System V shared memory segment.
These are the corresponding functions:

* [shmget](https://man7.org/linux/man-pages/man2/shmget.2.html)
* [shmat](https://man7.org/linux/man-pages/man2/shmat.2.html)
* [shmdt](https://man7.org/linux/man-pages/man2/shmdt.2.html)
* [shmctl](https://man7.org/linux/man-pages/man2/shmctl.2.html)

*There are other/new solutions to implement inter process communication. Maybe these solutions are more suitable for your problem. So before settling on a technology please checkout the linux manual sections about [shared memory](https://man7.org/linux/man-pages/man7/shm_overview.7.html), [pipes](https://man7.org/linux/man-pages/man2/pipe.2.html) and [sockets](https://man7.org/linux/man-pages/man2/socket.2.html).*

```c
// this variable will hold the id of the shared memory objects
int shared_memory_id;
// to access the memory we are going to need a pointer, this is it
// we want to share only a single integer value
// in a real implementation you a probably want a struct
int *pointer;

// we need to initialize the shared memory with the shmget function
// the size is corresponding to the size of the data we want to share
// in this case a single integer
// this normally only done on the parent process before the fork
// so when forking both the child and parent process already has the shared_memory_id
shared_memory_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | SHM_R | SHM_W);

// once we have initialized the shared memory we can attach in via a pointer
// this call is done after the fork by both the child and parent process
pointer = (int *)shmat(shared_memory_id, NULL, 0);

// the pointer is working like any other normal pointer
*pointer = 123;
printf("Value is %d", *pointer);

// when we not longer need to access the memory we can detach it with the shmdt function
// this done in the child process and the parent process
shmdt(pointer);

// when we no longer need to share memory we can completely remove it with a shmctl function call
// performing a shared memory control call that has the IPC_RMID flag set indicating we want to
// remove the shared memory object
//
// this is normally done inside the parent process only !
shmctl(shared_memory_id, IPC_RMID, NULL);
```
