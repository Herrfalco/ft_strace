/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_ret.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 08:13:35 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/13 11:29:43 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FN_RET
#define FN_RET

#include "incs.h"

#define ERR_BUFF_SZ		1024

typedef enum		ret_typ_e {
	RT_OK,
	RT_OPEN,
	RT_READ,
	RT_READ_FD,
	RT_SEEK_FD,
	RT_ARCH_UNK,
	RT_FORK,
	RT_RAISE,
	RT_EXEC,
	RT_TRACE,
	RT_WAIT,
	RT_REGS,
	RT_MASK,
	RT_KILL,
	RT_SYSC,
	RT_MEM,
}					ret_typ_t;

typedef struct		ret_s {
	ret_typ_t		typ;
	const void		*dat;
}					ret_t;

ret_t				ret_new(ret_typ_t typ, const void *dat);
int					ret_is_err(ret_t ret);
ret_typ_t			ret_err(ret_t ret);

#endif // FN_RET
