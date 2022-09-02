#ifndef KEYCODE_H
# define KEYCODE_H

# ifdef RT_LINUX
#  define RT_KEY_ESC 65307
#  define RT_KEY_Q 113
#  define RT_KEY_R 114
#  define RT_KEY_W 119
#  define RT_KEY_S 115
#  define RT_KEY_A 97
#  define RT_KEY_D 100
#  define RT_KEY_UP 65362
#  define RT_KEY_Z 122
#  define RT_KEY_X 120
#  define RT_KEY_DOWN 65364
#  define RT_KEY_LEFT 65361
#  define RT_KEY_RIGHT 65363
#  define RT_KEY_SPACE 32
#  define RT_KEY_SHIFT 65505
#  define RT_KEY_1 49
#  define RT_KEY_2 50
#  define RT_KEY_3 51
#  define RT_KEY_4 52
# else
#  define RT_KEY_ESC 53
#  define RT_KEY_Q 12
#  define RT_KEY_R 15
#  define RT_KEY_W 13
#  define RT_KEY_S 1
#  define RT_KEY_A 0
#  define RT_KEY_D 2
#  define RT_KEY_UP 126
#  define RT_KEY_DOWN 125
#  define RT_KEY_Z 6
#  define RT_KEY_X 7
#  define RT_KEY_LEFT 123
#  define RT_KEY_RIGHT 124
#  define RT_KEY_SPACE 49
#  define RT_KEY_SHIFT 56
#  define RT_KEY_1 18
#  define RT_KEY_2 19
#  define RT_KEY_3 20
#  define RT_KEY_4 21
# endif

#endif
