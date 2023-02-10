/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 10:05:26 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/10 09:05:50 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSCALL_H
#define SYSCALL_H

#include "data.h"

const sysc_t	*sysc_get(void *regs);
void			sysc_name_print(const sysc_t *sc);
int				sysc_restart(const sysc_t *sc, const sysc_t *o_sc);
int				sysc_args_print(const sysc_t *sc, void *regs, pid_t pid);
void			sysc_ret_print(const sysc_t *sc, void *regs);

#endif
