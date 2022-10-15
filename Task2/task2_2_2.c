#include <stdio.h>

int main(){
    double x,res,coef;
    double res2;
    int polynom_pwr;
    int polynom_pwr2;
    scanf("%d",&polynom_pwr);
    scanf("%lf",&x);
    scanf("%lf",&coef);

    res = 0;
    res+=(polynom_pwr)*coef*x;

    res2 =0;
    polynom_pwr2 = polynom_pwr;
    res2 =(x*coef)/(polynom_pwr+1);
    
    while ((scanf("%lf",&coef))!=EOF){
        polynom_pwr--;  
        polynom_pwr2--; 
        if (polynom_pwr2>=0)
            res2 = (res2+coef/(polynom_pwr2+1))*x;
        if (polynom_pwr>1){
            res = (res+coef*polynom_pwr)*x;
        }
        else{
            res = res +coef*polynom_pwr;//x**1 and x**0 disappear
        }
    }

   
    printf("\n%lf",res);
    printf("\n");
    printf("%.10g",res2);

    return 0;
}



