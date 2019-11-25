CC = gcc
TARGET = hwr
THREAD = -lpthread -D _USE_THREAD
OBJECTS = main.o 

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $^ $(THREAD) 

 : $(OBJECTS)
	$(CC) -c -o $@ $<

clean : 
	rm *.o hwr 



