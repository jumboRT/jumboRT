#ifndef KEYCODE_H
# define KEYCODE_H

# ifdef RT_LINUX
#  define RT_KEY_ESC 65307
#  define RT_KEY_Q 113
#  define RT_KEY_R 114
# else
#  define RT_KEY_ESC 53
#  define RT_KEY_Q 12
#  define RT_KEY_R 15
# endif

#endif
