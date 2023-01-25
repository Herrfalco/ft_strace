/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:10:27 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/24 19:35:56 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
#define DATA_H

#define ARG_NB		6
#define SYSC_NB		413

#include "args.h"
#include "arch.h"

typedef struct		sysc_s {
	char			*name;
	int				id[ARCH_NB];		
	arg_typ_t		args[ARG_NB];
	arg_typ_t		ret;
}					sysc_t;

const sysc_t		SYSC[SYSC_NB];

#endif // DATA_H
