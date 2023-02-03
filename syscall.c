/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 10:05:33 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/01 23:51:43 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syscall.h"

static int		get_proc_mem(int pid) {
	char			mem_path[BUFF_SZ];

	sprintf(mem_path, "/proc/%d/mem", pid);
	return (open(mem_path, O_RDONLY));
}

const sysc_t	*sysc_get(void *regs) {
	uint64_t		i;

	for (i = 0; i < SYSC_NB; ++i) {
		if ((int)arch_get_reg(regs, REG_SYSC)
				== SYSC[i].id[arch_get()])
			return (&SYSC[i]);
	}
	return (NULL);
}

void		sysc_name_print(const sysc_t *sc) {
	set_col(sc ? YELLOW : WHITE);
	printf("%s", sc ? sc->name : "syscall");
	unset_col();
}

int			sysc_restart(const sysc_t *sc, const sysc_t *o_sc) {
	if (!sc || !o_sc || *(sc->args) != AT_R)
		return (0);
	set_col(YELLOW);
	printf("(");
	set_col(WHITE);
	printf("<... resuming interrupted %s ...>", o_sc->name);
	unset_col();
	printf(")");
	unset_col();
	return (1);
}

int			sysc_args_print(const sysc_t *sc, void *regs, pid_t pid) {
	int			mem_fd;
	uint64_t	i, j;

	set_col(sc ? YELLOW : WHITE);
	printf("(");
	if (!(regs)) {
		set_col(WHITE);
		printf("<unfinished ...>");
		unset_col();
		printf(")");
		unset_col();
		return (0);
	}
	if (!sc) {
		printf("%ld)", arch_get_reg(regs, REG_SYSC));
		unset_col();
		return (0);
	}
	if ((mem_fd = get_proc_mem(pid)) < 0)
		return (-1);
	for (i = 0; i < ARG_NB && sc->args[i] != AT__; ++i) {
		for (j = 0; j < ARG_TYP_NB; ++j) {
			if (sc->args[i] == j) {
				set_col(AT_COL[j]);
				if (AT_PRINT[j](
					arch_get_reg(regs, REG_ARGS + i),
					mem_fd) < 0) {
					unset_col();
					unset_col();
					close(mem_fd);
					return (-1);
				}
				break;
			}
		}
		unset_col();
		printf(", ");
	}
	printf("%s)", i ? "\b\b" : "");
	unset_col();
	close(mem_fd);
	return (0);
}

void		sysc_ret_print(const sysc_t *sc, void *regs) {
	uint64_t		ret;

	set_col(GREEN);
	if (!regs) {
		printf(" = ?\n");
		unset_col();
		return;
	}
	ret = arch_get_reg(regs, REG_RET);
	if (sc) {
		switch (sc->ret) {
			case AT_X:
				printf(" = ?\n");
				break;
			case AT_U:
				printf(" = %lu\n", ret);
				break;
			case AT_P:
				if (ret >= FST_ERR_PTR)
					error_print(ret);
				else if (!ret)
					printf(" = NULL\n");
				else
					printf(" = 0x%lx\n", ret);
				break;
			default:
				if ((int64_t)ret < 0)
					error_print(ret);
				else
					printf(" = %ld\n", ret);
		}
	} else
		printf(" = %ld\n", ret);
	unset_col();
}
