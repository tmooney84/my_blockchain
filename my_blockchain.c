#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stddef.h>
#include <string.h>

#include "dsa.h"

// char **parse_string(const char *string, int string_length, int *num_tokens);
// int my_strcmp(const char *s1, const char *s2);
// void free_string_array(char **names, int num_names);
// void *my_memset(void *str, int c, size_t n);
// void error_message1();
// void error_message2();
// void error_message3();
// void error_message4();
// void error_message5();
// void error_message6();
// void ok_computer();

int main()
{
    int *current_node_array_size = malloc(sizeof(int));
    *current_node_array_size = (int)INITIAL_NODE_ARRAY_SIZE;

    int fd;
    struct stat file_stats;
    int fileExists = 0;

    if (stat("backup.txt", &file_stats) == 0)
    {
        fileExists = 1;
    }

    // need to explicitly manage the file overwrite when storing blockchain to file
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
     //*************************TESTING******************** */ 
        // printf("Num nodes: are %d\n", num_nodes);
       
       
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
            return -1;
        }
        input_string[n - 1] = '\0';

        int *num_tokens = malloc(sizeof(num_tokens));
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

        // // TEST: print out array of strings
        // for (int i = 0; i < *num_tokens; i++)
        // {
        //     printf("token[%d]: ", i);

        //     for (int j = 0; j < INPUT_STRING_LENGTH && tokens_list[i][j] != '\0'; j++)
        //     {
        //         printf("%c", tokens_list[i][j]);
        //     }

        //     printf("\n");
        // }

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
            printf("Backing up blockchain...");
        }

    cleanup:
        free_string_array(tokens_list, *num_tokens);
        free(num_tokens);
        free(input_string);
    }

   // if (quit_flag == 1)
    //{
     //   if(save_blockchain_data(fd, node_array, current_node_array_size) < 0)
        // {
        //     printf("Unable to save blockchain data.");
        //     return -1;
        // }
    //    else
    //        printf("Backing up blockchain...");
   // }

    // free node array and everything involved with it
    close(fd); 
    free_node_array(node_array, current_node_array_size);
    free(current_node_array_size);
    return 0;
}

// void error_message1()
// {
//     printf("1: no more resources available on the computer\n");
// }

// void error_message2()
// {
//     printf("2: this node already exists\n");
// }

// void error_message3()
// {
//     printf("3: this block already exists\n");
// }

// void error_message4()
// {
//     printf("4: node doesn't exist\n");
// }

// void error_message5()
// {
//     printf("5: block doesn't exist\n");
// }

// void error_message6()
// {
//     printf("6: command not found\n");
// }

// void ok_computer()
// {
//     printf("OK\n");
// }

// // frees pointers related to string array
// void free_string_array(char **names, int num_names)
// {
//     if (names == NULL)
//     {
//         return;
//     }

//     for (int i = 0; i < num_names; i++)
//     {
//         free(names[i]); // Free each dynamically allocated string
//     }

//     free(names);
// }

// int my_strcmp(const char *s1, const char *s2)
// {
//     for (int i = 0; s1[i] != '\0' || s2[i] != '\0'; i++)
//     {
//         if (s1[i] < s2[i])
//         {
//             return -1;
//         }
//         else if (s1[i] > s2[i])
//         {
//             return 1;
//         }
//     }

//     return 0;
// }

// void *my_memset(void *str, int c, size_t n)
// {
//     unsigned char *ptr = (unsigned char *)str;

//     for (size_t i = 0; i < n; i++)
//     {
//         ptr[i] = (unsigned char)c;
//     }

//     return str;
// }

// char **parse_string(const char *string, int string_length, int *num_tokens)
// {
//     *num_tokens = 0;
//     int in_word = 0;

//     // how many substrings does the string contain
//     for (int i = 0; i < string_length; i++)
//     {
//         if (string[i] != ' ')
//         {
//             if (!in_word)
//             {
//                 in_word = 1;
//                 (*num_tokens)++;
//             }
//         }
//         else
//         {
//             in_word = 0;
//         }
//     }

//     // create array of empty strings
//     char **tokens_list = malloc(*num_tokens * sizeof(char *));
//     if (!tokens_list)
//     {
//         printf("Unable to read input\n");
//         return NULL;
//     }
//     my_memset(tokens_list, '\0', *num_tokens);

//     for (int i = 0; i < *num_tokens; i++)
//     {
//         tokens_list[i] = malloc((string_length + 1) * (sizeof(char)));
//         if (!tokens_list)
//         {
//             printf("Unable to allocate memory.");
//             for (int j = 0; j < i; j++)
//             {
//                 free(tokens_list[j]);
//             }
//             free(tokens_list);
//             return NULL;
//         }
//         my_memset(tokens_list[i], '\0', (string_length + 1));
//     }

//     // tokenize string into sub-strings in token_list
//     int ss_idx = 0; // substring index
//     in_word = 0;    // flag to indicate that currently iterating through word
//     for (int i = 0, j = 0; i < string_length && j < *num_tokens; i++)
//     {
//         if (string[i] != ' ')
//         {
//             if (in_word == 0)
//             {
//                 in_word = 1;
//             }

//             tokens_list[j][ss_idx] = string[i];
//             ss_idx++;
//         }
//         else
//         {
//             if (in_word == 1)
//             {
//                 j++;
//             }
//             in_word = 0;
//             ss_idx = 0;
//         }
//     }

//     return tokens_list;
// }
