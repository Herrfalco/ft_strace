NAMES		=	test_1 \
			test_2 \
			test_3 \
			test_4 \
			test_5
SRCS		=	$(NAMES:=.c)
OBJS		=	$(NAMES:=.o)
LIBS		=	-lpthread
CC		=	gcc
CFLAGS		=	#-m32

all		:	$(NAMES)

$(NAMES) : %	:	%.o
			$(CC) $(CFLAGS) $(ARCH) $< -o $@ $(LIBS)

%.o		:	%.c
			$(CC) $(CFLAGS) $(ARCH) -c $< -o $@

clean		:
			$(RM) $(OBJS)

fclean		:	clean
			rm -rf $(NAMES)

re		:	fclean all
