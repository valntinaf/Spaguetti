# Spaguetti

Matrix multiplication normal implementation, as well as processes and multithreading.

## Generating a matrix
```bash
./generateMatrix 10 matrix_a
./generateMatrix 10 matrix_b
```

## Normal multiplication

> ./mulmat _generatedMatrix_ _generatedMatrix_ _outputFile_ 

```bash
./mulmat 10 matrix_a matrix_b matrix_c
```

## Multi threading multiplication

> ./mulmathil _generatedMatrix_ _numberOfThreads_ _generatedMatrix_ _outputFile_ 

```bash
./mulmathil 10 5 matrix_a matrix_b matrix_c
```

## Multi processing multiplication

> ./mulmatproc _generatedMatrix_ _numberOfProcesses_ _generatedMatrix_ _outputFile_ 

```bash
./mulmatproc 10 5 matrix_a matrix_b matrix_c
```

## Copyright
Developed by ![@valntinaf](https://github.com/valntinaf) and Omar Espinel.
