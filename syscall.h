/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 10:05:26 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/25 16:42:47 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSCALL_H
#define SYSCALL_H

#include "data.h"

const sysc_t	*sysc_get(void *regs);
int				sysc_print(const sysc_t *sc, void *regs, int pid);
void			sysc_ret_print(const sysc_t *sc, void *regs);

#endif
