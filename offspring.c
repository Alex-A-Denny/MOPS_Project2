//
// File: offspring.c
//
// @author Alex A Denny aad7700@g.rit.edu
//
//////////////////////////////////////

#include "trimit.h"
#include "offspring.h"
#include <stdio.h>
#include <string.h>


//struct for a queue node
struct QueueNode {
    NTree_p node;//current node
    struct QueueNode* next;//one of node's children
};

//helper funcs
void enqueue(struct QueueNode** front, struct QueueNode** rear, NTree_p node);
NTree_p dequeue(struct QueueNode** front);


// function to enqueue a tree node into the queue
//
// struct QueueNode** front : ptr to front of queue
// struct QueueNode** rear : ptr to back of queue
// NTree_p node : ptr to node to be enqueued
void enqueue(struct QueueNode** front, struct QueueNode** rear, NTree_p node) {
    
	//initilizing new queue node
	struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    	newNode->node = node;
    	newNode->next = NULL;

	//queue logic
    	if (*front == NULL) {
        	*front = newNode;
        	*rear = newNode;
    	}
       	else {
        	(*rear)->next = newNode;
        	*rear = newNode;
    	}
}

//function to dequeue a tree node from the queue
//
// struct QueueNode** front : node to be dequeued
NTree_p dequeue(struct QueueNode** front) {
    	if (*front == NULL) {
        	return NULL;
    	}

	//removes the front node and returns the next
    	struct QueueNode* temp = *front;
    	*front = (*front)->next;
    	NTree_p node = temp->node;
    	free(temp);

    	return node;
}


NTree_p create_node( const char *name ){
	
	//memory allocation
	NTree_p tree = (NTree_p) malloc(sizeof(struct NTree_S));

	//setting up vars
	tree->name  = (char*) malloc(strlen(name) + 1);
	strcpy(tree->name, name);
	tree->children = NULL;
	tree->child_count = 0;
	tree->height = 0;

	return tree;
}

void destroy_tree(NTree_p tree) {

	//tree is already free'd
	if (tree == NULL) {
        	return;
    	}

	//freeing children
    	if (tree->child_count > 0) {
        	
		//recursively destroy from the leaf nodes to root
		for (size_t i = 0; i < tree->child_count; i++) {
            		destroy_tree(tree->children[i]);
        	}
       
        	//free array of child pointers now that it holds
		//no data
        	free(tree->children);
		tree->child_count = 0;
    	}

	//free name
    	if (tree->name != NULL) {
        	free(tree->name);
    	}

    	//only free the node itself if it's a leaf node
	//(it has no children)
    	if (tree->child_count == 0) {
        	free(tree);
    	}
}


NTree_p find_node( NTree_p tree, const char *name ){
	
	//tree has no data
	if(tree == NULL) {
        	return NULL;
    	}

	//substituting for lack of name
	if(name == NULL){
		printf("No name given, printing entire tree as substitue.\n");
		name = tree->name;
	}


	//modifed bfs traversal to find a specific node
    	struct QueueNode* front = NULL;
    	struct QueueNode* rear = NULL;
    	enqueue(&front, &rear, tree);

	
    	while (front != NULL) {
        	
		NTree_p current = dequeue(&front);
		
		if(current == NULL)
			continue;

		//found the right node
        	if (strcmp(current->name, name) == 0) {		
			
			//destroying queue
			while(front != NULL){
				NTree_p remaining = dequeue(&front);
				(void)remaining;//remaining is not set to be used
			}

			return current;
        	}
		
		//enqueue nodes children to continue BFS
        	for (size_t i = 0; i < current->child_count; i++) {
            		enqueue(&front, &rear, current->children[i]);
        	}
    	}
    	
	//node was not in tree
	return NULL;

}



void print_tree( NTree_p tree, char *name){

	//tree has data
    	if (tree == NULL) {
        	printf("Tree is empty.\n");
        	return;
    	}
	
	//substitute target name for root if not given
	if(name == NULL)
		name = tree->name;
	
    	//find the node with the specified name
    	NTree_p currentNode = find_node(tree, name);
	if (currentNode == NULL) {
        	printf("'%s' is not part of the family tree.\n", name);
        	return;
    	}

    	//create a queue for BFS traversal
    	struct QueueNode* front = NULL;
    	struct QueueNode* rear = NULL;
    	enqueue(&front, &rear, currentNode);
	
    	//print children with BFS traversal
    	while (front != NULL) {
        	NTree_p current = dequeue(&front);

        	//print the current node's information
        	printf("%s had ", current->name);

		//print current node's children
        	if (current->child_count >= 1) {
            		
			for (size_t i = 0; i < current->child_count; i++) {
                		
				printf("%s", current->children[i]->name);

				if (i < current->child_count - 2 && current->child_count > 1) {
                    			printf(", ");
                		}
				else if(i == current->child_count - 2 && current->child_count >= 2){
					printf(" and ");
				}
                		
				//enqueue child nodes for BFS traversal
                		enqueue(&front, &rear, current->children[i]);
            		
			}

			printf(".\n");
        	}
	       	else {
            		printf("no offspring.\n");
        	}
    	}

	printf("\n");

}


NTree_p add_child(NTree_p tree, const char *pname, const char *cname) {

	// Root creation
    	if (tree == NULL && pname != NULL) {
        	tree = create_node(pname);
    	}

    	// Find parent and add child
    	if (tree != NULL && pname != NULL && cname != NULL) {
        	
		NTree_p parent = find_node(tree, pname);
        	
		//checking if parent is there
		if (parent != NULL) {
            		
			//checking if parent already has child
			for(size_t i = 0; i < parent->child_count; i++){
				if(parent->children[i]->name == cname){
					fprintf(stderr, "Error: %s already has a child named %s", pname, cname);
					return tree;
				}
			}

			//increment child_count and reallocate space for new_children
            		parent->child_count++;
            		NTree_p *new_children = (NTree_p *)realloc(parent->children, parent->child_count * sizeof(NTree_p));

            		if (new_children == NULL) {//memory allocation failure
                		return tree;
            		}

            		//update parent's children with new_children
            		parent->children = new_children;

            		//creating new child and adding it to parent's children
            		parent->children[parent->child_count - 1] = create_node(cname);
			parent->children[parent->child_count - 1]->height = parent->height + 1;
        		
		}
    	}


	//changing tree root
	if(tree->name == cname){
	
		NTree_p new_root = create_node(pname);

		new_root->children = &tree;
		new_root->child_count = 1;

		struct QueueNode* front = NULL;
    		struct QueueNode* rear = NULL;
    		enqueue(&front, &rear, new_root);

    		while (front != NULL) {
        		NTree_p current = dequeue(&front);

        		if (current == new_root) {//skip incrementing child_count for root node
            			for (size_t i = 0; i < current->child_count; i++) {
                			enqueue(&front, &rear, current->children[i]);
            			}
        		}
	       		else {//increment child_count for non-root nodes
            			current->height++;
	
            			for (size_t i = 0; i < current->child_count; i++) {
                			enqueue(&front, &rear, current->children[i]);
            			}
        		}
    		}

    		//clean up the queue
    		while (front != NULL) {
        		NTree_p current = dequeue(&front);
			
			if (current == new_root) {
                		continue; // Skip freeing the new_root itself
            		}

        		free(current);
    		}

		return new_root;
	}
	
	
	//node doesn't exist
	if( find_node(tree, pname) == NULL && find_node(tree, cname) == NULL ){
		fprintf(stderr, "Error: Neither %s or %s is in the tree.\n", pname, cname);
		
	}
    	

	return tree;
}


int main( int argc, char *argv[] ){

	//initilizing user input and tree vars
	char * input = (char*) malloc(sizeof(char) * 1024);
	NTree_p tree = NULL;
	

	//input validation
	if( argc > 2 ){
		fprintf(stderr, "Error: Usage is ./offspring <filename/file path (optional)>");
		return 1;
	}

	//file input
	if(argc == 2){
	
		//file path given
		FILE* file = fopen(argv[1], "r");
    		
		if (file == NULL) {
        		fprintf(stderr,"Error opening file\n");
        		return 1;
    		}

		
		//adding children
		char * line = (char*) malloc(sizeof(char) * 1024);
		while(fgets(line, 1024, file) != NULL){
			
			if(line[0] == '\n')
				continue;
	
			//getting each parent child relationship in line
			char * trimmed = trim(line);
                        char * token = strtok(trimmed, ",");

			int size = 0;
			int capacity = 10;

			char ** relations = (char **)malloc(sizeof(char *) * capacity);
			
			//creating family relations
			while(token != NULL){
				
				token = trim(token);

				// Resize the array if needed
				if (size == capacity) {
       					capacity *= 2;
       					relations = (char**)realloc(relations, sizeof(char*) * capacity);
       				}
        		
				relations[size] = (char*) malloc(strlen(token) + 1);
				strcpy(relations[size], token);

				size++;
			
				token = strtok(NULL, ",");
			}
			

			//adding children
			for(int i = 1; i < size; i++){
				tree = add_child(tree, relations[0], relations[i]);
			}
			
			//freeing relations
			for (int index = 0; index < size; index++) {
    				free( relations[index] );
			}
			free( relations );
		
		}
		//freeing temp ptrs
		free( line );
		fclose(file);
	
	}
	
	//printing commadns for user
	printf("Commands:\n"
		"- add parent, child to search for parent and add child.\n"
		"– find [name] to search from tree root; print information on name if found.\n"
		"– print [name] to print the breadth first traversal.\n"
		"– size [name] to count of all members in the [sub]tree.\n"
		"– height [name] to return the height of [sub]tree.\n"
		"– init to free all dynamic memory and start with an empty tree.\n"
		"– help to provide command help.\n"
		"– quit to free all dynamic memory and exit.\n");
	

	//user input loop after file
	int active = 1;
	while(active == 1){
		
		//constant user input
		printf("offspring> ");
        	fgets(input, 1024, stdin);
		char * subStr = (char*) malloc(sizeof(char) * 1024);

		char * trimmed = trim(input);
		

		//add command
		if( strncmp(trimmed, "add", 3) == 0 ){
			
			if( strlen(trimmed) == 3 ){
				fprintf(stderr,"Error: usage is add Parent, [child]");
			}

			 char *subStrStart = trimmed + 4;

                        if (*subStrStart != '\0' && *subStrStart != ' ')
                                sscanf(subStrStart, "%[^\n]", subStr);

                        //getting each parent child relationship in line
                        char * token = strtok(subStr, ",");

                        int size = 0;
                        int capacity = 10;

                        char ** relations = (char **)malloc(sizeof(char *) * capacity);

                        //creating family relations
                        while(token != NULL){

                                token = trim(token);

                                // Resize the array if needed
                                if (size == capacity) {
                                        capacity *= 2;
                                        relations = (char**)realloc(relations, sizeof(char*) * capacity);
                                }

                                relations[size] = (char*) malloc(strlen(token) + 1);
                                strcpy(relations[size], token);

                                size++;

                                token = strtok(NULL, ",");
                        }


                        //adding children
                        for(int i = 1; i < size; i++){
                                tree = add_child(tree, relations[0], relations[i]);
                        }

                        //freeing relations
                        for (int index = 0; index < size; index++) {
                                free( relations[index] );
                        }
                        free( relations );
		
		}

		//find node
		if( strncmp(trimmed, "find", 4) == 0 ){

			//parsing user input
                        char *subStrStart = trimmed + 5;

                        sscanf(subStrStart, "%[^\n]", subStr);


			//finding node
                        NTree_p node = find_node(tree, subStr);

			//printing output
                        if(node == NULL){
                                printf("%s is not a part of the family tree.\n", subStr);
                        }
                        else{
                                printf("%s is part of the family tree. Here is their children.\n", node->name);
                                print_tree(node, node->name);
                        }


                }	

		//print tree
		if ( strncmp(trimmed, "print", 5) == 0 ) {
			
			//parsing user input	
			char *subStrStart = trimmed + 6;
    			
			//printing tree
			if (*subStrStart != '\0' && *subStrStart != ' ') {
        			sscanf(subStrStart, "%[^\n]", subStr);
				print_tree(tree, subStr);
    			}
		       	else {
    				print_tree(tree, NULL);
			}
			
		}

		//size
		if(strncmp(trimmed, "size", 4) == 0) {
    			
			//parsing user input
			char *subStrStart = trimmed + 5;
    			char *subStr = (char*) malloc(sizeof(char) * 1024);
			
    			sscanf(subStrStart, "%[^\n]", subStr);
			
    			if(strlen(trimmed) == 4) {
        			strcpy(subStr, tree->name);
    			}
		

			//finding node
    			NTree_p node = find_node(tree, subStr);

			//node not in tree
    			if(node == NULL) {
        			printf("Person does not exist, tree size = 0\n\n");
        			continue;
    			}

			//finding size
    			int tree_size = 0;
			
			//modified bfs traversal to find the size of all children
    			struct QueueNode* front = NULL;
    			struct QueueNode* rear = NULL;
    			enqueue(&front, &rear, node);

    			while (front != NULL) {
        			NTree_p current = dequeue(&front);
        			tree_size += current->child_count;

        			for (size_t i = 0; i < current->child_count; i++) {
            				enqueue(&front, &rear, current->children[i]);
        			}

    			}

			//printing size
    			printf("Tree size for '%s': %d\n\n", subStr, tree_size);

		}

		//hieght
		if(strncmp(trimmed, "height", 6) == 0) {
                        
			//parsing user input
			char *subStrStart = trimmed + 7;
                        char *subStr = (char*) malloc(sizeof(char) * 1024);

                        sscanf(subStrStart, "%[^\n]", subStr);

                        if(strlen(trimmed) == 6) {
                                strcpy(subStr, tree->name);
                        }
			
			//finding node
                        NTree_p node = find_node(tree, subStr);

			//node not found
                        if(node == NULL) {
                                printf("Person does not exist, hieght = -1\n\n");
                                continue;
                        }

			//printing height of node
                        printf("The height for '%s' is %d\n\n", subStr, node->height);

                }

		//free tree to reinitilize
		if( strncmp(trimmed, "init", 4) == 0 ){
			destroy_tree(tree);
			tree = NULL;
		}

		//display commands
		if( strncmp(trimmed, "help", 4) == 0 ){
			printf("Commands:\n"
                		"- add parent, child to search for parent and add child.\n"
                		"– find [name] to search from tree root; print information on name if found.\n"
                		"– print [name] to print the breadth first traversal.\n"
                		"– size [name] to count of all members in the [sub]tree.\n"
                		"– height [name] to return the height of [sub]tree.\n"
                		"– init to free all dynamic memory and start with an empty tree.\n"
                		"– help to provide command help.\n"
                		"– quit to free all dynamic memory and exit.\n");

		}


		//quit
		if( strncmp(trimmed, "quit", 4) == 0 ){
			
			//free tree if haven't done so already
			if(tree != NULL)
				destroy_tree(tree);	
			
			//set active to 0 to end the loop
			active = 0;
		}
		
		//freeing statements
		free( subStr );
	
	}
	
	free( input );
	
	return 0;
}
