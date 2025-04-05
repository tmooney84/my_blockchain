// My Blockchain
// Submit directory	.
// Submit files	Makefile - *.c - *.h
// Description
// Command Name
// my_blockchain

// Blockchain: the Beginning
// my_blockchain -- create a blockchain

// Synopsis
// my_blockchain

// Description
// Blockchain is a command that allows for the creation and management of a blockchain. When the program starts (it loads a backup if there is one) then a prompt appears. This prompt allows to execute commands. When the commands are successful they display "ok" and if not, "nok: info" or info is an error message - see below:

// add node nid add a nid identifier to the blockchain node.
// rm node nid... remove nodes from the blockchain with a nid identifier. If nid is '*', then all nodes are impacted.
// add block bid nid... add a bid identifier block to nodes identified by nid. If nid is '*', then all nodes are impacted.
// rm block bid nid... remove the bid identified blocks from nodes identified by nid..
// ls list all nodes by their identifiers. The option -l attaches the blocks bid's associated with each node.
// sync synchronize all of the nodes with each other. Upon issuing this command, all of the nodes are composed of the same blocks.
// quit save and leave the blockchain.
// The blockchain prompt must display (see example below):

// a [ character
// a first letter that indicates the state of synchronization of the chain:
// "s" if the blockchain is synchronized
// "-" if the blockchain is not synchronized.
// n number of nodes in the chain.
// the "]> " string (with a space)
// Error messages
// 1: no more resources available on the computer
// 2: this node already exists
// 3: this block already exists
// 4: node doesn't exist
// 5: block doesn't exist
// 6: command not found
// Technical Information
// $>my_blockhain
// No Backup Found: Starting New Blockchain
// [s0]> add node 12
// OK
// [s1]> add block 21 *
// OK
// [s1]> add node 13
// OK
// [-2]> sync
// OK
// [s2]> ls -l
// 12: 21
// 13: 21
// [s2]> quit
// Backing up blockchain...
// $>my_blockhain
// Restoring From Backup
// [s2]> ls -l
// 12: 21
// 13: 21
// [s2]>
// you must create a Makefile, and the output is the command itself
// NID is an integer, BID is a string
// You can use:
// • malloc(3)

// • free(3)

// • printf(3)

// • write(2)

// • open(2)

// • read(2)

// • close(2)

// Multiline macros are forbidden

// Include another .c is forbidden

// Macros with logic (while/if/variables/...) are forbidden
//-----------------------------------------------------

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stddef.h>
#include <string.h>



#define HASH_SIZE 256
#define INPUT_STRING_LENGTH 40 // Maximum number of characters in the string (excluding the null terminator)
#define INITIAL_NODE_ARRAY_SIZE 256

char **parse_string(const char *string, int string_length, int *num_tokens);
int my_strcmp(const char *s1, const char *s2);
void free_string_array(char **names, int num_names);
void *my_memset(void *str, int c, size_t n);
void error_message1();
void error_message2();
void error_message3();
void error_message4();
void error_message5();
void error_message6();



// may be better to build an API with hashmap and set functionality

// typedef struct Hashmap
// {
// }

// add_node()
// remove_node()
// add_block()
// remove_block()
// list_all_blocks()
// sync_blockchain()
// quit_blockchain()





// int check_sync_status(Hashmap *map);







// add node nid

// rm node nid

// add block bid nid

// rm block bid nid

// ls list all nodes (ls -l)

// sync
// quit

int main()
{
    int fd;
    struct stat file_stats;
    int fileExists = 0;

    if (stat("backup.txt", &file_stats) == 0)
    {
        fileExists = 1;
    }

    //need to explicitly manage the file overwrite when storing blockchain to file
    fd = open("backup.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        printf("error opening file\n");
        return -1;
    }

    if (fileExists == 1)
    {
        // deserialize file and bring up structs
    }

    // else just chill and save to stuff later

    // int sync_status = check_sync_status(map);
    int sync_status = 1; // by default or if
    // checking through the file can confirm synced
    // else will be 0
    int num_nodes = 0;

    

    int quit_flag = 0;

    // could use enums here!!! to simpify parsing
    while (quit_flag == 0)
    {
    char *input_string = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    if (!input_string)
    {
        printf("Unable to allocate memory.\n");
        return -1;
    }
    my_memset(input_string, '\0', INPUT_STRING_LENGTH + 1);
    
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
        if(!num_tokens)
        {
            printf("Unable to allocate memory.\n");
            return -1;
        }

        char **tokens_list = parse_string(input_string, INPUT_STRING_LENGTH, num_tokens);
        if (!tokens_list)
        {
            printf("Unable to parse strings into inputs list\n");
            return -1;
        }

        // TEST: print out array of strings
        for (int i = 0; i < *num_tokens; i++)
        {
            printf("token[%d]: ", i);

            for (int j = 0; j < INPUT_STRING_LENGTH && tokens_list[i][j] != '\0'; j++)
            {
                printf("%c", tokens_list[i][j]);
            }

            printf("\n");
        }


        /*use the if then logic to differentiate what commands to use
        - are the numbers only for tokens_list[2] && tokens_list[3]


        */

        const char *quit_string = "quit";
        if (my_strcmp(quit_string, tokens_list[0]) == 0)
            {
                quit_flag = 1;
            }

        // free(current_node_array_size)???

        free_string_array(tokens_list, *num_tokens);
        free(num_tokens);
        free(input_string);
    }

    if (quit_flag == 1)
    {
        // backup info and write to file

        printf("Backing up blockchain...");
    }

    // free node array and everything involved with it
    return 0;
}

void error_message1()
{
    printf("1: no more resources available on the computer\n");
}

void error_message2()
{
    printf("2: this node already exists\n");
}

void error_message3()
{
    printf("3: this block already exists\n");
}

void error_message4()
{
    printf("4: node doesn't eixst");
}

void error_message5()
{
    printf("5: block doesn't exist");
}

void error_message6()
{
    printf("6: command not found\n");
}

// frees pointers related to string array
void free_string_array(char **names, int num_names)
{
    if (names == NULL)
    {
        return;
    }

    for (int i = 0; i < num_names; i++)
    {
        free(names[i]); // Free each dynamically allocated string
    }

    free(names);
}

int my_strcmp(const char *s1, const char *s2)
{
    for (int i = 0; s1[i] != '\0' || s2[i] != '\0'; i++)
    {
        if (s1[i] < s2[i])
        {
            return -1;
        }
        else if (s1[i] > s2[i])
        {
            return 1;
        }
    }

    return 0;
}

void *my_memset(void *str, int c, size_t n)
{
    unsigned char *ptr = (unsigned char *)str;
    
    for (size_t i = 0; i < n; i++)
    {
        ptr[i] = (unsigned char)c;
    }

    return str;
}

char **parse_string(const char *string, int string_length, int *num_tokens)
{
    *num_tokens = 0;
    int in_word = 0;

    // how many substrings does the string contain
    for (int i = 0; i < string_length; i++)
    {
        if (string[i] != ' ')
        {
            if (!in_word)
            {
                in_word = 1;
                (*num_tokens)++;
            }
        }
        else
        {
            in_word = 0;
        }
    }

    // create array of empty strings
    char **tokens_list = malloc(*num_tokens * sizeof(char *));
    if (!tokens_list)
    {
        printf("Unable to read input\n");
        return NULL;
    }
    my_memset(tokens_list, '\0', *num_tokens);

    for (int i = 0; i < *num_tokens; i++)
    {
        tokens_list[i] = malloc((string_length + 1) * (sizeof(char)));
        if(!tokens_list)
        {
            printf("Unable to allocate memory.");
            for(int j = 0; j < i; j++)
            {
                free(tokens_list[j]);
            }
            free(tokens_list);
            return NULL;
        }
        my_memset(tokens_list[i], '\0', (string_length + 1));
    }

    // tokenize string into sub-strings in token_list
    int ss_idx = 0; // substring index
    in_word = 0;    // flag to indicate that currently iterating through word
    for (int i = 0, j = 0; i < string_length && j < *num_tokens; i++)
    {
        if (string[i] != ' ')
        {
            if (in_word == 0)
            {
                in_word = 1;
            }

            tokens_list[j][ss_idx] = string[i];
            ss_idx++;
        }
        else
        {
            if (in_word == 1)
            {
                j++;
            }
            in_word = 0;
            ss_idx = 0;
        }
    }

    return tokens_list;
}

/*
1)check if backing store exists, if so open it
otherwise open() and make sure read and write
but also with complete write over (trunc?)

2) if backing store exists parse the serialized
input into the associated structs and build the
"objects/structs" from this info

3)display prompt of [s0]>
from there they can call one of the functions
with typing in the string "add node 12". Remember
* means to apply to all, so delimit with space.
could use regex? or keep it simple, when space break add '\n' and use switch fn. I then
parse this string and if formatted correctly,
splice it so that I can get the one - three arguments,
if they exist to plug into the associated function.
If there is an with input make sure to return

3) process based off each of the particular functions

4) once completed gather the nodes and save them to
file by "Backing up the blockchain..."

*/
