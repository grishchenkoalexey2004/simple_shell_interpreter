#include <stdio.h>
#define MAX_NUMBER_LEN 100
int program_index = 0;// number of string element we are currently working at 
int is_neg = 0;


int is_digit(char sym){
    if ((sym>='0')&&(sym<='9'))
        return 1;
    else
        return 0;
}

double to_power(double number,int pow){
    while (pow>0){
        number = number*10;
        pow--;
    }
    while (pow<0){
        number = number/10;
        pow++;
    }
    return number;
}

int char_to_int(char sym){
    static char code_shift = '0';
    int res = sym-code_shift;
    return res;
}

double process_integer_part(char str[],double res_num){
    int base = 10;
    if (str[program_index]=='+'){
        program_index++;
    }
    else if (str[program_index]=='-'){
        program_index++;
        is_neg = 1;
    }

    while (is_digit(str[program_index])){
        res_num = res_num*base;
        res_num = res_num+char_to_int(str[program_index]);
        program_index++;
    }
    return res_num;
}

double process_fract_part(char str[],double res_num){
    long int ten_power = 10;    
    double fract_part = 0;
    double fract_digit;
    
    if (str[program_index]!='.')
        return res_num;
    program_index++;
    while (is_digit(str[program_index])){
        fract_digit = char_to_int(str[program_index]);
        fract_part = fract_part + ((double) fract_digit)/ten_power;
        ten_power*=10;
        program_index++;
    }
    res_num = res_num+fract_part;
    return res_num;
}

//incorrect (need to check for f|F)
double process_e_part(char str[],double res_num){
    int mult,power;
    char e_sign;
    if ((str[program_index]!='e')&&(str[program_index]!='E'))
        return res_num;

    program_index++;
    e_sign = str[program_index];
    mult = (e_sign=='+')?1:(-1);
    power = 0;
    program_index++;
    while (is_digit(str[program_index])){
        power = power*10;
        power = power+char_to_int(str[program_index]);
        program_index++;
    }
    power = power*mult; //taking into account the fact that power can be negative
    res_num = to_power(res_num,power);
    return res_num;
}


double str2double(char  str[]){
    double res = 0;
    res = process_integer_part(str,res);
    res = process_fract_part(str,res);
    res = process_e_part(str,res);
    return res;
}

int main(){
    double result; 
    char input_string[MAX_NUMBER_LEN];
    while (scanf("%s",&input_string)!=EOF){
        result =str2double(input_string);
        if (is_neg)
            result = result*(-1);
        
        printf("\n%.10g",result);
        program_index = 0;

    }
    printf("\n");

    return 0; 
}

