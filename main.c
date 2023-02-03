/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:12:31 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/01 23:51:46 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define		_GNU_SOURCE

#include "syscall.h"
#include "sig.h"

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
	uint64_t					sig;
	sys_state_t					sys_state = S_CALL;
	uint8_t						regs[REGS_BUFF_SZ], start = 0;
	const sysc_t				*sc, *old_sc = NULL;
	struct iovec				iov = { 
		.iov_base = regs, .iov_len = REGS_BUFF_SZ
	};

	(void)argc;
//	arch_set(ARCH_ARM_64);
	arch_set(ARCH_AMD_64);
	if ((pid = fork()) < 0)
		return (error(1));
	if (!pid) {
		if (raise(SIGSTOP))
			return (1);
		execvpe(argv[1], argv + 1, env);
		return (1);
	}
	if (ptrace(PTRACE_SEIZE, pid, 0, 0) < 0)
		return (error(3));
	do {
		sig = 0;
		wait_stop(pid, &status);
		if (WIFSTOPPED(status)) {
			if ((sig = WSTOPSIG(status)) == SIGTRAP) {
				if (sys_state == S_CALL) {
					ptrace(PTRACE_GETREGSET, pid,
						NT_PRSTATUS, &iov);
					sc = sysc_get(regs);
					if (!start && (!sc
						|| !strcmp(sc->name, "execve")))
						start = 1;
					if (start) {
						sysc_name_print(sc);
						if (!sysc_restart(sc, old_sc)
							&& (!sc || sc->pstate == S_CALL))
							if (sysc_args_print(sc, regs, pid))
								return (error(4));
					}
				} else if (start) {
					old_sc = sc;
					if (sc && sc->pstate == S_RET)
						if (sysc_args_print(sc, regs, pid))
							return (error(4));
					ptrace(PTRACE_GETREGSET, pid,
							NT_PRSTATUS, &iov);
					sysc_ret_print(sc, regs);
				}
				sys_state ^= S_RET;
				sig = 0;
			} else if (start)
				printf("--- %s ---\n", sig_name(sig));
		} else if (start && WIFSIGNALED(status)) {
			sig = WTERMSIG(status);
			if (sc && sc->pstate == S_RET) {
				sysc_args_print(sc, NULL, pid);
				sysc_ret_print(sc, NULL);
			}
			printf("+++ killed by %s %s+++\n",
				sig_name(sig), sig_is_core(WTERMSIG(status))
				? "(core dumped) " : "");
			kill(getpid(), sig);
		} else {
			sysc_ret_print(sc, regs);
			printf("+++ exited with %d +++\n",
					WEXITSTATUS(status));
			return (0);
		}
		ptrace(PTRACE_SYSCALL, pid, 0, sig);
	} while (status);
	sysc_ret_print(sc, regs);
	return (0);
}
