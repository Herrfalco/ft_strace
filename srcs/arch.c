/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arch.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:43:27 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/13 12:14:21 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/arch.h"

static arch_t			g_arch;

static const uint8_t	g_regs_bind[ARCH_NB][REG_NB] = {
	{ 10, 15, 14, 13, 12, 7, 9, 8 },
	{ 0, 8, 0, 1, 2, 3, 4, 5 },
	{ 6, 11, 0, 1, 2, 3, 4, 5 },
};

ret_t						arch_set(const char *path) {
	int				fd;
	Elf32_Ehdr		e_hdr;

	if ((fd = open(path, O_RDONLY)) < 0)
		return (ret_new(RT_OPEN, path));
	if (read(fd, &e_hdr, sizeof(Elf32_Ehdr))
			!= sizeof(Elf32_Ehdr)) {
		close(fd);
		return (ret_new(RT_READ, path));
	}
	switch (e_hdr.e_machine) {
		case EM_386:
			g_arch = ARCH_AMD_32;
			break;
		case EM_X86_64:
			g_arch = ARCH_AMD_64;
			break;
		case EM_AARCH64:
			g_arch = ARCH_ARM_64;
			break;
		default:
			g_arch = ARCH_UNK;
			close(fd);
			return(ret_new(RT_ARCH_UNK, NULL));
	}
	close(fd);
	return (ret_new(RT_OK, NULL));
}

arch_t				arch_get(void) {
	return (g_arch);
};

int					arch_64(void) {
	return (!(g_arch == ARCH_AMD_32));
}

uint64_t			arch_get_reg(void *regs, uint8_t idx) {
	return (arch_64()
		? ((uint64_t *)regs)[g_regs_bind[g_arch][idx]]
		: ((uint32_t *)regs)[g_regs_bind[g_arch][idx]]);
}
