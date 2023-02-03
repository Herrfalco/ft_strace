/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arch.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:43:27 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/24 17:45:23 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arch.h"

static arch_t			g_arch = 0;

static const uint8_t	g_regs_bind[ARCH_NB][REG_NB] = {
	{ 10, 15, 14, 13, 12, 7, 9, 8 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 8, 0, 1, 2, 3, 4, 5 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
};

void				arch_set(arch_t arch) {
	g_arch = arch;
}

arch_t				arch_get(void) {
	return (g_arch);
};

uint64_t			arch_get_reg(void *regs, uint8_t idx) {
	return (g_arch == ARCH_AMD_64 || g_arch == ARCH_ARM_64 
		? ((uint64_t *)regs)[g_regs_bind[g_arch][idx]]
		: ((uint32_t *)regs)[g_regs_bind[g_arch][idx]]);
}
