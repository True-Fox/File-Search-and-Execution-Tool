gcc -g -gdwarf-2 -Wall -std=gnu99 -c -o main.o main.c
echo "Compiled main"
gcc -g -gdwarf-2 -Wall -std=gnu99 -c -o execute.o execute.c
echo "Compiled execute"
gcc -g -gdwarf-2 -Wall -std=gnu99 -c -o filter.o filter.c
echo "compiled filter"
gcc -g -gdwarf-2 -Wall -std=gnu99 -c -o search.o search.c
echo "compiled search"
gcc -g -gdwarf-2 -Wall -std=gnu99 -c -o utilities.o utilities.c
echo "compiled utilities"
gcc -L. -o search main.o execute.o filter.o search.o utilities.o
echo "Linked object files"