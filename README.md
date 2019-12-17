# Hand-written number recognition
 - Implement the Artificial Neural Network, Machine-Learning with C

## build
```sh
 $make
```

## clean files
```sh
 $make clean
```

## add object
 OBJECTS = main.o {added object name} #in Makefile

## Diagram
 https://drive.google.com/drive/folders/1l0glLN2NOzfIU0XoJTe6JGRZfi9qcNMg?usp=sharing 

## Build without Makefile
```sh
 $gcc {filename} -o {binary name}
```
  - options
    -g : add the debugging information

## For memory test
 ```sh
 $_valgrind {binary name} {arguments}  - for memory leak check
 $pmap {PID}                           - while running process
 ```