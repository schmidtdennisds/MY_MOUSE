all : my_mouse

my_mouse : my_mouse.c
	gcc -o my_mouse my_mouse.c

clean:
	rm -f *.o

fclean: clean
	rm -f my_mouse

re: fclean all
