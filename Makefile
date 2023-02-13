NAME	=	ft_strace
SRCS	=	srcs/arch.c \
		srcs/args.c \
		srcs/col.c \
		srcs/data.c \
		srcs/main.c \
		srcs/syscall.c \
		srcs/error.c \
		srcs/sig.c
OBJS	=	$(SRCS:.c=.o)
CC	=	gcc
CFLAGS	=	-Wall -Wall -Wextra

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o	:	%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) $(OBJS)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean all
