/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 11:54:10 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/30 12:25:23 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SIG_H
#define SIG_H

#define SIG_NB			34
#define CORE_SIG_NB		11

#include "incs.h"

int				sig_is_core(uint64_t sig);
const char		*sig_name(uint64_t sig);

#endif // SIG_H
