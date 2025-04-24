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
#define INPUT_STRING_LENGTH 40 

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

int find_nl_index(const char *s);
char *combine_str_and_free_first(char *s1, char *s2, int len2);
void init_my_readline();
char *my_readline(int fd);
Node **build_node_array(int *current_node_array_size);
Node *create_node();
Block *create_block();
void bubble_sort_ints(int arr[], int n);
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
int *build_bid_numbers(int bid_array_size, int *num_bids, Node **node_array, int *current_node_array_size);
int rebuild_node_array(Node **node_array, int *current_node_array_size, int bid_numbers[], int bid_array_size);
ssize_t parse_ssize_t(char *string, int str_len);
int check_sync_status(Node **node_array, int *current_node_array_size);
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
void ok_computer();

#endif