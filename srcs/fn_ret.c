/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_ret.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 08:18:36 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/13 11:38:44 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/fn_ret.h"

static const char		*g_ret_str[] = {
	"Success",
	"Can't open \"%s\"",
	"Can't read from \"%s\"",
	"Can't read from file descriptor %ld",
	"Can't seek into file descriptor %ld",
	"Unsupported file/arch",
	"Can't fork",
	"Can't raise signal",
	"Can't execute \"%s\"",
	"Can't trace process %ld",
	"Can't wait process %ld",
	"Can't get registers value from process %ld",
	"Can't set signal mask for process %ld",
	"Can't kill process %ld",
	"Can't trace syscall from process %ld",
	"Can't access memory from process %ld",
};

ret_t			ret_new(ret_typ_t typ, const void *dat) {
	ret_t		ret = { .typ = typ, .dat = dat };

	return (ret);
}

int				ret_is_err(ret_t ret) {
	return (ret.typ);
}

ret_typ_t		ret_err(ret_t ret) {
	fprintf(stderr, "Error: ");
	fprintf(stderr, g_ret_str[ret.typ], ret.dat);
	fprintf(stderr, "\n");
	return (ret.typ);
}
