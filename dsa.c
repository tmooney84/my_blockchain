
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#define INITIAL_NODE_ARRAY_SIZE 256
//***** NEED TO SET int* current_node_array_size = INITIAL_NODE_ARRAY_SIZE in runner file to be passed in */
    //if(!current_node_array_size)

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

//array of pointers to nodes 
Node *build_node_array(int *current_node_array_size)
{
    Node *node_array = malloc(INITIAL_NODE_ARRAY_SIZE * sizeof(Node));
    if(!node_array)
    {
        printf("Unable to allocate memory for node array");
        return -1;
    }
}

int resize_node_array(Node * node_array, int *current_node_array_size)
{
    //take in array make new array and then copy info to new array, reassign array and delete old array


}
