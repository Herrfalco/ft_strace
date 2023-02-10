/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arch.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:43:49 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/10 12:38:37 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARCH_H
#define ARCH_H

#include "incs.h"

#define ARCH_NB		3

#define REG_RET		0
#define REG_SYSC	1
#define REG_ARGS	2
#define REG_NB		8

typedef enum		arch_e {
	ARCH_AMD_64,
	ARCH_ARM_64,
	ARCH_AMD_32,
	ARCH_UNK,
}					arch_t;

int					arch_set(const char *path);
arch_t				arch_get(void);
int					arch_64(void);
uint64_t			arch_get_reg(void *regs, uint8_t idx);

#endif // ARCH_H
