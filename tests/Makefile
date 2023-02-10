NAMES		=	test_1 \
			test_2 \
			test_3 \
			test_4 \
			test_5
SRCS		=	$(NAMES:=.c)
OBJS		=	$(NAMES:=.o)
LIBS		=	-lpthread
CC		=	gcc
CFLAGS		=	-Wall -Wall -Wextra

ifndef ARCH
	ARCH	=	64
endif

all		:	$(NAMES)

$(NAMES) : %	:	%.o
			$(CC) $(CFLAGS) -m$(ARCH) $< -o $@ $(LIBS)

%.o		:	%.c
			$(CC) $(CFLAGS) -m$(ARCH) -c $< -o $@

clean		:
			$(RM) $(OBJS)

fclean		:	clean
			rm -rf $(NAMES)

re		:	fclean all
