CC=		gcc
CFLAGS=		-g -gdwarf-2 -Wall -std=gnu99
LD=		gcc
LDFLAGS=	-L.
AR=		ar
ARFLAGS=	rcs
TARGETS=	search

all:		$(TARGETS)

test:		search test_search.sh
	@echo Testing $<...
	@bash ./test_search.sh

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) *.o *.log *.input

.PHONY:		all test benchmark clean

# TODO: Add rules for search and object files
execute.o: execute.c search.h
	$(CC) $(CFLAGS) -o $@ -c $<

filter.o: filter.c search.h
	$(CC) $(CFLAGS) -o $@ -c $<

main.o: main.c search.h
	$(CC) $(CFLAGS) -o $@ -c $<

search.o: search.c search.h
	$(CC) $(CFLAGS) -o $@ -c $<

utilities.o: utilities.c search.h
	$(CC) $(CFLAGS) -o $@ -c $<

search: execute.o filter.o main.o search.o utilities.o
	$(LD) $(LDFLAGS) -o $@ $
