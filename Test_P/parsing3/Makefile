# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   Makefile                                           :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2021/08/15 11:07:34 by psaumet           #+#    #+#             #
#   Updated: 2021/08/15 11:07:42 by psaumet          ###   ########.fr       #
#                                                                            #
# ************************************************************************** #


# SIMPLE =======================================================================
NAME			= webserv

# SRCS =========================================================================
SRCS	= ./

SOURCES			+=	main.cpp		\
					Parsing.cpp		\
					Parsing_get_server.cpp	\
					Parsing_get_location.cpp	\
					Parsing_utils.cpp			\
					HttpServer.cpp				\
					HttpServerRequest.cpp		\
					HttpServerUtils.cpp			\
					Error.cpp					\
					to_delete.cpp
					
# INCLUDES =====================================================================
INCLUDE 		+= 	Headers/Parsing.hpp			\
					Headers/HttpServer.hpp		\
					Headers/Error.hpp

# FLAGS ========================================================================
FLAGS 			= -Wall -Wextra -Werror -g -std=c++98 

# OBJECTS ======================================================================
OBJECTS_FOLDER 	= ./objects/


OBJECT			= $(SOURCES:.cpp=.o)
OBJECTS		 	= $(addprefix $(OBJECTS_FOLDER), $(OBJECT));


.PHONY: all
all: $(NAME)

# TESTER webserv=================================================================
$(OBJECTS_FOLDER)%.o :	$(SRCS)%.cpp	$(INCLUDE) 
	@mkdir -p	$(OBJECTS_FOLDER)
	@echo "Compiling: $<"
	clang++ $(FLAGS) -c $< -o $@

$(NAME): $(OBJECTS)
	@echo "\n"
	clang++ $(FLAGS) -o  $(NAME) $(OBJECTS)

.PHONY: clean
clean:
	@echo "Cleaning: objects"
	@rm -rf ./objects

.PHONY: fclean
fclean:	clean
	@echo "Cleaning: webserv"
	@rm -rf ./webserv

.PHONY: re
re: fclean all
