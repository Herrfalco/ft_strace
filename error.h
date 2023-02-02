/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:14:03 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/01 23:36:11 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
#define ERROR_H

#include "incs.h"

#define ERROR_NB		127
#define FST_ERR_PTR		0xfffffffffffffdfc

#define ERESTARTSYS				512
#define ERESTARTNOINTR			513
#define ERESTARTNOHAND			514
#define ERESTART_RESTARTBLOCK	516

void		error_print(int64_t ret);

#endif // ERROR_H
