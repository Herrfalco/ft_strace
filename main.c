/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:12:31 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/10 10:23:39 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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

int			handle_stop(main_dat_t *dat, char **argv) {
	uint8_t						regs[REGS_BUFF_SZ];
	struct iovec				iov = { 
		.iov_base = regs, .iov_len = REGS_BUFF_SZ
	};

	if ((dat->sig = WSTOPSIG(dat->status)) == SIGTRAP) {
		ptrace(PTRACE_GETREGSET, dat->pid,
				NT_PRSTATUS, &iov);
		if (dat->state == S_CALL) {
			dat->sc = sysc_get(regs);
			if (dat->start || !dat->sc
					|| !strcmp(dat->sc->name, "execve")) {
				sysc_name_print(dat->sc);
				if (!sysc_restart(dat->sc, dat->old_sc)
					&& (!dat->sc || dat->sc->pstate == S_CALL))
					if (sysc_args_print(dat->sc, regs, dat->pid))
						return (-1);
				if (!dat->start) {
					dat->start = 1;
					if (arch_set(argv[1])
							|| arch_get() == ARCH_UNK)
						return (-1);
				}
			}
		} else {
			dat->old_sc = dat->sc;
			if (dat->start) {
				if (dat->sc && dat->sc->pstate == S_RET)
					if (sysc_args_print(dat->sc, regs, dat->pid))
						return (-1);
				sysc_ret_print(dat->sc, regs);
			}
			if (dat->old_arch != arch_get()) {
				dat->old_arch = arch_get();
				printf("[ Process PID=%d runs in %s bit mode. ]\n",
					dat->pid, arch_64() ? "64" : "32");
			}
		}
		dat->state ^= S_RET;
		dat->sig = 0;
	} else if (dat->start)
		printf("--- %s ---\n", sig_name(dat->sig));
	return (0);
}

int			handle_kill(main_dat_t *dat) {
	dat->sig = WTERMSIG(dat->status);
	if (dat->sc && dat->sc->pstate == S_RET) {
		if (sysc_args_print(dat->sc, NULL, dat->pid))
			return (-1);
		sysc_ret_print(dat->sc, NULL);
	}
	printf("+++ killed by %s %s+++\n", sig_name(dat->sig),
			sig_is_core(WTERMSIG(dat->status))
			? "(core dumped) " : "");
	kill(getpid(), dat->sig);
	return (0);
}

void		handle_exit(main_dat_t *dat) {
	sysc_ret_print(dat->sc, NULL);
	printf("+++ exited with %d +++\n",
			WEXITSTATUS(dat->status));
}

int			init_trace(main_dat_t *dat, char **argv, char **env) {
	if (arch_set(argv[0])
			|| (dat->old_arch = arch_get()) == ARCH_UNK)
		return (-1);
	if ((dat->pid = fork()) < 0)
		return (-1);
	if (!dat->pid) {
		if (raise(SIGSTOP))
			return (-1);
		execvpe(argv[1], argv + 1, env);
		return (-1);
	}
	if (ptrace(PTRACE_SEIZE, dat->pid, 0, 0) < 0)
		return (-1);
	return (0);
}

int			main(int argc, char **argv, char **env) {
	main_dat_t					dat = { 
		.start = 0,
		.state = S_CALL,
		.old_sc = NULL,
	};

	(void)argc;
	if (init_trace(&dat, argv, env))
		return (1);
	while (42) {
		dat.sig = 0;
		wait_stop(dat.pid, &dat.status);
		if (WIFSTOPPED(dat.status)) {
			if (handle_stop(&dat, argv))
				return (2);
		} else if (dat.start && WIFSIGNALED(dat.status)) {
			if (handle_kill(&dat))
				return (3);
		} else {
			handle_exit(&dat);
			return (4);
		}
		ptrace(PTRACE_SYSCALL, dat.pid, 0, dat.sig);
	}
	return (0);
}
