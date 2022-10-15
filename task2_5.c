#include <stdio.h>
#include <stdlib.h>
typedef struct word_node *word_link;
typedef struct word_list_node *list_link;

typedef struct word_node {
	char letter;
	word_link next_letter;

} word_node;

typedef struct word_list_node {
	word_link this_word;
	list_link next_word;
	list_link prev_word;

} word_list_node;

void print_word(word_link word){
	if (!word)
		return;

	while (word){
		printf("%c",word->letter);
		word = word->next_letter;

	}
	printf(" ");
	return;
}

void print_word_list(list_link word_list_elem){
	while (word_list_elem){
		print_word(word_list_elem->this_word);
		word_list_elem = word_list_elem->next_word;
	}
	printf("\n");
	return;
}

//pointer to the first elem of the list is turned to NULL
void clear_word_list(list_link *word_list){
	if ((*word_list)->next_word==NULL){
		free(*word_list);
		*word_list = NULL;
	}
	else{
		clear_word_list(&(*word_list)->next_word);
		free(*word_list);
		*word_list= NULL;
	}
}

//returns pointer to the word stored in the last list elem 
word_link get_last_word(list_link word_list){
	if (!word_list)//if list is empty
		return NULL;
	while (word_list->next_word)
		word_list = word_list->next_word;

	return word_list->this_word;
}

//returns 1 if words are equal
int words_equal(word_link word1, word_link word2){
	do{
		if ((word1->letter)!=(word2->letter)){
			return 0;
		}
		else{
			word1 = word1->next_letter;
			word2 = word2->next_letter;
		}
	} while ((word1!=NULL)&&(word2!=NULL));

	if ((word1==NULL)&&(word2==NULL))
		return 1;//return 1 if we reached ends of words simultaneously, otherwise we can be sure they have
	//different lengths
	return 0;
}

void delete_word(word_link *word){//word isn't empty 
	//after the deletion pointer to the word will point at null
	word_link this_letter = *word;
	word_link n_letter = this_letter->next_letter;
	while (n_letter){
		free(this_letter);
		this_letter = n_letter;
		n_letter = n_letter->next_letter;
	}
	free(this_letter);
	*word = NULL;//nulling the pointer
	return;
}

//deletes list item (without changing pointer value )
void delete_list_elem (list_link word_list_elem){//our word_list contains >=2 elems
	//connecting to neighbouring words directly
	if (word_list_elem->prev_word){
		word_list_elem->prev_word->next_word = word_list_elem->next_word;
		if (word_list_elem->next_word)// if we are not in the end of array 
			word_list_elem->next_word->prev_word = word_list_elem->prev_word;
	}
	delete_word(&(word_list_elem->this_word));
	free(word_list_elem);
	return;
}

//w_l_p contains address of pointer to the first word in word list
void append_to_w_list(list_link *w_l_p,word_link new_word){
	list_link new_word_node = malloc(sizeof(word_list_node));
	new_word_node->next_word = NULL;
	new_word_node->this_word = new_word;
	list_link last_word_node = *w_l_p;

	if (!(*w_l_p)){ // if word list is empty 
		new_word_node->prev_word = NULL;
		*w_l_p = new_word_node;

	}
	else{
		while (last_word_node->next_word)
			last_word_node = last_word_node->next_word;

		new_word_node->prev_word = last_word_node;
		last_word_node->next_word = new_word_node;
	}
	return;
}	

//cur_w_p contains the adress of the pointer to last symbol of the word 

void append_to_w(word_link *cur_w_p,char letter){ 
	word_link new_node = malloc(sizeof(word_node));
	new_node->letter = letter;
	new_node->next_letter = NULL;
	word_link last_letter_p = *cur_w_p;
	if (!(*cur_w_p)){//if we are dealing with the first letter of the word 
		*cur_w_p = new_node;
	}
	else{
		while (last_letter_p->next_letter)
			last_letter_p=last_letter_p->next_letter;
		last_letter_p->next_letter = new_node;
	}
	return;
}


int main(){
	char input_char;
	int in_word = 0;
	list_link word_list_p = NULL;
	word_link cur_word_p = NULL;
	word_link last_word_p;
	list_link aux_p1,aux_p2;
	//pointers word_list_p and cur_word_p always point at first elem 
	while ((input_char=getchar())!=EOF){
		if ((input_char==' ')||(input_char=='\t')){
			if (in_word==1){
				append_to_w_list(&word_list_p,cur_word_p);
				in_word = 0;
				cur_word_p = NULL;
			}
		}
		else{
			in_word =1;
			append_to_w(&cur_word_p,input_char);
		}
	}
	if (in_word==1)
		append_to_w_list(&word_list_p,cur_word_p);

	//word list creation finished
	last_word_p = get_last_word(word_list_p);
	aux_p1 = word_list_p;

	if (!word_list_p)
		return 0;

	while (aux_p1->next_word!=NULL){//while we haven't reached the last word
		if (words_equal(last_word_p,aux_p1->this_word)){
			aux_p2=aux_p1;//aux_p2 points at the element that has to be deleted
			if (aux_p1->prev_word==NULL){ //if we are deleting first elem, we need to set new beginning of the list
				aux_p1 = aux_p1->next_word;
				aux_p1->prev_word = NULL;
				word_list_p = aux_p1;
			}
			else{
				aux_p1 = aux_p1->next_word;
			}
			delete_list_elem(aux_p2);
		}
		else
		{
			aux_p1 = aux_p1->next_word;
		}
	}
	//print_word_list(word_list_p);
	clear_word_list(&word_list_p);
	print_word_list(word_list_p);

	return 0;
}
