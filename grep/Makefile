s21_grep:
	gcc -std=c11 -pedantic -Wall -Werror -Wextra s21_grep.c -o s21_grep
test: s21_grep
	bash test.sh
style:
	clang-format -style=Google -n *.c *.h
	clang-format -style=Google -i *.c *.h

clean:
	rm s21_grep
	clear
	
leaks:	s21_grep
	leaks -atExit -- ./s21_cat -lovei testfile1.txt
	
valgrind:
	cd ../.. && cd materials/build/valgrind && bash run.sh
