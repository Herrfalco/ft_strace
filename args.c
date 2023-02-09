/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:39:38 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/09 08:52:54 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"

const at_print_t	AT_PRINT[ARG_TYP_NB] = {
	at___print,
	at_u_print,
	at_i_print,
	at_h_print,
	at_p_print,
	at_o_print,
	at_s_print,
	at_b_print,
	at_l_print,
	at_c_print,
};

const col_t			AT_COL[ARG_TYP_NB] = {
	WHITE,
	BLUE,
	BLUE,
	BLUE,
	RED,
	BLUE,
	PURPLE,
	PURPLE,
	CYAN,
	CYAN,
};

static read_lim_t	g_rlim = { 0 };

void			args_set_rlim(uint64_t val) {
	g_rlim.val = val;
	g_rlim.set = 1;
}

int				at___print(uint64_t reg_val, int mem_fd) {
	(void)reg_val;
	(void)mem_fd;
	return (0);
}

int				at_u_print(uint64_t reg_val, int mem_fd) {
	(void)mem_fd;
	if (arch_64())
		printf("%lu", reg_val);
	else
		printf("%u", (uint32_t)reg_val);
	return (0);
}

int				at_i_print(uint64_t reg_val, int mem_fd) {
	(void)mem_fd;
	printf("%d", (int32_t)reg_val);
	return (0);
}

int				at_h_print(uint64_t reg_val, int mem_fd) {
	(void)mem_fd;
	if (!reg_val)
		printf("0");
	else if (arch_64())
		printf("0x%lx", reg_val);
	else
		printf("0x%x", (int32_t)reg_val);
	return (0);
}

int				at_p_print(uint64_t reg_val, int mem_fd) {
	(void)mem_fd;
	if (!reg_val)
		printf("NULL");
	else if (arch_64())
		printf("0x%lx", reg_val);
	else
		printf("0x%x", (uint32_t)reg_val);
	return (0);
}

static void		oct_rec(uint64_t val) {
	if (val / 8)
		oct_rec(val / 8);
	printf("%ld", val % 8);
}

int				at_o_print(uint64_t reg_val, int mem_fd) {
	(void)mem_fd;
	printf("0");
	if (arch_64())
		oct_rec(reg_val);
	else
		oct_rec((uint32_t)reg_val);
	return (0);
}

static int		str_print(uint64_t reg_val, int mem_fd, uint8_t bin) {
	uint64_t			i, len;
	uint8_t				dump = 1, r_lim;
	const char			esc_c[] = "abtnvfr";
	unsigned char		str[BUFF_SZ + 2];

	r_lim = g_rlim.set;
	g_rlim.set = 0;
	if (lseek(mem_fd, reg_val, SEEK_SET) < 0)
		return (-1);
	for (i = 0; (!r_lim || i < g_rlim.val)
			&& i < BUFF_SZ + 2; ++i) {
		if (read(mem_fd, str + i, 1) != 1)
			return (-1);
		if (!i && !bin)
			dump = str[i] && !(str[i] > 6 && str[i] < 14)
				&& !(str[i] >= ' ' && str[i] <= '~');
		if (!(dump || str[i]))
			break;
	}
	len = i;
	printf("\"");
	for (i = 0; (!r_lim || i < g_rlim.val)
			&& i < BUFF_SZ && (str[i] || dump); ++i) {
		if (bin) {
			printf("\\x%02x", str[i]);
		} else if (str[i] > 6 && str[i] < 14) {
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

int				at_b_print(uint64_t reg_val, int mem_fd) {
	return (str_print(reg_val, mem_fd, 1));
}

int				at_s_print(uint64_t reg_val, int mem_fd) {
	return (str_print(reg_val, mem_fd, 0));
}

int				at_l_print(uint64_t reg_val, int mem_fd) {
	uint64_t		i;
	void			*offs[BUFF_SZ + 2] = { 0 };

	if (lseek(mem_fd, reg_val, SEEK_SET) < 0)
		return (-1);
	for (i = 0; i < BUFF_SZ + 2; ++i) {
		if (arch_64()) {
			if (read(mem_fd, offs + i, sizeof(uint64_t))
					!= sizeof(uint64_t))
				return (-1);
		} else {
			if (read(mem_fd, offs + i, sizeof(uint32_t))
					!= sizeof(uint32_t))
				return (-1);
		}
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

int				at_c_print(uint64_t reg_val, int mem_fd) {
	uint64_t		ptr, i;

	at_p_print(reg_val, mem_fd);
	if (lseek(mem_fd, reg_val, SEEK_SET) < 0)
		return (-1);
	for (i = 0; i < MAX_LC; ++i) {
		if (arch_64()) {
			if (read(mem_fd, &ptr, sizeof(uint64_t))
					!= sizeof(uint64_t))
				return (-1);
		} else {
			if (read(mem_fd, &ptr, sizeof(uint32_t))
					!= sizeof(uint32_t))
				return (-1);
		}
		if (!ptr)
			break;
	}
	printf(" /* %ld vars */", i);
	return (0);
}
