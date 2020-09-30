# A demonstration of dekker's algorithm failing in real computers

[Dekker's algorithm](https://en.wikipedia.org/wiki/Dekker%27s_algorithm) is known as the first correct solution to mutual exclusion - the idea that only one process should excute a critical section at a time.

Dekker's algorithm has been proven to work, assuming [Sequencial Consistency](https://en.wikipedia.org/wiki/Sequential_consistency). However, real computers ***Do not*** preserve sequencial consistency, and instead preserve [Processor Consistency](https://en.wikipedia.org/wiki/Processor_consistency).

In the following repository, there is a simple C program that demonstrates that Dekker's algorithm fails in real world computers.


## How to run it
There is a program called `dekker_fail.c` in this repository, all you need is to compile and run it.

To compile it, run
```bash
gcc dekker_fail.c -o <name_of_output> -lpthread
```

Then simply run the resulting program:
```bash
./<name_of_output>
```

You'll see one of two messages, either a message stating that mutual exclusion was violated since both threads were inside the critical section, or a message stating that everything was okay and mutual exclusion was preserved.

## Wait, mutual exclusion is preserved everytime for me!!!
Well uhhhh it worked on my device? Kidding aside, I would check the properties of the system you are running this on and confirm that it's a multicore system (well... it doesn't *have to be* multicore, but with multiple cores the chances are higher that two cores are executing the same time and cause issues for each other)


## How do I limit the cores to play around with this?
In linux systems you can use `taskset` to change the CPU affinity of a process. So to run the process with 1 core try:
```bash
taskset -c 1 ./<name_of_output>
```

You can try playing around with the cpu affinity and see how likely you are to break Dekker's algorithm.


Happy coding!
