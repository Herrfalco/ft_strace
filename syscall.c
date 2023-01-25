/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 10:05:33 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/25 17:07:13 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syscall.h"

static int		args_print(const sysc_t *sysc, void *regs, int fd) {
	uint64_t	i, j;

	for (i = 0; i < ARG_NB; ++i) {
		if (sysc->args[i] == AT__)
			return (i);
		for (j = 0; j < ARG_TYP_NB; ++j) {
			if (sysc->args[i] == j) {
				set_col(AT_COL[j]);
				if (AT_PRINT[j]((uint64_t *)regs + i, fd) < 0) {
					unset_col();
					return (-1);
				}
				break;
			}
		}
		unset_col();
		printf(", ");
	}
	return (i);
}

static int		get_proc_mem(int pid) {
	char			mem_path[BUFF_SZ];

	sprintf(mem_path, "/proc/%d/mem", pid);
	return (open(mem_path, O_RDONLY));
}

const sysc_t		*sysc_get(void *regs) {
	const sysc_t	*sysc = NULL;
	uint64_t		i;

	for (i = 0; i < SYSC_NB; ++i) {
		if ((int)((uint64_t *)regs)[8] 
				== SYSC[i].id[arch_get()]) {
			sysc = &SYSC[i];
			return (sysc);
		}
	}
	return (NULL);
}

int			sysc_print(const sysc_t *sc, void *regs, int pid) {
	int				fd, ret;

	if (sc) {
		if ((fd = get_proc_mem(pid)) < 0)
			return (-1);
		set_col(YELLOW);
		printf("%s(", sc->name);
		if ((ret = args_print(sc, regs, fd)) < 0)
			return (-1);
		close(fd);
		printf("%s) ", ret ? "\b\b" : "");
		unset_col();
		if (sc->ret == AT_X)
			sysc_ret_print(sc, NULL);
	} else
		printf("syscall(%d) ", (int)((uint64_t *)regs)[8]);
	return (0);
}

void		sysc_ret_print(const sysc_t *sc, void *regs) {
	set_col(GREEN);
	if (sc) {
		switch (sc->ret) {
			case AT_X:
				printf("= ?\n");
				break;
			case AT_U:
				printf("= %lu\n", *((uint64_t *)regs));
				break;
			case AT_P:
				if (*((uint64_t *)regs))
					printf("= 0x%lx\n", *((uint64_t *)regs));
				else
					printf("= NULL\n");
				break;
			default:
				printf("= %ld\n", *((uint64_t *)regs));
		}
	} else
		printf("= %ld\n", *((uint64_t *)regs));
	unset_col();
}
