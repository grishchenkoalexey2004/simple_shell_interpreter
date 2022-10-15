#include <stdio.h>
#define coef 0.5
#define zero 0
double eps;
double calc_sq_root(double arg);
double calc_difference(double arg1,double arg2);
double calc_x_next(double x_arg,double x_next);


int main(){
	double x;
	double result;
	scanf("%lf",&eps);
	while (scanf("%lf",&x)!=EOF){
		result = calc_sq_root(x);
		printf("\n%.10g",result);
	}
	return 0;
}


double calc_sq_root(double arg){
	double x_prev = 1;
	double x_next = calc_x_next(arg,x_prev);
	if (arg == zero)
		return 0;
	else{
		while (calc_difference(x_prev,x_next)>=eps){
			x_prev = x_next;
			x_next = calc_x_next(arg,x_prev);
		}	
		return x_next;
	}
}
	

double calc_x_next(double x_arg,double x_prev){
	double res =coef*(x_prev+(x_arg/x_prev));
	return res;
}

double calc_difference(double arg1,double arg2){
	double return_val =arg1-arg2;
	if (return_val<0){
		return_val*=(-1);
	} 
	return return_val;
}
