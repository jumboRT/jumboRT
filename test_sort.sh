#!/bin/sh
gcc src/vector/*.c src/util/*.c src/mt/*.c src/math/*.c -Iinclude -Idependencies/libft -Idependencies/ft_printf dependencies/libft/libft.a dependencies/ft_printf/libftprintf.a -lm -Ofast -flto -march=native || exit 1
./a.out > /tmp/sort_a.txt
sort /tmp/sort_a.txt > /tmp/sort_b.txt
diff /tmp/sort_a.txt /tmp/sort_b.txt
