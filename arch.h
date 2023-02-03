/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arch.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:43:49 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/24 19:26:53 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARCH_H
#define ARCH_H

#include "incs.h"

#define ARCH_NB		4

#define REG_RET		0
#define REG_SYSC	1
#define REG_ARGS	2
#define REG_NB		8

typedef enum		arch_e {
	ARCH_AMD_64,
	ARCH_ARM_32,
	ARCH_ARM_64,
	ARCH_AMD_32,
}					arch_t;

void				arch_set(arch_t arch);
arch_t				arch_get(void);
uint64_t			arch_get_reg(void *regs, uint8_t idx);

#endif // ARCH_H
