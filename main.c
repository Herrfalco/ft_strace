/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:12:31 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/29 12:33:53 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define		_GNU_SOURCE

#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/uio.h>
#include <sys/user.h>
#include <elf.h>

#include "syscall.h"

static int		error(int ret) {
	perror("Error");
	return (ret);
}

static void		wait_stop(pid_t pid, int *status) {
	sigset_t		set;

	sigemptyset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);
	waitpid(pid, status, 0);
	sigaddset(&set, SIGHUP);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGPIPE);
	sigaddset(&set, SIGTERM);
	sigprocmask(SIG_BLOCK, &set, NULL);
}

int			main(int argc, char **argv, char **env) {
	int							pid, status;
	sys_state_t					sys_state = S_CALL;
	uint8_t						regs[REGS_BUFF_SZ];
	const sysc_t				*sc;
	struct iovec				iov = { 
		.iov_base = regs, .iov_len = REGS_BUFF_SZ
	};
(void)argc; arch_set(ARCH_ARM_64);
	if ((pid = fork()) < 0)
		return (error(1));
	if (!pid) {
		if (raise(SIGSTOP))
			exit(error(2));
		execvpe(argv[1], argv + 1, env);
	}
	if (ptrace(PTRACE_SEIZE, pid, 0, 0) < 0)
		return (error(3));
	do {
		wait_stop(pid, &status);
		if (!WIFSTOPPED(status) || WSTOPSIG(status) != SIGTRAP) {
			if (WIFEXITED(status)) {
				sysc_ret_print(sc, regs);
				printf("+++ exited with %d +++\n",
						WEXITSTATUS(status));
				return (0);
			}
			ptrace(PTRACE_SYSCALL, pid, 0, 0);
			continue;
		}
		switch (sys_state) {
			case S_CALL:
				ptrace(PTRACE_GETREGSET, pid,
						NT_PRSTATUS, &iov);
				sysc_name_print(
						(sc = sysc_get(regs)));
				if (sc && sc->pstate == S_RET)
					break;
				if (sysc_args_print(sc, regs, pid))
					return (error(4));
				break;
			default:
				if (sc && sc->pstate == S_RET)
					if (sysc_args_print(sc, regs, pid))
						return (error(4));
				ptrace(PTRACE_GETREGSET, pid,
						NT_PRSTATUS, &iov);
				sysc_ret_print(sc, regs);
		}
		sys_state ^= S_RET;
		ptrace(PTRACE_SYSCALL, pid, 0, 0);
	} while (status);
	sysc_ret_print(sc, iov.iov_base);
	return (0);
}
