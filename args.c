/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:39:38 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/25 16:18:05 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"

const at_print_t	AT_PRINT[ARG_TYP_NB] = {
	at___print,
	at_u_print,
	at_i_print,
	at_p_print,
	at_o_print,
	at_s_print,
	at_l_print,
	at_c_print,
};

const col_t			AT_COL[ARG_TYP_NB] = {
	WHITE,
	BLUE,
	BLUE,
	RED,
	BLUE,
	PURPLE,
	CYAN,
	CYAN,
};

int				at___print(void *reg, int fd) {
	(void)reg;
	(void)fd;
	return (0);
}

int				at_u_print(void *reg, int fd) {
	(void)fd;
	printf("%lu", *((uint64_t *)reg));
	return (0);
}

int				at_i_print(void *reg, int fd) {
	(void)fd;
	printf("%ld", *((uint64_t *)reg));
	return (0);
}

int				at_p_print(void *reg, int fd) {
	(void)fd;
	if (!*((uint64_t *)reg))
		printf("NULL");
	else
		printf("0x%lx", *((uint64_t *)reg));
	return (0);
}

static void		oct_rec(uint64_t val) {
	if (val / 8)
		oct_rec(val / 8);
	printf("%ld", val % 8);
}

int				at_o_print(void *reg, int fd) {
	(void)fd;
	printf("0");
	oct_rec(*((uint64_t *)reg));
	return (0);
}

static char		*rep_esc(char *str) {
	static char		new_str[BUFF_SZ * 2];
	const char		esc_c[] = "abtnvfr";
	uint64_t		i, j;

	bzero(new_str, BUFF_SZ * 2);
	for (i = 0, j = 0; i < BUFF_SZ && str[i]; ++i) {
		if (str[i] > 6 && str[i] < 14) {
			new_str[j++] = '\\';
			new_str[j++] = esc_c[str[i] - 7];
			continue;
		} else if (str[i] < ' ' || str[i] > '~')
			break;
		new_str[j++] = str[i];
	}
	return (new_str);
}

int				at_s_print(void *reg, int fd) {
	uint64_t		i;
	char			str[BUFF_SZ + 2];

	if (lseek(fd, *((uint64_t *)reg), SEEK_SET) < 0)
		return (-1);
	for (i = 0; i < BUFF_SZ + 2; ++i) {
		if (read(fd, str + i, 1) != 1)
			return (-1);
		if (!str[i])
			break;
	}
	printf("\"%.*s\"%s", BUFF_SZ, rep_esc(str),
			i > BUFF_SZ ? "..." : "");
	return (0);
}

int				at_l_print(void *reg, int fd) {
	uint64_t		i;
	void			*offs[BUFF_SZ + 2];

	if (lseek(fd, *((uint64_t *)reg), SEEK_SET) < 0)
		return (-1);
	for (i = 0; i < BUFF_SZ + 2; ++i) {
		if (read(fd, offs + i, sizeof(uint64_t))
				!= sizeof(uint64_t))
			return (-1);
		if (!offs[i])
			break;
	}
	printf("[");
	for (i = 0; i < BUFF_SZ + 2 && offs[i]; ++i) { 
		if (at_s_print(offs + i, fd) < 0)
			return (-1);
		printf(", ");
	}
	printf("%s", i > BUFF_SZ ? "...]" : "\b\b]");
	return (0);
}

int				at_c_print(void *reg, int fd) {
	uint64_t		ptr, i;

	at_p_print(reg, fd);
	if (lseek(fd, *((uint64_t *)reg), SEEK_SET) < 0)
		return (-1);
	for (i = 0; i < MAX_LC; ++i) {
		if (read(fd, &ptr, sizeof(uint64_t))
				!= sizeof(uint64_t))
			return (-1);
		if (!ptr)
			break;
	}
	printf(" /* %ld vars */", i);
	return (0);
}
