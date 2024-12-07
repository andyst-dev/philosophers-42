NAME = philo

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRC_FILES = main.c init.c routine.c forks.c \
			state.c utils.c
SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -pthread #-fsanitize=thread -g

all:			$(NAME)

$(NAME): 		$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c | $(OBJ_DIR)
				$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
				mkdir -p $(OBJ_DIR)

clean:
				rm -rf $(OBJ_DIR)

fclean: clean
				rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re