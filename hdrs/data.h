/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:10:27 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/13 17:33:35 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
#define DATA_H

#define ARG_NB		6
#define SYSC_NB		401

#include "args.h"

typedef enum		sys_state_e {
	S_CALL,
	S_RET,
	S_NONE,
}					sys_state_t;

typedef struct		sysc_s {
	char			*name;
	int				id[ARCH_NB];		
	arg_typ_t		args[ARG_NB];
	arg_typ_t		ret;
	sys_state_t		pstate;
}					sysc_t;

extern const sysc_t		SYSC[SYSC_NB];

#endif // DATA_H
