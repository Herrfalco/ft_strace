/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 16:18:13 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/24 16:18:27 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void interrupt() {
	write(2, "xyzzy\n", 6);
}

int main(void) {
	char buf[1024];

	signal(SIGINT, interrupt);
	read(0, buf, 1024);
	write(2, "qwerty\n", 7);
	return 0;
}
