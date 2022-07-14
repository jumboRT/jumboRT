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
# else
#  define RT_KEY_ESC 53
#  define RT_KEY_Q 12
#  define RT_KEY_R 15
#  define RT_KEY_W 13
#  define RT_KEY_S 1
#  define RT_KEY_A 0
#  define RT_KEY_D 2
# endif

#endif
