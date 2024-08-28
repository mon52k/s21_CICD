CC=gcc
FLAGS=-Wall -Werror -Wextra -std=c11 -pedantic
#LINUX_FLAGS=-lsubunit -lrt -lpthread -lm



s21_cat:
	$(CC) $(FLAGS) *.c -o s21_cat
test: s21_cat
	bash s21_cat_test.sh
style:
	clang-format -style=Google  -n *.c *.h
	clang-format -style=Google -i *.c *.h
	rm .clang-format
clean:
	rm s21_cat
	clear
leaks: s21_cat
	leaks -atExit -- ./s21_cat -benstv testfile1.txt

valgrind:
	cd ../.. && cd materials/build/valgrind && bash run.sh

rebuild:
	rm -rf s21_cat
