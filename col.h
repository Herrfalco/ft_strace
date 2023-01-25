/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   col.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:45:44 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/25 16:50:47 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COL_H
#define COL_H

#include "incs.h"

#define COLOR_SZ	8

#define	RED				"\e[1;31m"
#define	GREEN			"\e[1;32m"
#define	YELLOW			"\e[1;33m"
#define	BLUE			"\e[1;34m"
#define	PURPLE			"\e[1;35m"
#define	CYAN			"\e[1;36m"
#define	WHITE			"\e[1;37m"

typedef char		col_t[COLOR_SZ];

typedef struct		col_stack_s {
	col_t			dat[BUFF_SZ];
	uint64_t		sz;
}					col_stack_t;

void	set_col(const col_t color);
void	unset_col(void);

#endif // COL_H
