/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:55:52 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/30 09:15:54 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
#define ARGS_H

#include "col.h"

#define MAX_LC				4096
#define ARG_TYP_NB			9

typedef enum		arg_typ_e {
	AT__,
	AT_U,
	AT_I,
	AT_P,
	AT_O,
	AT_S,
	AT_L,
	AT_C,
	AT_X,
	AT_E,
}					arg_typ_t;

typedef int			(*at_print_t)(void *, int);

const at_print_t	AT_PRINT[ARG_TYP_NB];
const col_t			AT_COL[ARG_TYP_NB];

int				at___print(void *reg, int mem_fd);
int				at_u_print(void *reg, int mem_fd);
int				at_i_print(void *reg, int mem_fd);
int				at_p_print(void *reg, int mem_fd);
int				at_o_print(void *reg, int mem_fd);
int				at_s_print(void *reg, int mem_fd);
int				at_l_print(void *reg, int mem_fd);
int				at_c_print(void *reg, int mem_fd);

#endif // ARGS_H
