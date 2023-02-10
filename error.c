/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 15:11:24 by fcadet            #+#    #+#             */
/*   Updated: 2023/02/10 10:42:14 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

static const uint64_t	ERRORS[] = {
	E2BIG, EACCES, EADDRINUSE, EADDRNOTAVAIL, EAFNOSUPPORT, 
	EAGAIN, EALREADY, EBADE, EBADF, EBADFD, 
	EBADMSG, EBADR, EBADRQC, EBADSLT, EBUSY, 
	ECANCELED, ECHILD, ECHRNG, ECOMM, ECONNABORTED, 
	ECONNREFUSED, ECONNRESET, EDEADLK, EDESTADDRREQ, EDOM, 
	EDQUOT, EEXIST, EFAULT, EFBIG, EHOSTDOWN, 
	EHOSTUNREACH, EHWPOISON, EIDRM, EILSEQ, EINPROGRESS, 
	EINTR, EINVAL, EIO, EISCONN, EISDIR, 
	EISNAM, EKEYEXPIRED, EKEYREJECTED, EKEYREVOKED, EL2HLT, 
	EL2NSYNC, EL3HLT, EL3RST, ELIBACC, ELIBBAD, 
	ELIBMAX, ELIBSCN, ELIBEXEC, ELOOP, EMEDIUMTYPE, 
	EMFILE, EMLINK, EMSGSIZE, EMULTIHOP, ENAMETOOLONG, 
	ENETDOWN, ENETRESET, ENETUNREACH, ENFILE, ENOANO, 
	ENOBUFS, ENODATA, ENODEV, ENOENT, ENOEXEC, 
	ENOKEY, ENOLCK, ENOLINK, ENOMEDIUM, ENOMEM, 
	ENOMSG, ENONET, ENOPKG, ENOPROTOOPT, ENOSPC, 
	ENOSR, ENOSTR, ENOSYS, ENOTBLK, ENOTCONN, 
	ENOTDIR, ENOTEMPTY, ENOTRECOVERABLE, ENOTSOCK, ENOTSUP, 
	ENOTTY, ENOTUNIQ, ENXIO, EOVERFLOW, EOWNERDEAD, 
	EPERM, EPFNOSUPPORT, EPIPE, EPROTO, EPROTONOSUPPORT, 
	EPROTOTYPE, ERANGE, EREMCHG, EREMOTE, EREMOTEIO, 
	ERESTART, ERFKILL, EROFS, ESHUTDOWN, ESPIPE, 
	ESOCKTNOSUPPORT, ESRCH, ESTALE, ESTRPIPE, ETIME, 
	ETIMEDOUT, ETOOMANYREFS, ETXTBSY, EUCLEAN, EUNATCH, 
	EUSERS, EXDEV, EXFULL, ERESTARTSYS, ERESTARTNOINTR,
	ERESTARTNOHAND, ERESTART_RESTARTBLOCK,
};

static const char	*ERRORS_STR[] = {
	"E2BIG", "EACCES", "EADDRINUSE", "EADDRNOTAVAIL", "EAFNOSUPPORT", 
	"EAGAIN", "EALREADY", "EBADE", "EBADF", "EBADFD", 
	"EBADMSG", "EBADR", "EBADRQC", "EBADSLT", "EBUSY", 
	"ECANCELED", "ECHILD", "ECHRNG", "ECOMM", "ECONNABORTED", 
	"ECONNREFUSED", "ECONNRESET", "EDEADLK", "EDESTADDRREQ", "EDOM", 
	"EDQUOT", "EEXIST", "EFAULT", "EFBIG", "EHOSTDOWN", 
	"EHOSTUNREACH", "EHWPOISON", "EIDRM", "EILSEQ", "EINPROGRESS", 
	"EINTR", "EINVAL", "EIO", "EISCONN", "EISDIR", 
	"EISNAM", "EKEYEXPIRED", "EKEYREJECTED", "EKEYREVOKED", "EL2HLT", 
	"EL2NSYNC", "EL3HLT", "EL3RST", "ELIBACC", "ELIBBAD", 
	"ELIBMAX", "ELIBSCN", "ELIBEXEC", "ELOOP", "EMEDIUMTYPE", 
	"EMFILE", "EMLINK", "EMSGSIZE", "EMULTIHOP", "ENAMETOOLONG", 
	"ENETDOWN", "ENETRESET", "ENETUNREACH", "ENFILE", "ENOANO", 
	"ENOBUFS", "ENODATA", "ENODEV", "ENOENT", "ENOEXEC", 
	"ENOKEY", "ENOLCK", "ENOLINK", "ENOMEDIUM", "ENOMEM", 
	"ENOMSG", "ENONET", "ENOPKG", "ENOPROTOOPT", "ENOSPC", 
	"ENOSR", "ENOSTR", "ENOSYS", "ENOTBLK", "ENOTCONN", 
	"ENOTDIR", "ENOTEMPTY", "ENOTRECOVERABLE", "ENOTSOCK", "ENOTSUP", 
	"ENOTTY", "ENOTUNIQ", "ENXIO", "EOVERFLOW", "EOWNERDEAD", 
	"EPERM", "EPFNOSUPPORT", "EPIPE", "EPROTO", "EPROTONOSUPPORT", 
	"EPROTOTYPE", "ERANGE", "EREMCHG", "EREMOTE", "EREMOTEIO", 
	"ERESTART", "ERFKILL", "EROFS", "ESHUTDOWN", "ESPIPE", 
	"ESOCKTNOSUPPORT", "ESRCH", "ESTALE", "ESTRPIPE", "ETIME", 
	"ETIMEDOUT", "ETOOMANYREFS", "ETXTBSY", "EUCLEAN", "EUNATCH", 
	"EUSERS", "EXDEV", "EXFULL", "ERESTARTSYS", "ERESTARTNOINTR",
	"ERESTARTNOHAND", "ERESTART_RESTARTBLOCK",
};

void		error_print(int64_t ret) {
	uint8_t			i, find = 0;
	uint64_t		err = arch_64()
		? ret * -1 : (uint32_t)(ret * -1);

	printf(" = -1 ");
	for (i = 0; i < ERROR_NB; ++i) {
		if (err == ERRORS[i]) {
			printf("%s", ERRORS_STR[i]);
			find = 1;
		}
	}
	if (!find)
		printf("%s", "UNKNOWN");
	printf(" (%s)\n", err == ERESTARTSYS
			|| err == ERESTARTNOINTR
			|| err == ERESTARTNOHAND
			|| err == ERESTART_RESTARTBLOCK
		? "Interrupted by signal" : strerror(err));
}
