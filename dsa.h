#ifndef DSA_H
#define DSA_H

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h> 

#define INITIAL_NODE_ARRAY_SIZE 16
#define ARRAY_EXPANSION_AMOUNT 16

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
int calc_num_nodes(Node **node_array, int *current_node_array_size);
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
int *build_bid_numbers(int bid_array_size, Node **node_array, int *current_node_array_size);
int rebuild_node_array(Node **node_array, int *current_node_array_size, int bid_numbers[], int bid_array_size);
ssize_t parse_ssize_t(char *string, int str_len);
int check_sync_status(Node **node_array, int *current_node_array_size);




#endif