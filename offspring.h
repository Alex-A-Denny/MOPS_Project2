// 
// File: offspring.h
//
// @author: Alex A Denny @aad7700@g.rit.edu
// 
////////////////////////////////////////////

#ifndef OFFSPRING_H
#define OFFSPRING_H

#include <stdlib.h>

/// NTree_S implementation for N-ary tree structure.
typedef struct NTree_S {
    char * name;           ///< name of the person.
    struct NTree_S ** children;   ///< collection of children.
    size_t child_count;          ///< current number of children.
    int height; 		//<hieght of current node
} NTree_S;

/// NTree_p is the "pointer to N-ary tree struct" type.
typedef struct NTree_S * NTree_p;


// Returns a node pointer or NULL if it fails.
//
// const char *name : name of new node
NTree_p create_node( const char *name );


// Frees all memory of the tree and its offspring.
//
// NTree_p tree : tree to be freed
void destroy_tree( NTree_p tree );


// Searches tree for specific node, 
// returns the node pointer is found or NULL if not.
//
// NTree_p tree : tree to search through
// const char * name : person to find
NTree_p find_node( NTree_p tree, const char *name );



// prints the offspring of person to stdout
//
// NTree_p tree : tree to search through
// char * name : person who's offspring to print
void print_tree( NTree_p tree, char *name);


// Finds the named parent, adds the child, and returns 
// the possibly revised tree node pointer
//
// NTree_p tree : tree to search through
// const char *pname : name of parent
// const char *cname : name pf new child
NTree_p add_child( NTree_p tree, const char *pname, const char *cname );

#endif /* OFFSPRING_H*/
