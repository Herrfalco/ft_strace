/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:12:31 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/13 17:40:26 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/main.h"

static ret_t	trace_init(main_dat_t *dat, char **argv, char **env) {
	ret_t			ret;

	if (ret_is_err((ret = arch_set(argv[0]))))
		return (ret);
	dat->old_arch = arch_get();
	if ((dat->pid = fork()) < 0)
		return (ret_new(RT_FORK, NULL));
	if (!dat->pid) {
		if (raise(SIGSTOP))
			return (ret_new(RT_RAISE, NULL));
		execvpe(argv[1], argv + 1, env);
		exit(RT_EXEC);
	}
	if (ptrace(PTRACE_SEIZE, dat->pid, 0, 0) < 0)
		return (ret_new(RT_TRACE, (void *)(uint64_t)dat->pid));
	return (ret_new(RT_OK, NULL));
}

static ret_t	trace_wait(pid_t pid, int *status) {
	sigset_t		set;

	sigemptyset(&set);
	if (sigprocmask(SIG_SETMASK, &set, NULL))
		return (ret_new(RT_MASK, (void *)(uint64_t)pid));
	if (waitpid(pid, status, 0) < 0)
		return (ret_new(RT_WAIT, (void *)(uint64_t)pid));
	sigaddset(&set, SIGHUP);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGPIPE);
	sigaddset(&set, SIGTERM);
	if (sigprocmask(SIG_BLOCK, &set, NULL))
		return (ret_new(RT_MASK, (void *)(uint64_t)pid));
	return (ret_new(RT_OK, NULL));
}

static ret_t		trace_call(main_dat_t *dat, char **argv, uint8_t *regs) {
	ret_t			ret;

	dat->sc = sysc_get(regs);
	if (dat->start || !dat->sc
			|| !strcmp(dat->sc->name, "execve")) {
		sysc_name_print(dat->sc);
		if (!sysc_restart(dat->sc, dat->old_sc)
				&& (!dat->sc || dat->sc->pstate == S_CALL))
			if (ret_is_err((ret
				= sysc_args_print(dat->sc, regs, dat->pid))))
				return (ret);
		if (!dat->start) {
			dat->start = 1;
			if (ret_is_err((ret = arch_set(argv[1])))
					&& ret.typ == RT_ARCH_UNK)
				return (ret);
		}
	}
	return (ret_new(RT_OK, NULL));
}

static ret_t	trace_ret(main_dat_t *dat, uint8_t *regs) {
	ret_t			ret;

	dat->old_sc = dat->sc;
	if (dat->start) {
		if (dat->sc && dat->sc->pstate == S_RET)
			if (ret_is_err((ret
				= sysc_args_print(dat->sc, regs, dat->pid))))
				return (ret);
		sysc_ret_print(dat->sc, regs);
	}
	if (dat->old_arch != arch_get()) {
		dat->old_arch = arch_get();
		printf("[ Process PID=%d runs in %s bit mode. ]\n",
				dat->pid, arch_64() ? "64" : "32");
	}
	return (ret_new(RT_OK, NULL));
}

static ret_t	trace_stop(main_dat_t *dat, char **argv) {
	ret_t						ret;
	uint8_t						regs[REGS_BUFF_SZ];
	struct iovec				iov = { 
		.iov_base = regs, .iov_len = REGS_BUFF_SZ
	};

	if ((dat->sig = WSTOPSIG(dat->status)) == SIGTRAP) {
		if (dat->state == S_NONE)
			dat->state = S_CALL;
		if (ptrace(PTRACE_GETREGSET, dat->pid,
				NT_PRSTATUS, &iov))
			return (ret_new(RT_REGS,
				(void *)(uint64_t)dat->pid));
		if (dat->state == S_CALL) {
			if (ret_is_err((ret = trace_call(dat, argv, regs))))
				return (ret);
		} else if (ret_is_err((ret = trace_ret(dat, regs))))
			return (ret);
		++dat->state;
		dat->sig = 0;
	} else if (dat->start)
		printf("--- %s ---\n", sig_name(dat->sig));
	return (ret_new(RT_OK, NULL));
}

static ret_t	trace_kill(main_dat_t *dat) {
	int64_t			pid;
	ret_t			ret;

	dat->sig = WTERMSIG(dat->status);
	if (dat->sc && dat->sc->pstate == S_RET)
		if (ret_is_err((ret
				= sysc_args_print(dat->sc, NULL, dat->pid))))
			return (ret);
	if (dat->state != S_NONE)
		sysc_ret_print(dat->sc, NULL);
	printf("+++ killed by %s %s+++\n", sig_name(dat->sig),
			sig_is_core(WTERMSIG(dat->status))
			? "(core dumped) " : "");
	if (kill((pid = getpid()), dat->sig))
		return (ret_new(RT_KILL, (void *)pid));
	return (ret_new(RT_OK, NULL));
}

static int		trace_exit(main_dat_t *dat) {
	int			ret;

	if (dat->state != S_NONE)
		sysc_ret_print(dat->sc, NULL);
	printf("+++ exited with %d +++\n",
			(ret = WEXITSTATUS(dat->status)));
	return (ret);
}

int			main(int argc, char **argv, char **env) {
	ret_t						ret;
	main_dat_t					dat = { 
		.start = 0,
		.state = S_NONE,
		.old_sc = NULL,
	};

	(void)argc;
	if (ret_is_err((ret = trace_init(&dat, argv, env))))
		return (ret_err(ret));
	while (42) {
		dat.sig = 0;
		if (ret_is_err((ret
			= trace_wait(dat.pid, &dat.status))))
			return (ret_err(ret));
		if (WIFSTOPPED(dat.status)) {
			if (ret_is_err((ret = trace_stop(&dat, argv))))
				return (ret_err(ret));
		} else if (dat.start && WIFSIGNALED(dat.status)) {
			if (ret_is_err((ret = trace_kill(&dat))))
				return (ret_err(ret));
		} else
			return (trace_exit(&dat));
		if (ptrace(PTRACE_SYSCALL, dat.pid, 0, dat.sig))
			return (ret_err(ret_new(RT_SYSC,
				(void *)(uint64_t)dat.pid)));
	}
	return (0);
}
