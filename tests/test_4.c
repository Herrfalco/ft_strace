/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 16:19:03 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/24 16:20:12 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void null_handler(int sig) {
	(void)sig;
	return;
}

int main(void) {
	sigset_t set;

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigaddset(&set, 33);
	sigaddset(&set, 35);
	sigaddset(&set, 63);
	sigaddset(&set, 64);
	sigprocmask(SIG_BLOCK, &set, NULL);
	signal(SIGWINCH, null_handler);
	raise(SIGWINCH);
	return 0;
}
