/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:55:52 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/13 12:36:26 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
#define ARGS_H

#include "col.h"
#include "error.h"

#define MAX_LC				4096
#define ARG_TYP_NB			12

typedef enum		arg_typ_e {
	AT__,
	AT_U,
	AT_I,
	AT_H,
	AT_P,
	AT_O,
	AT_S,
	AT_SL,
	AT_B,
	AT_L,
	AT_C,
	AT_X,
	AT_E,
	AT_R,
}					arg_typ_t;

typedef struct		read_lim_s {
	uint64_t		val;
	uint8_t			set;
}					read_lim_t;

typedef ret_t		(*at_print_t)(uint64_t, int64_t);

extern const at_print_t		AT_PRINT[ARG_TYP_NB];
extern const col_t			AT_COL[ARG_TYP_NB];

void			args_set_rlim(uint64_t val);

ret_t			at___print(uint64_t reg_val, int64_t mem_fd);
ret_t			at_u_print(uint64_t reg_val, int64_t mem_fd);
ret_t			at_i_print(uint64_t reg_val, int64_t mem_fd);
ret_t			at_h_print(uint64_t reg_val, int64_t mem_fd);
ret_t			at_p_print(uint64_t reg_val, int64_t mem_fd);
ret_t			at_o_print(uint64_t reg_val, int64_t mem_fd);
ret_t			at_s_print(uint64_t reg_val, int64_t mem_fd);
ret_t			at_b_print(uint64_t reg_val, int64_t mem_fd);
ret_t			at_l_print(uint64_t reg_val, int64_t mem_fd);
ret_t			at_c_print(uint64_t reg_val, int64_t mem_fd);

#endif // ARGS_H
