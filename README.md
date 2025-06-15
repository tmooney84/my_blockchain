# Welcome to My Blockchain
***

## Task
my_blockchain is a command that creates and manages a blockchain.


## Description
The program creates a blockchain from user input and has the ability to load a blockchain found in a backup file. This program allows the user to add and remove nodes, add and remove blocks, synchronize all of the nodes to contain the same blocks and upon quiting will save the blockchain to file.

This implementation uses arrays and linked lists at each node to link each of the blocks to their corresponding node.

## Installation
1) Download the repository from Gitea 
using https:
```
git clone https://git.us.qwasar.io/my_blockchain_180609_mnattp/my_blockchain.git
```

using ssh:
```
git clone git@git.us.qwasar.io:my_blockchain_180609_mnattp/my_blockchain.git
```

2) Compile to executable file:
```
make my_blockchain
```
The executable will be named my_blockchain.

## Usage
Arguments Key:
add = add
rm = remove
nid = node id
bid = block id
* = all nodes wildcard
sync = synchronize
ls = list nodes
ls -l = list nodes and the blocks belonging to each node

Add a node
```
./my_blockchain add node nid 
```

Remove node(s)
```
./my_blockchain rm node nid 
./my_blockchain rm node * 
```

Add block to node(s)
```
./my_blockchain add block bid nid 
./my_blockchain add block bid * 
```

Remove a block from a node
```
./my_blockchain rm block bid nid 
```

List nodes
```
./my_blockchain ls
```

List nodes and contained blocks
```
./my_blockchain ls -l
```

Synchronize nodes
```
./my_blockchain sync
```

Quit and save blockchain to file
```
./my_blockchain quit
```
