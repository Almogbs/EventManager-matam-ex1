CC=gcc
OBJS=pq_example_tests.o event_manager_example_tests.o
EXEC=prog
DEBUG=
CFLAGS=-std=c99 -Wall -Werror -pedantic-errors $(DEBUG) 

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o 
	
$@
pq_example_tests.o: pq_example_tests.c test_utilities.h priority_queue.h
event_manager_example_tests.o: event_manager_example_tests.c \
 								test_utilities.h event_manager.h date.h

clean:	rm -f $(OBJS) $(EXEC)