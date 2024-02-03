all : my_mouse

my_mouse : my_mouse.c
	gcc -o my_mouse my_mouse.c -I.

clean:
	rm -f *.o

fclean: clean
	rm -f my_mouse

re: fclean all
