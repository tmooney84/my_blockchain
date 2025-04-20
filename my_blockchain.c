#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stddef.h>
#include <string.h>

#include "dsa.h"

int main()
{
    int *current_node_array_size = malloc(sizeof(int));
    *current_node_array_size = (int)INITIAL_NODE_ARRAY_SIZE;

    int fd;
    struct stat file_stats;
    int fileExists = 0;

    if (stat("backup.txt", &file_stats) == 0 && file_stats.st_size > 0)
    {
        fileExists = 1;
    }
    else
    {
        printf("No Backup Found: Starting New Blockchain\n");
    }

    fd = open("backup.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        printf("error opening file\n");
        return -1;
    }

    Node **node_array = NULL;

    if (fileExists == 1)
    {
        node_array = load_blockchain_data(fd, current_node_array_size);
        if (!node_array)
        {
            printf("Unable to load blockchain data from file.\n");
            return -1;
        }
    }
    else
    {
        node_array = build_node_array(current_node_array_size);
        if (!node_array)
        {
            printf("Unable to allocate memory.\n");
            return -1;
        }
    }

    int sync_status = 1; // default sync status
    int num_nodes = 0;
    int quit_flag = 0;

    while (quit_flag == 0)
    {
        char *input_string = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
        if (!input_string)
        {
            printf("Unable to allocate memory.\n");
            return -1;
        }
        my_memset(input_string, '\0', INPUT_STRING_LENGTH + 1);

        num_nodes = calc_num_nodes(node_array, current_node_array_size);
       
       sync_status = check_sync_status(node_array, current_node_array_size);

        char sync_symbol;

        if (sync_status == 1)
        {
            sync_symbol = 's';
        }
        else
        {
            sync_symbol = '-';
        }

        printf("[%c%d]> ", sync_symbol, num_nodes);
        fflush(stdout);

        int n = read(STDIN_FILENO, input_string, INPUT_STRING_LENGTH);
        if (n < 0)
        {
            printf("Unable to read input\n");
            free(input_string);
            return -1;
        }
        if(n == 0){
            //End of input (EOF), exit the loop
            free(input_string);
            break;
        }
        input_string[n - 1] = '\0';

        int *num_tokens = malloc(sizeof(int));
        if (!num_tokens)
        {
            printf("Unable to allocate memory.\n");
            free(input_string);
            return -1;
        }

        char **tokens_list = parse_string(input_string, INPUT_STRING_LENGTH, num_tokens);
        if (!tokens_list)
        {
            printf("Unable to parse strings into inputs list\n");
            free(input_string);
            free(num_tokens);
            return -1;
        }

        // keywords for blockchain commands
        char add_c[10] = "add";
        char rm_c[10] = "rm";
        char node_c[10] = "node";
        char block_c[10] = "block";
        char ls_c[10] = "ls";
        char l_c[10] = "-l";
        char sync_c[10] = "sync";
        char quit_c[10] = "quit";

        // add node nid
        if (*num_tokens == 3 && my_strcmp(add_c, tokens_list[0]) == 0 && my_strcmp(node_c, tokens_list[1]) == 0)
        {
            if (add_node(tokens_list[2], node_array, current_node_array_size) < 0)
            {
                // shows error_message(2) or allocation error and gracefully go to next input
                goto cleanup;
            }
            else
                ok_computer();
        }

        // rm node nid         rm node nid *
        else if (*num_tokens == 3 && my_strcmp(rm_c, tokens_list[0]) == 0 && my_strcmp(node_c, tokens_list[1]) == 0)
        {
            if (remove_node(tokens_list[2], node_array, current_node_array_size) < 0)
            {
                goto cleanup;
            }
            else
                ok_computer();
        }

        //add block bid nid       add block bid *
        else if (*num_tokens == 4 && my_strcmp(add_c, tokens_list[0]) == 0 && my_strcmp(block_c, tokens_list[1]) == 0)
        {
            if(add_block(tokens_list[3], tokens_list[2], node_array, current_node_array_size) < 0)
            {
                goto cleanup;
            }
            else
                ok_computer();
        } 
        
        //ls          
        else if (*num_tokens == 1 && my_strcmp(ls_c, tokens_list[0]) == 0)
        {
            if(list_nodes(node_array, current_node_array_size) < 0)
            {
                goto cleanup;
            }
        }

        //ls -l
        else if (*num_tokens == 2 && my_strcmp(ls_c, tokens_list[0]) == 0 && my_strcmp(l_c, tokens_list[1]) == 0)
        {
            if(list_nodes_blocks(node_array, current_node_array_size) < 0)
            {
                goto cleanup;
            }
        }

        //sync
        else if (*num_tokens == 1 && my_strcmp(sync_c, tokens_list[0]) == 0)
        {
            if(sync_blockchain(node_array, current_node_array_size) < 0)
            {
                goto cleanup;
            }
            else
                ok_computer();
        }
        
        //quit 
        else if (my_strcmp(quit_c, tokens_list[0]) == 0)
        {
            quit_flag = 1;
            lseek(fd, 0, SEEK_SET);
            save_blockchain_data(fd, node_array, current_node_array_size);
            printf("Backing up blockchain...\n");
            goto cleanup;
        }

        else
        {
            error_message6();
        }

    cleanup:
        free_string_array(tokens_list, *num_tokens);
        free(num_tokens);
        free(input_string);
    }

    close(fd); 
    free_node_array(node_array, current_node_array_size);
    free(current_node_array_size);
    return 0;
}