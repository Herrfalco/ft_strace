/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:55:52 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/01 23:13:12 by fcadet           ###   ########.fr       */
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
	AT_R,
}					arg_typ_t;

typedef int			(*at_print_t)(uint64_t, int);

extern const at_print_t		AT_PRINT[ARG_TYP_NB];
extern const col_t			AT_COL[ARG_TYP_NB];

int				at___print(uint64_t reg_val, int mem_fd);
int				at_u_print(uint64_t reg_val, int mem_fd);
int				at_i_print(uint64_t reg_val, int mem_fd);
int				at_p_print(uint64_t reg_val, int mem_fd);
int				at_o_print(uint64_t reg_val, int mem_fd);
int				at_s_print(uint64_t reg_val, int mem_fd);
int				at_l_print(uint64_t reg_val, int mem_fd);
int				at_c_print(uint64_t reg_val, int mem_fd);

#endif // ARGS_H
