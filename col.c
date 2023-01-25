/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   col.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:42:02 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/25 14:47:48 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "col.h"

static col_stack_t		g_col_stack = {
	.dat = { WHITE },
	.sz = 1,
};

void	set_col(const col_t color) {
	strncpy(g_col_stack.dat[g_col_stack.sz++], color, COLOR_SZ);
	printf("%s", color);
}

void	unset_col(void) {
	printf("%s", g_col_stack.dat[--g_col_stack.sz - 1]);
}
