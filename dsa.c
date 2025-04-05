
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include "my_blockchain.c" // only temporarily while building

#define INITIAL_NODE_ARRAY_SIZE 256
#define ARRAY_EXPANSION_AMOUNT 256
//***** NEED TO SET int* current_node_array_size = INITIAL_NODE_ARRAY_SIZE in runner file to be passed in */
// if(!current_node_array_size)

typedef struct Block
{
    size_t block_id;
    Block *next;
} Block;

typedef struct Node
{
    size_t node_id;
    Block *block_list;
} Node;

// array of pointers to nodes
Node **build_node_array(int *current_node_array_size)
{
    Node **node_array = malloc(INITIAL_NODE_ARRAY_SIZE * sizeof(Node *));
    if (!node_array)
    {
        printf("Unable to allocate memory for node array");
        return -1;
    }
    my_memset(node_array, '\0', *current_node_array_size);
}

// add node nid >>> go through the array check if it is in the array, if less than array size add to first null spot, if full
// resize array

Node *create_node()
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node)
    {
        printf("Unable to allocate memory.\n");
        return -1;
    }

    new_node->node_id = 0;
    new_node->block_list = NULL;

    return new_node;
}

Block *create_block()
{
    Block *new_block = malloc(sizeof(Block));
    if (new_block)
    {
        printf("Unable to allocate memory.\n");
        return -1;
    }

    new_block->block_id = 0;
    new_block->next = NULL;
}

int add_node(ssize_t nid, Node **node_array, int *current_node_array_size)
{
    int node_exists_flag = 0;
    ssize_t open_element_num = -1;

    for (int i = 0; i < *current_node_array_size; i++)
    {
        if (node_array[i]->node_id == nid)
        {
            node_exists_flag = 1;
        }
        else if (node_array[i]->node_id == NULL)
        {
            open_element_num = i;
        }
    }

    if (node_exists_flag)
    {
        error_message2();
        return -1;
    }

    Node *new_node = create_node();
    if (!new_node)
    {
        printf("Unable to allocate memory.");
        return -1;
    }

    new_node->node_id = nid;

    if (open_element_num >= 0)
    {
        node_array[open_element_num] = new_node;
    }
    else if (open_element_num == -1)
    {
        if (!expand_node_array(node_array, *current_node_array_size))
        {
            printf("Unable to expand node_array");
            return -1;
        }

        node_array[*current_node_array_size - ARRAY_EXPANSION_AMOUNT] = new_node;
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

int remove_node(ssize_t nid, Node **node_array, int *current_node_array_size)
{
    int node_found = 0;
    // remove all nodes and associated blocks
    if (nid == '*')
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
            free(node_array[i]);
        }
    }
    // make sure the id is a number string??? ...else if

    // remove node and associated block
    else
    {
        for (int i = 0; i < *current_node_array_size; i++)
        {
            if (node_array[i]->node_id == nid)
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
                free(node_array[i]);
            }
        }
    }

    return 0;
    // also need to use error_massage4() if the node doesn't exist
}

int add_block(ssize_t nid, ssize_t bid, Node **node_array, int *current_node_array_size)
{
    Block *new_block = create_block();
    if (!new_block)
    {
        return -1;
    }
    // if nid *
    if (nid == '*')
    {
        // if blocklist contains blocks
        for (int i = 0; i < *current_node_array_size; i++)
        {
            if (node_array[i] && node_array[i]->block_list)
            {
                Block *iterator = node_array[i]->block_list;

                while (iterator->next = NULL)
                {
                    iterator = iterator->next;
                }

                iterator->next = new_block;
            }

            // if blocklist empty
            else if (node_array[i] && node_array[i]->block_list == NULL)
            {
                node_array[i]->block_list = new_block;
            }
        }
    }

    // if nid
    int nid_found_flag = 0;

    for (int i = 0; i < *current_node_array_size; i++)
    {
        // if block contains block list
        if (node_array[i]->node_id == nid && node_array[i]->block_list)
        {
            nid_found_flag = 1;
            Block *iterator = node_array[i]->block_list;

            while (iterator->next = NULL)
            {
                iterator = iterator->next;
            }

            iterator->next = new_block;
            break;
        }

        // if blocklist empty
        else if (node_array[i]->node_id == nid && node_array[i]->block_list == NULL)
        {

            nid_found_flag = 1;
            node_array[i]->block_list = new_block;
            break;
        }
    }
    if (nid_found_flag == 0)
    {
        error_message4();
        return -1;
    }

    return 0;
}

int remove_block(ssize_t nid, ssize_t bid, Node **node_array, int *current_node_array_size)
{
    // if nid
    for (int i = 0; i < *current_node_array_size; i++)
    {
        // if block contains block list
        if (node_array[i]->node_id == nid && node_array[i]->block_list)
        {
            Block *current_block = node_array[i]->block_list;

            // if first block of the block list
            if (current_block->block_id == bid)
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
            
            //if middle block 
                if(current_block->block_id == bid && current_block->next != NULL)
                {
                    prev->next = current_block->next;
                    free(current_block);
                    return 1;
                }
            // if end block of block list
                else if(current_block->block_id == bid && current_block->next == NULL)
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
    
    for(int i = 0; i < len; i++)
    {
       for(int j = 0; j < len - i -1; j++) 
        {
            if(node_array[j]->node_id > node_array[j+1]->node_id)
            {
                //swap elements
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
    if(!sort_node_array(node_array, *current_node_array_size))
    {
        printf("Unable to sort node_array");
        return -1;
    } 

    for(int i = 0; i < current_node_array_size; i++)
    {
        printf("%d\n", node_array[i]->node_id);
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
    if(!sort_node_array(node_array, *current_node_array_size))
    {
        printf("Unable to sort node_array");
        return -1;
    }
    for(int i = 0; i < current_node_array_size; i++)
    {
        printf("%d: ", node_array[i]->node_id);
        while(node_array[i]->block_list->next == NULL)
        {
            printf("%d ", node_array[i]->block_list->block_id);
        }
        printf("\n");
    }
    return 0;
}

int sync_blockchain(Node **node_array, int *current_node_array_size)
{

}

int serialize_blockchain(Node **node_array, int *current_node_array_size)
{

}

int deserialize_blockchain(Node **node_array, int *current_node_array_size)
{

}
