/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 10:05:26 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/20 19:33:16 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSCALL_H
#define SYSCALL_H

#define _GNU_SOURCE

#define ARCH_NB		4
#define ARG_NB		6
#define SYSC_NB		413
#define BUFF_SZ		32

#include <stdio.h>
#include <stdint.h>
#include <sys/uio.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define COLOR_SZ	8

#define	RED				"\e[1;31m"
#define	GREEN			"\e[0;32m"
#define	YELLOW			"\e[1;33m"
#define	BLUE			"\e[1;34m"
#define	PURPLE			"\e[0;35m"
#define	CYAN			"\e[0;36m"
#define	WHITE			"\e[1;37m"

typedef enum		col_op_e {
	CO_PUSH,
	CO_POP,
}					col_op_t;

typedef enum		arch_e {
	ARCH_AMD_64,
	ARCH_ARM_32,
	ARCH_ARM_64,
	ARCH_AMD_32,
}					arch_t;

typedef enum		arg_typ_e {
	AT_IGN,
	AT_UINT,
	AT_INT,
	AT_HEX,
	AT_OCT,
	AT_STR,
	AT_LST,
}					arg_typ_t;

typedef struct		sysc_s {
	char			*name;
	int				id[4];		
	arg_typ_t		args[6];
}					sysc_t;

void		arch_set(arch_t arch);
int			sysc_print(void *regs, int pid);
void		set_color(char *color, col_op_t op);

#endif

