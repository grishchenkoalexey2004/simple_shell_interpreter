#include <stdio.h>
#include <stdlib.h>
#define MAX_NUMBER_LEN 50
#define TRUE_CONST 1
typedef struct node *link;

typedef struct node{
	link l_link;
	link r_link;
	int value; 
}node;

typedef char request_str[MAX_NUMBER_LEN];
typedef link tree;
//function list
int convert_str_to_int(request_str str);
link find_greatest_leaf(link subtree_p); 
void delete_link(link node,char branch);
void create_new_link(link upper_node,link lower_node,char branch);
void append_to_tree(tree *tr_p,int val);
void remove_from_tree(int val);
void find_in_tree(tree tree_p,int val);


tree tree_p = NULL;//global tree variable


int convert_str_to_int(request_str str){
	int input_num = 0;
	int index = 1;
	while ((str[index]>='0') && (str[index]<='9')){
		input_num = input_num*10;
		input_num = input_num + (str[index]-'0');
		index++;
	}

	return input_num;
}

link find_greatest_leaf(link subtree_p){ //finds leaf with greatest value and return pointer to it
	while (subtree_p->r_link)
		subtree_p=subtree_p->r_link;
	return subtree_p;
}

void delete_link(link node,char branch){
	if (branch=='s'){
		tree_p = NULL;
	}
	else if (branch =='l'){
		node->l_link = NULL;
	}
	else
		node->r_link = NULL;
	return;
}

//connects upper_node->branch to lower_node
void create_new_link(link upper_node,link lower_node,char branch){
	if (branch=='s'){ //if we are deleting the root
		 //lower_node becomes the new root
		tree_p = lower_node;
	}
	else if (branch =='r')
		upper_node->r_link = lower_node;
	else
		upper_node->l_link = lower_node;
}

void append_to_tree(tree *tr_p,int val){
	if (*tr_p==NULL){
		*tr_p = malloc(sizeof(node));
		(*tr_p)->value = val;
		(*tr_p)->l_link = (*tr_p)->r_link = NULL;
		return;
	}

	if ((*tr_p)->value==val)
		return;
	else if ((*tr_p)->value>val)
		append_to_tree(&((*tr_p)->l_link),val);
	else
		append_to_tree(&((*tr_p)->r_link),val);
}

void remove_from_tree(int val){
	link tree_p_copy = tree_p;
	link prev_node = tree_p_copy;
	link r_subtr_p;
	link attachment_point;
	char prev_branch = 's';// saving the position we stepped from on the previous step

	if (!tree_p_copy)
		return; // if empty tree 
	while (TRUE_CONST){
		if ( tree_p_copy->value==val){
			if ((tree_p_copy->l_link==NULL)&&(tree_p_copy->r_link==NULL)){
				//if we are dealing with the leaf
				delete_link(prev_node,prev_branch);
			}
			else if ((tree_p_copy->r_link==NULL)||(tree_p_copy->l_link==NULL))
			{ // if we don't have either right or left branch
				if (tree_p_copy->r_link!=NULL)
					create_new_link(prev_node,tree_p_copy->r_link,prev_branch);
				else
					create_new_link(prev_node,tree_p_copy->l_link,prev_branch);

			}
			else{ //if case where is left and right subtree
				r_subtr_p = tree_p_copy->r_link;
				create_new_link(prev_node,tree_p_copy->l_link,prev_branch);// connecting prev_node with left subtree
				attachment_point = find_greatest_leaf(tree_p_copy->l_link);//searching for node 
				//with the greatest val in left subtree 
				//we will attach right subtree to it
				attachment_point->r_link = r_subtr_p;
			}
			free(tree_p_copy);
			break;
		}

		if (tree_p_copy->value>val){
			if (tree_p_copy->l_link){
				prev_node = tree_p_copy;
				prev_branch = 'l';// it means that we have chosen left branch 
				tree_p_copy=tree_p_copy->l_link;
			}
			else
				break;//node with val value can't be found in tree
		}
		else{
			if (tree_p_copy->r_link){
				prev_node = tree_p_copy;
				prev_branch = 'r';// it means that we have chosen right branch 
				tree_p_copy=tree_p_copy->r_link;
			}
			else
				break;
		}
	}
	return;
}

void find_in_tree(tree tree_p,int val){ //prints yes_no
	if (!tree_p){
		printf("\n%d no",val);
		return;
	}
	
	if (tree_p->value ==val){
		printf("\n%d yes",val);
		return;
	}
	else if (tree_p->value>val)
		find_in_tree(tree_p->l_link,val);
	else
		find_in_tree(tree_p->r_link,val);
}


int main(){
	request_str req_str;
	char op_char;
	int input_num;
	while (scanf("%s",req_str)!=EOF){
		op_char = req_str[0];
		input_num = convert_str_to_int(req_str);
		switch (op_char){
			case '+':append_to_tree(&tree_p,input_num);
				break;
			case '-':remove_from_tree(input_num);
				break;
			case '?':find_in_tree(tree_p,input_num);
				break;
		}
	}
	return 0;

}
