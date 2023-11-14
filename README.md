# testMPI
Test MPI for Distributed Systems

## To compile and launch the source files :
Compilation (on one machine, so without hostfile) :
```sh
mpicc main.c -o main
```

Execution :
```sh
mpirun -np <number_of_nodes> main
```
