/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 08:22:50 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/08 08:31:22 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
#define MAIN_H

#define		_GNU_SOURCE

#include "syscall.h"
#include "sig.h"

typedef struct		main_dat_s {
	uint8_t			start;
	int				pid;
	int				status;
	uint64_t		sig;
	sys_state_t		state;
	const sysc_t	*sc;
	const sysc_t	*old_sc;
}					main_dat_t;

#endif // MAIN_H
