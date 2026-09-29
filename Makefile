CC = gcc
CFLAGS = -Wall -Wextra

q4: src/q4_expression_tree.c
	$(CC) $(CFLAGS) -o q4 src/q4_expression_tree.c

run: q4
	./q4 input/input.txt | tee output/output.txt

clean:
	rm -f q4
