/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:39:38 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/30 08:29:54 by fcadet           ###   ########.fr       */
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
};

int				at___print(uint64_t regval, int mem_fd) {
	(void)regval;
	(void)mem_fd;
	return (0);
}

int				at_u_print(uint64_t regval, int mem_fd) {
	(void)mem_fd;
	printf("%lu", regval);
	return (0);
}

int				at_i_print(uint64_t regval, int mem_fd) {
	(void)mem_fd;
	printf("%ld", regval);
	return (0);
}

int				at_p_print(uint64_t regval, int mem_fd) {
	(void)mem_fd;
	if (!regval)
		printf("NULL");
	else
		printf("0x%lx", regval);
	return (0);
}

static void		oct_rec(uint64_t val) {
	if (val / 8)
		oct_rec(val / 8);
	printf("%ld", val % 8);
}

int				at_o_print(uint64_t regval, int mem_fd) {
	(void)mem_fd;
	printf("0");
	oct_rec(regval);
	return (0);
}

int				at_s_print(uint64_t regval, int mem_fd) {
	uint64_t		i, len;
	uint8_t			dump;
	const char		esc_c[] = "abtnvfr";
	char			str[BUFF_SZ + 2];

	if (lseek(mem_fd, regval, SEEK_SET) < 0)
		return (-1);
	for (i = 0; i < BUFF_SZ + 2; ++i) {
		if (read(mem_fd, str + i, 1) != 1)
			return (-1);
		if (!i)
			dump = !(str[i] > 6 && str[i] < 14)
				&& !(str[i] >= ' ' && str[i] <= '~');
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
			printf("\\");
			oct_rec(str[i]);
		} else
			break;
	}
	printf("\"%s", len > BUFF_SZ ? "..." : "");
	return (0);
}

int				at_l_print(uint64_t regval, int mem_fd) {
	uint64_t		i;
	void			*offs[BUFF_SZ + 2];

	if (lseek(mem_fd, regval, SEEK_SET) < 0)
		return (-1);
	for (i = 0; i < BUFF_SZ + 2; ++i) {
		if (read(mem_fd, offs + i, sizeof(uint64_t))
				!= sizeof(uint64_t))
			return (-1);
		if (!offs[i])
			break;
	}
	printf("[");
	for (i = 0; i < BUFF_SZ + 2 && offs[i]; ++i) { 
		if (at_s_print(((uint64_t *)offs)[i], mem_fd) < 0)
			return (-1);
		printf(", ");
	}
	printf("%s", i > BUFF_SZ ? "...]" : "\b\b]");
	return (0);
}

int				at_c_print(uint64_t regval, int mem_fd) {
	uint64_t		ptr, i;

	at_p_print(regval, mem_fd);
	if (lseek(mem_fd, regval, SEEK_SET) < 0)
		return (-1);
	for (i = 0; i < MAX_LC; ++i) {
		if (read(mem_fd, &ptr, sizeof(uint64_t))
				!= sizeof(uint64_t))
			return (-1);
		if (!ptr)
			break;
	}
	printf(" /* %ld vars */", i);
	return (0);
}
