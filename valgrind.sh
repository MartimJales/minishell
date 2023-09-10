make re && clear && valgrind --leak-check=full --track-origins=yes --log-file="leaks.txt" --suppressions="minishell.sup" ./minishell
