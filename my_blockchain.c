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

typedef struct node
{

} Node;

typedef struct block
{

} Block;

// add_node()
// remove_node()
// add_block()
// remove_block()
// list_all_blocks()
// sync_blockchain()
// quit_blockchain()

char **parse_string(char *string, int string_length);
int my_strcmp(const char *s1, const char *s2);

// error_message1()
// error_message2()
// error_message3()
// error_message4()
// error_message5()
// error_message6()

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

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

    if (stat("./backup.txt", &file_stats) == 0)
    {
        fileExists = 1;
    }

    fd = open("./file.txt", O_RDWR | O_TRUNC, 0666);
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

    int sync_status = 1; // by default or if
    // checking through the file can confirm synced
    int num_nodes = 0;
    int input_string_length = 100;
    char *input_string = malloc((input_string_length + 1) * sizeof(char));
    if(!input_string)
    {
        printf("Error allocating memory\n");
        return -1;
    } 

    int quit_flag = 0;

    // could use enums here!!! to simpify parsing
    while (quit_flag == 0)
    {
        printf("[%c%d]> \n", sync_status, num_nodes);
        if (read(0, input_string, input_string_length) < 0)
        {
            printf("Unable to read input\n");
            return -1;
        }

        char **inputs_list = parse_string(input_string, input_string_length);
        if(!inputs_list)
        {
            printf("Unable to parse strings into inputs list\n");
            return -1;
        }
    }

    free(input_string);
    return 0;
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


    char **parse_string(char *string, int string_length)
    {
        int args_count = 0;
        int in_word = 0;

        for (int i = 0; i < string_length; i++)
        {
            if (string[i] != ' ')
            {
                if (!in_word)
                {
                    in_word = 1;
                    args_count++;
                }
            }
            else
            {
                in_word = 0;
            }
        }

        char ** inputs_list = malloc(args_count * sizeof(char *));
        if(!inputs_list)
        {
            printf("Unable to read input\n");
            return NULL;
        }
    

        return inputs_list;
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
    could use regex? or keep it simple. I then
    parse this string and if formatted correctly,
    splice it so that I can get the one - three arguments,
    if they exist to plug into the associated function.
    If there is an with input make sure to return

    3) process based off each of the particular functions

    4) once completed gather the nodes and save them to
    file by "Backing up the blockchain..."

    */
