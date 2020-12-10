CC=gcc
OBJS1=event_manager_example_tests.o date.o event.o event_manager.o member.o member_list.o priority_queue.o 
OBJS2=pq_example_tests.o date.o event.o event_manager.o member.o member_list.o priority_queue.o 
EXEC= event_manager priority_queue
CFLAGS=-std=c99 -Wall -Werror -pedantic-errors -DNDEBUG $(DEBUG) 



event_manager: $(OBJS1)
	$(CC) $(DEBUG) $(OBJS1) -o event_manager
priority_queue: $(OBJS2)
	$(CC) $(DEBUG) $(OBJS2) -o priority_queue

date.o: date.c date.h
event.o: event.c event.h date.h member.h priority_queue.h member_list.h
event_manager.o: event_manager.c event_manager.h date.h priority_queue.h \
 					member_list.h member.h event.h
member.o: member.c member.h priority_queue.h
member_list.o: member_list.c member_list.h member.h priority_queue.h
priority_queue.o: priority_queue.c priority_queue.h
event_manager_example_tests.o: tests/event_manager_example_tests.c \
 								tests/test_utilities.h event_manager.h date.h
							$(CC) -c $(DEBUG) $(CFLAGS) tests/$*.c
pq_example_tests.o: tests/pq_example_tests.c tests/test_utilities.h priority_queue.h
							$(CC) -c $(DEBUG) $(CFLAGS) tests/$*.c

clean:	rm -f $(OBJS) $(EXEC)