CC=gcc
FLAGS = -Wall -Wextra
	
all: my_cat

my_cat:
	$(CC) $(FLAGS) -o ./my_cat ./*.c

test: rebuild
	./test_func_cat.sh
	
leak: rebuild
	./test_leak_cat.sh

valgrind: rebuild
	./test_valgrind_cat.sh

check: rebuild
	python3 ../../cpplint.py --extensions=c,h ./*.c ./*.h
	clang-format -i *.c 
	clang-format -i *.h

clean: clean_cat

clean_cat:
	rm -f ./*.o ./*.a ./*.so ./my_cat ./*.log

rebuild: clean all
