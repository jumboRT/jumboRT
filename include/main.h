/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.h                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:50 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:50 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include "work.h"

void			setup_sighandlers(void);
void			main_update_start(t_work *work);
void			main_update_stop(t_work *work);
void			main_image(t_work *work, const char *image_file);
void			main_run(t_options *options, t_work *work);
void			main_viewer(t_options *options);
void			main_worker(t_options *options);
void			windows_init(void);
void			windows_exit(void);
sig_atomic_t	should_exit(sig_atomic_t incr);
int				rt_key_down(int keycode, void *ctx);
void			rt_usleep(unsigned long us);
int				rt_exit(t_work *work);

#endif
