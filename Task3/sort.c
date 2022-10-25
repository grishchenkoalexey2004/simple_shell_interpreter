#include <stdio.h>
#include <stdlib.h>
#include "list_types.h"
//bubble sort


//returns true if second_word<first_word
int to_swap(buf_type first_word,buf_type second_word){
	char f_w_char,s_w_char;
	int greater_word_num = 2; // 
	int index = 0;
	while (1){
		f_w_char = *(first_word+index);
		s_w_char = *(second_word+index);
		//if one of the words has ended
		if (f_w_char=='\0' || s_w_char == '\0'){
			//if second word is longer than first
			if (s_w_char!='\0')
				greater_word_num =2;
			
			//if first_word is longer than second
			else if (f_w_char!='\0')
				greater_word_num = 1;
			

			//if words are equal we think that second is greater by default
			//(this is in order not to make unnecessary swaps)
			else
				greater_word_num =2;
			break;
		}
		else{
			if (f_w_char!=s_w_char){
				(f_w_char>s_w_char)?(greater_word_num = 1):(greater_word_num = 2);
				break;
			}
			else
				index++;
		}


	}
	return (greater_word_num==2)?0:1;
}

void sort(list_type *list,int arr_len){
	printf("%d",to_swap(**list,*(*list+1)));
	return;
}

