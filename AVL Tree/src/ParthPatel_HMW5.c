/*
 ============================================================================
 Name        : ParthPatel_HMW5.c
 Author      : Parth Patel
 Version     :
 Copyright   : Your copyright notice
 Description : A AVL that does various functions
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LH +1 //numbers corresponding to each balance state
#define EH 0
#define RH -1

struct node //each node in tree
{
	char word[10];
	char desc[100];
	struct node *right_child;
	struct node *left_child;
	int balance;
}; typedef struct node node;

struct tree //tree that points to the root
{
	int count;
	node *root;

}; typedef struct tree tree;

node* search(node *root, char find[10]) //searches the tree for a word
{
	if (root == NULL) { //word is never found or list is empty
		printf("word not found\n\n");

		return root;
	}
	else if (strcasecmp(root->word, find) == 0) { //if word is found then prints the word
		printf(" %s was found with description: %s \n\n", root->word, root->desc);
		return root;
	}
	else if (strcasecmp(find, root->word) > 0) // //if word is greater then the current root search with the right of the current root
		return search(root->right_child, find);
	else //if word is smaller then current word call search again with the new root
		return search(root->left_child, find);
}

tree* create_tree() { //initializes the tree
	tree* newtree;
	newtree = (tree*)malloc(sizeof(tree));
	if (newtree)
	{
		newtree->root = NULL;
		newtree->count = 0;
	}

	return newtree;
}




node* rotateLeft(node* root) //rotate left at node
{
	node* temp;

	temp=root->right_child;
	root->right_child=temp->left_child;
	temp->left_child=root;
	return temp;
}


node* rotateRight(node* root) //rotate right at node
{
	node* temp;

	temp=root->left_child;
	root->left_child=temp->right_child;
	temp->right_child=root;
	return temp;
}

node* insert_leftbalance(node* root,bool* taller) //balances left for insert
	{
		node* rightTree;
		node* leftTree;

		leftTree=root->left_child; //left subtree
		switch(leftTree->balance) //checks balance of left subtree
		{
		case LH: //if left high then rotating right will change everything to EH since only one rotation is required
				root->balance=EH;
				leftTree->balance=EH;

				root=rotateRight(root);
				*taller=false;
				break;
		case EH: //if EH then insert left should not have been called since if something was inserted it is no longer balanced
				printf("\nSomehow broke left balance\n");
				exit(100);
		case RH: //if RH then that means 2 balances are needed
				rightTree=leftTree->right_child; //right subtree of left subtree
				switch(rightTree->balance)
				{
				case LH: //if right subtree is LH then rotating will result in RH and the left subtree becomes EH
						root->balance=RH;
						leftTree->balance=EH;
						break;
				case EH: //if EH then after rotations current node becomes EH and left subtree becomes LH
						root->balance=EH;
						leftTree->balance=LH;
						break;
				case RH: //If RH then current node becomes EH and left subtree becomes LH after rotations
						root->balance=EH;
						leftTree->balance=LH;
						break;
				}
		rightTree->balance=EH;
		root->left_child=rotateLeft(leftTree); //subtree has to be rotated before the node can be rotated since the node is RH

		root=rotateRight(root); //rotating everything right will balance out the tree
		*taller=false;
		}
		return root;
		}

node*	insert_rightbalance(node* root,bool* taller) //balances right for insert
		{
			node* rightTree;
			node* leftTree;

			rightTree=root->right_child; //right subtree
			switch(rightTree->balance)
			{
			case RH: //if RH then only one rotation is required
					root->balance=EH;
					rightTree->balance=EH;
					root=rotateLeft(root);
					*taller=false;
					break;
			case EH:
					printf("\nSomehow broke left balance\n");
					exit(100);
			case LH: //if LH then that means two rotations are required
					leftTree=rightTree->left_child;
					switch(leftTree->balance)
					{
					case RH:
							root->balance=LH;
							leftTree->balance=EH;
							break;
					case EH:
							root->balance=EH;
							leftTree->balance=LH;
							break;
					case LH:
							root->balance=EH;
							rightTree->balance=RH;
							break;
					}
			leftTree->balance=EH;
			root->right_child=rotateRight(rightTree);

			root=rotateLeft(root);
			*taller=false;
			}
			return root;
			}



node* insert_node(tree* newtree,node *root,node*p,bool* taller) //internal insert
{

	if(!root)
	{
		root=p;
		*taller=true;
		return root;
	}

	if (strcasecmp(p->word, root->word) < 0) { //if word is greater then check the left of the current root
		root->left_child= insert_node(newtree,root->left_child,p,taller);

		if(*taller) //if taller flag is active
			switch(root->balance)
			{
			case LH: //if LH then balance is needed since we will be adding another node to left
					root=insert_leftbalance(root,taller);
					break;
			case EH: //if EH then adding left will change to LH
					root->balance=LH;
					break;
			case RH: //if RH then adding to left will balance out
					root->balance=EH;
					*taller=false;
					break;

			}
		return root;
	}
	else //adding to right
	{
		root->right_child=insert_node(newtree,root->right_child,p,taller);

		if(*taller)
					switch(root->balance)
					{
					case LH: //if LH then adding to right will balance out
							root->balance=EH;
							*taller=false;
							break;
					case EH: //if EH then adding right will make it RH
							root->balance=RH;
							break;
					case RH: //if RH already then adding right will require balance
							root=insert_rightbalance(root,taller);
							break;
					}
		return root;
	}
	return root;
	}


bool add_node(tree* newtree,char newword[10], char newdesc[100]) //creates the node and calls internal insert
{
	bool Taller;
	node *p;

	p = (node*)malloc(sizeof(node));
	if(!p)
		return false;

	newword = strlwr(newword);
	newdesc = strlwr(newdesc);
	strcpy(p->word, newword);
	strcpy(p->desc, newdesc);
	p->left_child = NULL;
	p->right_child = NULL;
	p->balance= EH;
	newtree->root=insert_node(newtree,newtree->root,p,&Taller);
	printf(" %s was added with description: %s \n\n", p->word,p->desc);
	(newtree->count)++;
	return true;
}

node* deleteLeftBalance(node* root,bool* shorter) //balance for deleting from left
{
	node* rightTree;
	node* leftTree;

	switch(root->balance) //current node
	{
	case RH: //if current node is RH then deleting from left will even out
		root->balance=EH;
		break;
	case EH: //if EH then deleting left will cause it to become shorter and LH
		root->balance=LH;
		*shorter=false;
		break;
	case LH: //if LH then need to check the subtree for two rotations
		leftTree=root->left_child;//left child of current node
		if(leftTree->balance==RH)
		{
			rightTree=leftTree->right_child; //right subtree of left child of current node

			switch(rightTree->balance) //what will happen after the two rotations
			{
			case RH: //if right subtree RH then delete will cause left child to become LH and current node becomes balanced
				leftTree->balance=LH;
				root->balance=EH;
				break;
			case EH: //if EH then everything should become balanced
				root->balance=EH;
				leftTree->balance=EH;
				break;
			case LH: //if LH then current node becomes RH and left child becomes EH
				root->balance=RH;
				leftTree->balance=EH;
				break;
			}
			rightTree->balance=EH;
			root->left_child=rotateLeft(leftTree);
			root=rotateRight(root);
		}
		else //if leftTree is LH then only one rotation is required
		{
			switch(leftTree->balance)
			{
			case RH:
					printf("Broke left balance\n");
					exit(100);
			case LH:
				root->balance=EH;
				leftTree->balance=EH;
				break;
			case EH:
					root->balance=RH;
					leftTree->balance=LH;
					*shorter=false;
					break;
			}
			root=rotateRight(root);

		}
	}
	return root;
	}


	node* deleteRightBalance(node* root,bool* shorter) //right balance for delete
	{
		node* rightTree;
		node* leftTree;

		switch(root->balance) //current node
		{
		case LH: //if LH then deleting from left will even it out
			root->balance=EH;
			break;
		case EH: //if EH then it will become RH
			root->balance=RH;
			*shorter=false;
			break;
		case RH: //If RH then two rotations could be required and right child has to be checked
			rightTree=root->right_child;
			if(rightTree->balance==LH) //if right child is LH then two rotations will be required
			{
				leftTree=rightTree->left_child;

				switch(leftTree->balance) //how the balance will change after the two rotations happen
				{
				case LH: //if left child of right child is LH then right child will become RH and current node becomes EH
					rightTree->balance=RH;
					root->balance=EH;
					break;
				case EH: //if EH then everything will balance out
					root->balance=EH;
					rightTree->balance=EH;
					break;
				case RH: //if RH then current node becomes LH and right child becomes EH
					root->balance=LH;
					rightTree->balance=EH;
					break;
				}
				leftTree->balance=EH;
				root->right_child=rotateRight(rightTree);
				root=rotateLeft(root);
			}
			else //only one rotation required
			{
				switch(rightTree->balance)
				{
				case LH:
						printf("Broke right balance\n");
						exit(100);
				case RH:
					root->balance=EH;
					rightTree->balance=EH;
					break;
				case EH:
						root->balance=RH;
						rightTree->balance=LH;
						*shorter=false;
						break;
				}
				root=rotateLeft(root);
			}
		}
		return root;
		}




node* internal_delete(tree* newtree,node* root,char delword[10],bool* shorter,bool* success) //internal delete
{
node* dltPtr;
node* temp;
node*newRoot;

if(!root) //empty tree
{
	*shorter=false;
	*success=false;
	return NULL;
}

if (strcasecmp(delword, root->word) < 0) //if word is smaller then current node
{
	root->left_child=internal_delete(newtree,root->left_child,delword,shorter,success);
	if(*shorter)//if balance required
		root=deleteRightBalance(root,shorter);
}

else if (strcasecmp(delword, root->word) > 0) //if word is larger then current node
{
	root->right_child=internal_delete(newtree,root->right_child,delword,shorter,success);

	if(*shorter)//if balance required
		root=deleteLeftBalance(root,shorter);
}
else//found word and need to check children if there are any
{
	dltPtr=root; //points to node needing to be deleted so it can be deleted easier and new root can be found
	if(!root->right_child)//if no right child then root becomes left child
	{
		newRoot=root->left_child;
		*success=true; //word deleted and tree becomes shorter so balance needed
		*shorter=true;
		printf("Word was deleted\n");
		free(dltPtr);
		return newRoot;
	}
	else
		if(!root->left_child)//if no left child then root becomes right child
		{
			newRoot=root->right_child;
			*success=true; //word deleted and tree becomes shorter so balance needed
			*shorter=true;
			printf("Word was deleted\n");
			free(dltPtr);
			return newRoot;
		}
		else //two children
		{
			temp=root->left_child; //hold the left child value
			while(temp->right_child){ //finds the predecessor
				temp=temp->right_child;
			}

				strcpy(root->word,temp->word);//predecessor becomes new root
				strcpy(root->desc,temp->desc);
				root->left_child=internal_delete(newtree,root->left_child,root->word,shorter,success); //after transferring old child is deleted

				if(*shorter) //as long as child not null then tree needs to be balanced
				{
					root=deleteRightBalance(root,shorter);
				}
		}
		}
	return root;
}



bool delete_node(tree* newtree, char newword[10])//deletes word and sees if it was deleted
{
	bool shorter;
	bool success;
	node* newroot;

	newroot=internal_delete(newtree,newtree->root,newword,&shorter,&success);

	if(success)
	{
		newtree->root=newroot;
		(newtree->count)--;
		return true;
	}
	else{

		printf("\nWord was not found\n");
		return false;
	}
}



void display_tree(node *root)
{

	if (root != NULL) //makes sure that there is a tree
	{
		display_tree(root->left_child); //starts left
		printf("%-10s        %-10s \n\n", root->word, root->desc); //after printing the left child prints parent of it
		display_tree(root->right_child);// prints the right child
	}

}

int main()
{
	tree *newtree;
	newtree = create_tree();
	FILE *fp;
	 char menu_option,newword[10],newdesc[100],filename[20];
	 do{//prints menu as long as the q option is not pressed
		printf("Main Menu\n");
		printf("What would you like to do?\n");
		printf("a. Build AVL from a file\n");
		printf("b. Insert a new word and description\n");
		printf("c. Delete a word\n");
		printf("d. Search for a word\n");
		printf("e. Display the tree in alphabetical order\n");
		printf("q. Press q at this menu to quit.\n\n");
		fflush(stdout);
		scanf(" %c",&menu_option);
		fflush(stdin);


		switch(menu_option){
		case 'a': //build from file

			printf("Enter the name of the file \n"); //asks the user for a file
			fflush(stdout);
			scanf("%s",filename);
			if((fp= fopen(filename,"r"))==NULL){ //makes sure file can be opened
			printf("Error opening file\n");

		}
			while(fscanf(fp,"%s\t%[^\n]", newword, newdesc)!=EOF) //if file is opened then reads line by line following the format until EOF
			{
			add_node(newtree,newword,newdesc); //inserts each word as it is read
			}
			fclose(fp); //closes file after reading
			break;
		case 'b': //insert new word
			fflush(stdin);
			printf("Enter a word \n"); //asks for a word and description and then adds to tree
			fflush(stdout);
			scanf(" %10s",newword);
			fflush(stdin);
			printf("Enter a description\n");
			fflush(stdout);
			scanf("%[^\n]",newdesc); //reads until end of line so that spaces and sentences can be used for description
			fflush(stdin);
			add_node(newtree,newword,newdesc); //inserts the word making sure that it creates a root if it is empty
			printf("\n");
			fflush(stdout);
			break;
		case 'c': //delete_node a word
			printf("Enter a word \n"); //gets a word from the user then deletes
			fflush(stdout);
			scanf(" %10s",newword);
			fflush(stdin);
			delete_node(newtree,newword);
			break;
		case 'd': //searches for a word
			printf("Enter a word \n"); //searches for a word and then displays it
			fflush(stdout);
			scanf(" %10s",newword);
			fflush(stdin);
			search(newtree->root,newword);
			break;
		case 'e': //display the current tree
			printf("\nThe current Tree contains:\n");
			printf("Word:             Description: \n\n");
			display_tree(newtree->root);
			break;
		case 'q': //breaks out if q is pressed
			break;
		default: //if other key is pressed then the error message is printed
			printf("invalid input\n");
				break;
		}
	}while(menu_option!='q');

	return 0;
}
