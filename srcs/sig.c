/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 11:28:00 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/13 07:41:50 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/sig.h"

static const uint64_t		CORE_SIGS[] = {
	SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGIOT, SIGQUIT,
	SIGSEGV, SIGSYS, SIGTRAP, SIGXCPU, SIGXFSZ,
};

static const char			*SIG_STR[] = {
	"SIGABRT", "SIGALRM", "SIGBUS", "SIGCHLD", "SIGCLD",
	"SIGCONT", "SIGFPE", "SIGHUP", "SIGILL", "SIGINT",
	"SIGIO", "SIGIOT", "SIGKILL", "SIGPIPE", "SIGPOLL",
	"SIGPROF", "SIGPWR", "SIGQUIT", "SIGSEGV", "SIGSTKFLT",
	"SIGSTOP", "SIGTSTP", "SIGSYS", "SIGTERM", "SIGTRAP",
	"SIGTTIN", "SIGTTOU", "SIGURG", "SIGUSR1", "SIGUSR2",
	"SIGVTALRM", "SIGXCPU", "SIGXFSZ", "SIGWINCH",
};

static const uint64_t		SIGS[] = {
	SIGABRT, SIGALRM, SIGBUS, SIGCHLD, SIGCLD,
	SIGCONT, SIGFPE, SIGHUP, SIGILL, SIGINT,
	SIGIO, SIGIOT, SIGKILL, SIGPIPE, SIGPOLL,
	SIGPROF, SIGPWR, SIGQUIT, SIGSEGV, SIGSTKFLT,
	SIGSTOP, SIGTSTP, SIGSYS, SIGTERM, SIGTRAP,
	SIGTTIN, SIGTTOU, SIGURG, SIGUSR1, SIGUSR2,
	SIGVTALRM, SIGXCPU, SIGXFSZ, SIGWINCH,
};

int			sig_is_core(uint64_t sig) {
	uint8_t		i;

	for (i = 0; i < CORE_SIG_NB; ++i)
		if (sig == CORE_SIGS[i])
			return (1);
	return (0);
}

const char		*sig_name(uint64_t sig) {
	uint8_t		i;

	for (i = 0; i < SIG_NB; ++i)
		if (sig == SIGS[i])
			return (SIG_STR[i]);
	return (NULL);
}
