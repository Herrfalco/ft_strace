/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:39:38 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/25 18:00:06 by fcadet           ###   ########.fr       */
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
	at___print,
	at_m_print,
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
	WHITE,
	PURPLE,
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

static int		print_str(void *reg, int fd, uint8_t dump) {
	uint64_t		i, len;
	const char		esc_c[] = "abtnvfr";
	char			str[BUFF_SZ + 2];

	if (lseek(fd, *((uint64_t *)reg), SEEK_SET) < 0)
		return (-1);
	for (i = 0; i < BUFF_SZ + 2; ++i) {
		if (read(fd, str + i, 1) != 1)
			return (-1);
		if (!(dump || str[i]))
			break;
	}
	len = i;
	printf("\"");
	for (i = 0; i < BUFF_SZ
			&& (str[i] || dump); ++i) {
		if (str[i] > 6 && str[i] < 14) {
			printf("\\%c", esc_c[str[i] - 7]);
		} else if (str[i] >= ' ' && str[i] <= '~')
			printf("%c", str[i]);
		else if (dump) {
			printf("\\%x", str[i]);
			/*
			printf("\\");
			oct_rec(str[i]);
			*/
		} else
			break;
	}
	printf("\"%s", len > BUFF_SZ ? "..." : "");
	return (0);
}

int				at_s_print(void *reg, int fd) {
	return (print_str(reg, fd, 0));
}

int				at_m_print(void *reg, int fd) {
	return (print_str(reg, fd, 1));
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
