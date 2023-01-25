/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:12:31 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/25 17:07:28 by fcadet           ###   ########.fr       */
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

typedef enum		sys_state_e {
	S_CALL,
	S_RET,
}					sys_state_t;

int			error(int ret) {
	perror("Error");
	return (ret);
}

int			main(int argc, char **argv, char **env) {
	int							pid, status;
	uint8_t						buff[1024];
	sys_state_t					sys_state = S_CALL;
	const sysc_t				*sysc = NULL;
	struct iovec				iov = {
		iov.iov_base = buff,
		iov.iov_len = 1024,
	};

	(void)argc;
	arch_set(ARCH_ARM_64);
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
		waitpid(pid, &status, 0);
		if (WIFSTOPPED(status)) {
			if (WSTOPSIG(status) == SIGTRAP) {
				ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &iov);
				if (sys_state == S_CALL) {
					sysc = sysc_get(iov.iov_base);
					if (sysc_print(sysc, iov.iov_base, pid) < 0)
						return (error(4));
				} else
					sysc_ret_print(sysc, iov.iov_base);
				sys_state ^= S_RET;
			}
		}
		ptrace(PTRACE_SYSCALL, pid, 0, 0);
	} while (status);
	return (0);
}
