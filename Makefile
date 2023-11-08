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
	@./test_search.sh

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) *.o *.log *.input

.PHONY:		all test benchmark clean

# TODO: Add rules for search and object files
