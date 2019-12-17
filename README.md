# Hand-written number recognition
 - Implement the Artificial Neural Network, Machine-Learning with C

## build
 $make

## clean files
 $make clean

## add object
 OBJECTS = main.o {added object name} #in Makefile

## Diagram
 https://drive.google.com/drive/folders/1l0glLN2NOzfIU0XoJTe6JGRZfi9qcNMg?usp=sharing 

## Build without Makefile
 gcc {filename} -o {binary name}
  - options
    -g : add the debugging information

## For memory test
 _valgrind {binary name} {arguments}  - for memory leak check
 pmap {PID}                           - while running process