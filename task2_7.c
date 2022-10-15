//task2_7 with power function

//new expression grammar

//expr = additive{+-additive}
//additive = multiplier{*/multiplier}
//multiplier = power_base | power_base {^mult}
//power_base  = 0|1|2|3...|9|(expression)

#include<stdio.h>
#include<setjmp.h>

jmp_buf begin;
char curlex;
char sum_lex;
char mult_lex;

void getlex(void); 
int expr(void); 
int add(void); 
int mult(void); 
int power(void);
int raise_to_power(int a,int b);//raises a to b 
void error(); 

int main()
{
	int result;
	setjmp(begin);
	printf("==>");
	getlex();
	result=expr();
	if (curlex != '\n') error();
	printf("\n%d\n",result);
	return 0;
}

//swallows whitespace
void getlex()
{
	while ( (curlex=getchar()) == ' ');
}

void error(void)
{
	printf("\nERROR!\n");
	while(getchar()!='\n');
	longjmp(begin,1);
}

int expr()
{	
	//add procedure forms additive
	int e=add();
	while ((curlex == '+') ||(curlex == '-')){
		sum_lex = curlex; // getlex will get reach next argument and corrupt operation sign
		getlex();
		if (sum_lex=='+')
			e+=add();
		else
			e-=	add();
	}
	
	return e;
}

int add()
{
	int a=mult();
	int mult_res;
	while ((curlex == '*') || (curlex=='/')){
		mult_lex = curlex;
		getlex(); 
		if (mult_lex=='*')
			a*=mult();
		else{
			mult_res = mult();
			if (mult_res==0)
				error();
			a=a/mult();

		}
	}
	return a;
}



int mult()
{
	int res = power();
	if (curlex == '^'){
		getlex();
		res = raise_to_power(res,mult());
	}
	return res;
}

int power(){
	int m;
	switch(curlex){
		case '0': case '1': case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9': m=curlex-'0'; break;
		case '(': getlex(); m=expr();
		if (curlex == ')') break;
		default : error();
	}
	getlex();
	return m;
}

int raise_to_power(int a,int b){
	int multiplier = a;
	int res = 1;
	if (b<0){
		error();
	}
	else{
		for (int i =0;i<b;i++)
			res = res*multiplier;
		return res;
	}
	return 0;
}
