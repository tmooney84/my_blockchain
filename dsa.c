
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#include <string.h> //TEMPORARY WHILE TESTING!!!
// #include "my_blockchain.c" // only temporarily while building

// #define INITIAL_NODE_ARRAY_SIZE 256
// #define ARRAY_EXPANSION_AMOUNT 256

#define INPUT_STRING_LENGTH 40 // ************************JUST FOR TESTING ************Maximum number of characters in the string (excluding the null terminator)

#define INITIAL_NODE_ARRAY_SIZE 16
#define ARRAY_EXPANSION_AMOUNT 16

//***** NEED TO SET int* current_node_array_size = INITIAL_NODE_ARRAY_SIZE in runner file to be passed in */
// if(!current_node_array_size)

typedef struct Block
{
    ssize_t block_id;
    struct Block *next;
} Block;

typedef struct Node
{
    ssize_t node_id;
    Block *block_list;
} Node;

Node **build_node_array(int *current_node_array_size);
Node *create_node();
Block *create_block();
int add_node(char *nid, Node **node_array, int *current_node_array_size);
int expand_node_array(Node **node_array, int *current_node_array_size);
int remove_node(char *nid, Node **node_array, int *current_node_array_size);
int add_block(char *nid, char *bid, Node **node_array, int *current_node_array_size);
int remove_block(char *nid, char *bid, Node **node_array, int *current_node_array_size);
int sort_node_array(Node **node_array, int *current_node_array_size);
int list_nodes(Node **node_array, int *current_node_array_size);
int list_nodes_blocks(Node **node_array, int *current_node_array_size);
int check_unique(ssize_t bid_num, int bid_numbers[], int array_size);
int sync_blockchain(Node **node_array, int *current_node_array_size);
int save_blockchain_data(int fd, Node **node_array, int *current_node_array_size);
Node **load_blockchain_data(int fd, int *current_node_array_size);
int free_node_array(Node **node_array, int *current_node_array_size);
int free_list(Block *head);
int build_bid_numbers(int bid_numbers[], int bid_array_size, Node **node_array, int *current_node_array_size);
int rebuild_node_array(Node **node_array, int *current_node_array_size, int bid_numbers[], int bid_array_size);
ssize_t parse_ssize_t(char *string, int str_len);

//--------------------------------------------------------------
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
    printf("4: node doesn't exist\n");
}

void error_message5()
{
    printf("5: block doesn't exist\n");
}

void error_message6()
{
    printf("6: command not found\n");
}

void ok_computer()
{
    printf("OK\n");
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
//---------------------------------------------------------------------

void *my_memset(void *str, int c, size_t n)
{
    unsigned char *ptr = (unsigned char *)str;

    for (size_t i = 0; i < n; i++)
    {
        ptr[i] = (unsigned char)c;
    }

    return str;
}

// array of pointers to nodes
Node **build_node_array(int *current_node_array_size)
{
    Node **node_array = malloc(INITIAL_NODE_ARRAY_SIZE * sizeof(Node *));
    if (!node_array)
    {
        printf("Unable to allocate memory for node array");
        return NULL;
    }
    my_memset(node_array, '\0', *current_node_array_size);

    for (int i = 0; i < *current_node_array_size; i++)
    {
        node_array[i] = create_node();
        if (node_array[i] == NULL)
        {
            printf("Unable to create node array");
            return NULL;
        }
    }

    return node_array;
}

// add node nid >>> go through the array check if it is in the array, if less than array size add to first null spot, if full
// resize array

Node *create_node()
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        printf("Unable to allocate memory.\n");
        return NULL;
    }

    new_node->node_id = 0;
    new_node->block_list = NULL;

    return new_node;
}

Block *create_block()
{
    Block *new_block = malloc(sizeof(Block));
    if (!new_block)
    {
        printf("Unable to allocate memory.\n");
        return NULL;
    }

    new_block->block_id = 0;
    new_block->next = NULL;

    return new_block;
}

int add_node(char *nid, Node **node_array, int *current_node_array_size)
{
    int str_len = (int)INPUT_STRING_LENGTH;
    ssize_t nid_num = parse_ssize_t(nid, str_len);

    int node_exists_flag = 0;
    ssize_t open_element_num = -1;

    // determine if node already exists
    for (int i = 0; i < *current_node_array_size; i++)
    {
        if (node_array[i]->node_id == nid_num)
        {
            node_exists_flag = 1;
        }
        else if (node_array[i]->node_id == 0)
        {
            open_element_num = i;
            break;
        }
    }

    if (node_exists_flag)
    {
        error_message2();
        return -1;
    }

    if (open_element_num >= 0)
    {
        node_array[open_element_num]->node_id = nid_num;
    }
    else if (open_element_num == -1)
    {
        if (!expand_node_array(node_array, current_node_array_size))
        {
            printf("Unable to expand node_array");
            return -1;
        }

        node_array[*current_node_array_size - ARRAY_EXPANSION_AMOUNT]->node_id = nid_num;
    }

    return 1;
}

int expand_node_array(Node **node_array, int *current_node_array_size)
{
    int old_size = *current_node_array_size;
    *current_node_array_size += 256;
    Node **temp_array = malloc((*current_node_array_size) * sizeof(Node *));
    if (!temp_array)
    {
        printf("Unable to allocate memory");
        for (int i = 0; i < old_size; i++)
        {

            ///
            free(node_array[i]->block_list);
            free(node_array[i]);
        }
        free(node_array);
        return -1;
    }

    for (int i = 0; i < old_size; i++)
    {
        temp_array[i]->node_id = node_array[i]->node_id;
    }

    free(node_array);
    node_array = temp_array;
    temp_array = NULL;

    return 1;
}

int remove_node(char *nid, Node **node_array, int *current_node_array_size)
{
    // remove all nodes and associated blocks
    if (nid[0] == '*' && nid[1] == '\0')
    {
        for (int i = 0; i < *current_node_array_size; i++)
        {
            if (node_array[i]->block_list)
            {
                Block *current_block = node_array[i]->block_list;
                //>>>need to check if it contains blocks and make sure to free them correctly
                while (current_block != NULL)
                {
                    Block *next_block = current_block->next;
                    free(current_block);
                    current_block = next_block;
                }
            }

            node_array[i]->node_id = 0;
            node_array[i]->block_list = NULL;
        }
    }
    // make sure the id is a number string??? ...else if

    // remove node and associated block
    else
    {
        int str_len = (int)INPUT_STRING_LENGTH; //*************TEMPORARY WHILE TESTING *************** */
        ssize_t nid_num = parse_ssize_t(nid, str_len);

        for (int i = 0; i < *current_node_array_size; i++)
        {
            if (node_array[i]->node_id == nid_num)
            {
                if (node_array[i]->block_list)
                {
                    Block *current_block = node_array[i]->block_list;
                    //>>>need to check if it contains blocks and make sure to free them correctly
                    while (current_block != NULL)
                    {
                        Block *next_block = current_block->next;
                        free(current_block);
                        current_block = next_block;
                    }
                }
                node_array[i]->node_id = 0;
                node_array[i]->block_list = NULL;
            }
        }
    }
    return 0;
}

int add_block(char *nid, char *bid, Node **node_array, int *current_node_array_size)
{
    if (!node_array)
    {
        printf("Node array not found.\n");
        return -1;
    }

    int str_len = (int)INPUT_STRING_LENGTH;
    ssize_t bid_num = parse_ssize_t(bid, str_len);

    // if nid *
    if (nid[0] == '*' && nid[1] == '\0')
    {
        // if blocklist contains blocks
        for (int i = 0; i < *current_node_array_size; i++)
        {
            Block *new_block = create_block();
            if (!new_block)
            {
                return -1;
            }
            new_block->block_id = bid_num;

            // if blocklist empty
            if (node_array[i]->block_list == NULL)
            {
                node_array[i]->block_list = new_block;
            }

            else
            {
                Block *iterator = node_array[i]->block_list;

                while (iterator->next != NULL)
                {
                    iterator = iterator->next;
                }

                iterator->next = new_block;
            }
        }
    }

    else
    {
        // if nid
        ssize_t nid_num = parse_ssize_t(nid, str_len);
        int nid_found_flag = 0;

        for (int i = 0; i < *current_node_array_size; i++)
        {
            // if block contains block list
            if (node_array[i]->node_id == nid_num)
            {
                nid_found_flag = 1;

                Block *new_block = create_block();
                if (!new_block)
                {
                    return -1;
                }
                new_block->block_id = bid_num;

                if (node_array[i]->block_list == NULL)
                {
                    node_array[i]->block_list = new_block;
                }

                else
                {
                    Block *iterator = node_array[i]->block_list;

                    while (iterator->next != NULL)
                    {
                        iterator = iterator->next;
                    }

                    iterator->next = new_block;
                }
                break;
            }
        }
        if (nid_found_flag == 0)
        {
            error_message4();
            return -1;
        }
    }
    return 0;
}

ssize_t parse_ssize_t(char *string, int str_len)
{
    ssize_t num = 0;

    for (int i = 0; i < str_len; i++)
    {
        if (string[i] == '\0')
        {
            break;
        }

        if ((string[i] < '0' || string[i] > '9'))
        {
            return -1;
        }

        num = num * 10 + (string[i] - '0');
    }

    return num;
}

int remove_block(char *nid, char *bid, Node **node_array, int *current_node_array_size)
{

    int str_len = (int)INPUT_STRING_LENGTH;
    ssize_t bid_num = parse_ssize_t(bid, str_len);
    ssize_t nid_num = parse_ssize_t(nid, str_len);

    // if nid
    for (int i = 0; i < *current_node_array_size; i++)
    {
        // if block contains block list
        if (node_array[i]->node_id == nid_num && node_array[i]->block_list)
        {
            Block *current_block = node_array[i]->block_list;

            // if first block of the block list
            if (current_block->block_id == bid_num)
            {
                node_array[i]->block_list = current_block->next;
                free(current_block);

                return 1;
            }

            // if middle block or end block of block list
            while (current_block->next != NULL)
            {
                Block *prev = current_block;
                current_block = current_block->next;

                // if middle block
                if (current_block->block_id == bid_num && current_block->next != NULL)
                {
                    prev->next = current_block->next;
                    free(current_block);
                    return 1;
                }
                // if end block of block list
                else if (current_block->block_id == bid_num && current_block->next == NULL)
                {
                    prev->next = NULL;
                    free(current_block);
                    return 1;
                }
            }
        }
    }
    // if block does not exist
    error_message5();
    return -1;
}

/*ls >>> copy array & sort by id >>> could put back in orginal array and then print out???
12
13
*/
int sort_node_array(Node **node_array, int *current_node_array_size)
{
    // use bubble sort to flip nodes
    int len = *current_node_array_size;

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len - i - 1; j++)
        {
            ssize_t id1 = node_array[j]->node_id;
            ssize_t id2 = node_array[j + 1]->node_id;
            if ((id1 == 0 && id2 != 0) || (id1 != 0 && id2 != 0 && id1 > id2))
            {
                // swap elements
                Node *temp = node_array[j];
                node_array[j] = node_array[j + 1];
                node_array[j + 1] = temp;
            }
        }
    }
    return 1;
}

int list_nodes(Node **node_array, int *current_node_array_size)
{
    if (!sort_node_array(node_array, current_node_array_size))
    {
        printf("Unable to sort node_array");
        return -1;
    }

    for (int i = 0; i < *current_node_array_size; i++)
    {
        if (node_array[i]->node_id == 0)
        {
            continue;
        }

        printf("%ld\n", node_array[i]->node_id);
    }
    return 0;
}

/*ls -l >>> copy array & sort by id, copy back >>> then go down the ll within each;
sort and return dynamic array
12: 21
13: 21
*/
int list_nodes_blocks(Node **node_array, int *current_node_array_size)
{
    if (!sort_node_array(node_array, current_node_array_size))
    {
        printf("Unable to sort node_array");
        return -1;
    }

    for (int i = 0; i < *current_node_array_size; i++)
    {
        if (node_array[i]->node_id == 0)
        {
            continue;
        }

        printf("%ld: ", node_array[i]->node_id);
        Block *current = node_array[i]->block_list;

        while (current != NULL)
        {
            printf("%ld ", current->block_id);
            current = current->next;
        }
        printf("\n");
    }
    return 0;
}

int check_unique(ssize_t bid_num, int bid_numbers[], int array_size)
{
    int unique_flag = 1;

    for (int i = 0; i < array_size; i++)
    {
        if (bid_num == bid_numbers[i])
        {
            unique_flag = 0;
            break;
        }
        else if (bid_numbers[i] == 0)
        {
            break;
        }
    }

    return unique_flag;
}

int sync_blockchain(Node **node_array, int *current_node_array_size)
{
    int bid_numbers[INITIAL_NODE_ARRAY_SIZE];
    int bid_array_size = INITIAL_NODE_ARRAY_SIZE;

    for (int i = 0; i < INITIAL_NODE_ARRAY_SIZE; i++)
    {
        bid_numbers[i] = 0;
    }

    build_bid_numbers(bid_numbers, bid_array_size, node_array, current_node_array_size);

    sort_node_array(node_array, current_node_array_size);

    rebuild_node_array(node_array, current_node_array_size, bid_numbers, bid_array_size);

    return 0;
}

int build_bid_numbers(int bid_numbers[], int bid_array_size, Node **node_array, int *current_node_array_size)
{
    int block_idx = 0;

    for (int i = 0; i < *current_node_array_size; i++)
    {
        if (node_array[i]->node_id != 0 && node_array[i]->block_list)
        {
            Block *current = node_array[i]->block_list;
            while (current != NULL)
            {
                if (check_unique(current->block_id, bid_numbers, bid_array_size))
                {
                    bid_numbers[block_idx] = current->block_id;
                    block_idx++;
                }

                current = current->next;
            }
        }
    }
    return 0;
}

int rebuild_node_array(Node **node_array, int *current_node_array_size, int bid_numbers[], int bid_array_size)
{
    // If no blocks exists to sync
    if (bid_numbers[0] == 0)
    {
        printf("No blocks exist to sync.\n");
        // return gracefully
        return 0;
    }

    // clear and rebuild node_array with synced blocks
    for (int i = 0; i < *current_node_array_size; i++)
    {
        // if node array doesn't exist
        if (node_array[i]->node_id == 0)
        {
            continue;
        }

        // if node array exists
        else if (node_array[i]->node_id != 0)
        {
            // clear list
            free_list(node_array[i]->block_list);
            node_array[i]->block_list = NULL;

            // add blocks to list
            // Block *current = node_array[i]->block_list;
            Block *current = NULL;

            for (int j = 0; bid_numbers[j] != 0 && j < bid_array_size; j++)
            {
                // set node info
                Block *new_block = create_block();
                if (!new_block)
                {
                    printf("Failure to allocate memory.\n");
                    return -1;
                }

                new_block->block_id = bid_numbers[j];

                if (node_array[i]->block_list == NULL)
                {
                    node_array[i]->block_list = new_block;
                    current = new_block;
                }
                else
                {
                    current->next = new_block;
                    current = new_block;
                }
            }
        }
    }

    return 0;
}

int save_blockchain_data(int fd, Node **node_array, int *current_node_array_size)
{
    if (!fd)
    {
        printf("Unable to write to file");
        return -1;
    }

    // write number of nodes
    write(fd, current_node_array_size, sizeof(int));

    for (int i = 0; i < *current_node_array_size; i++)
    {
        write(fd, &node_array[i]->node_id, sizeof(ssize_t));

        Block *block = node_array[i]->block_list;

        // write the blocks to file
        while (block)
        {
            int flag = 1;
            write(fd, &flag, sizeof(int));
            write(fd, &block->block_id, sizeof(ssize_t));
            block = block->next;
        }

        int end_flag = 0;
        write(fd, &end_flag, sizeof(int));
    }
    return 1;
}

Node **load_blockchain_data(int fd, int *current_node_array_size)
{
    lseek(fd, 0, SEEK_SET);
    // read number of nodes
    if (read(fd, current_node_array_size, sizeof(int)) != sizeof(int))
    {
        printf("Unable to read file.\n");
        return NULL;
    }

    // build node_array
    Node **node_array = build_node_array(current_node_array_size);

    for (int i = 0; i < *current_node_array_size; i++)
    {
        // read node_id
        if (read(fd, &node_array[i]->node_id, sizeof(ssize_t)) != sizeof(ssize_t))
        {
            perror("Failed to read node_id");
            free(node_array);
            return NULL;
        }

        Block *head = NULL;
        Block *tail = NULL;

        int flag;

        while (read(fd, &flag, sizeof(int)) == sizeof(int))
        {
            if (flag == 0)
                break; // end of block list

            // read block_id
            ssize_t block_id;

            if (read(fd, &block_id, sizeof(ssize_t)) != sizeof(ssize_t))
            {
                printf("Failure to read block_id");
                free(node_array);
                return NULL;
            }

            // create new block
            Block *new_block = create_block();
            if (!new_block)
            {
                printf("Unable to create new block.\n");
                return NULL;
            }

            new_block->block_id = block_id;

            if (!head)
            {
                head = tail = new_block;
            }
            else
            {
                tail->next = new_block;
                tail = new_block;
            }
        }
        //attach block_list to node
        node_array[i]->block_list = head;
    }
    return node_array;
}

// free node array
int free_node_array(Node **node_array, int *current_node_array_size)
{
    if (node_array == NULL)
    {
        return 0;
    }
    for (int i = 0; i < *current_node_array_size; i++)
    {
        // if node array doesn't exist
        if (node_array[i]->block_list == NULL)
        {
            continue;
        }

        // if node array exists
        else if (node_array[i]->block_list != NULL)
        {
            free_list(node_array[i]->block_list);
        }
    }
    free(node_array);
    return 0;
}

int free_list(Block *head)
{
    if (head == NULL)
    {
        return 1;
    }

    Block *current = head;
    while (current != NULL)
    {
        Block *next = current->next;
        free(current);
        current = next;
    }

    return 1;
}

int main()
{
    int *current_node_array_size = malloc(sizeof(int));
    if (!current_node_array_size)
    {
        printf("Error building node array.\n");
        return -1;
    }
    *current_node_array_size = INITIAL_NODE_ARRAY_SIZE;
    Node **node_array = build_node_array(current_node_array_size);
    if (!node_array)
    {
        printf("Error building node array.\n");
        return -1;
    }

    char *node1 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(node1, "14", INPUT_STRING_LENGTH + 1);
    char *node2 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(node2, "11", INPUT_STRING_LENGTH + 1);
    char *node3 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(node3, "12", INPUT_STRING_LENGTH + 1);
    char *node4 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(node4, "7", INPUT_STRING_LENGTH + 1);

    char *node_ast = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(node_ast, "*", INPUT_STRING_LENGTH + 1);

    char *block1 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(block1, "2", INPUT_STRING_LENGTH + 1);
    char *block2 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(block2, "16", INPUT_STRING_LENGTH + 1);
    char *block3 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(block3, "32", INPUT_STRING_LENGTH + 1);
    char *block4 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(block4, "64", INPUT_STRING_LENGTH + 1);
    char *block5 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(block5, "100", INPUT_STRING_LENGTH + 1);
    char *block6 = malloc((INPUT_STRING_LENGTH + 1) * sizeof(char));
    strncpy(block6, "1024", INPUT_STRING_LENGTH + 1);

    add_node(node1, node_array, current_node_array_size);
    add_node(node2, node_array, current_node_array_size);
    add_node(node3, node_array, current_node_array_size);
    add_node(node4, node_array, current_node_array_size);

    add_block(node1, block2, node_array, current_node_array_size);
    add_block(node1, block3, node_array, current_node_array_size);
    add_block(node1, block4, node_array, current_node_array_size);

    // need to change the input for this to be a string and for
    // each of the other input functions as well

    // add_block(*, 9999, node_array, current_node_array_size);

    list_nodes(node_array, current_node_array_size);

    printf("--------------------------------------\n");

    // remove_node(node_ast, node_array, current_node_array_size);

    add_block(node_ast, block5, node_array, current_node_array_size);

    list_nodes_blocks(node_array, current_node_array_size);

    printf("--------------------------------------\n");

    ///////when adding second array doesn't work, but does with one array
    add_block(node_ast, block6, node_array, current_node_array_size);

    list_nodes(node_array, current_node_array_size);
    printf("--------------------------------------\n");
    list_nodes_blocks(node_array, current_node_array_size);

    printf("--------------------------------------\n");

    sync_blockchain(node_array, current_node_array_size);

    list_nodes_blocks(node_array, current_node_array_size);

    int fd;
   // struct stat file_stats;
   // int fileExists = 0;

    // if (stat("backup.txt", &file_stats) == 0)
    // {
    //     fileExists = 1;
    // }

    //need to explicitly manage the file overwrite when storing blockchain to file
    fd = open("backup.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        printf("error opening file\n");
        return -1;
    }



    
    save_blockchain_data(fd, node_array, current_node_array_size);

    Node **new_chain = load_blockchain_data(fd, current_node_array_size);
    if(!new_chain)
    {
        printf("Unable to build blockchain");
        return 0;
    }

    printf("--------------------------------------\n");
    printf("Re-loaded Data: \n");
    list_nodes_blocks(new_chain, current_node_array_size);

    free(current_node_array_size);
    close(fd);
    return 0;
}