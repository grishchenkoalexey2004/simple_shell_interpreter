#include <stdio.h>
//task 2_2_1 ¨ task 2_2_2
int main(){
        double x,res,coef;
        double res2;
        double res0;
        int polynom_pwr;
        int polynom_pwr2;
        scanf("%d",&polynom_pwr);
        scanf("%lf",&x);
        scanf("%lf",&coef);

        res = 0;
        res+=(polynom_pwr)*coef*x;

        res2 =0;
        polynom_pwr2 = polynom_pwr;

        res0 = coef;

        res2 =(x*coef)/(polynom_pwr+1);

        while ((scanf("%lf",&coef))!=EOF){
                polynom_pwr--;  
                polynom_pwr2--; 
                res0= res0*x+coef;
                if (polynom_pwr2>=0)
                    res2 = (res2+coef/(polynom_pwr2+1))*x;
                if (polynom_pwr>1){
                    res = (res+coef*polynom_pwr)*x;
                }
                else{
                    res = res +coef*polynom_pwr;//x**1 and x**0 disappear
                }
        }


        


        printf("%lf",res0);
        printf("\n%lf",res);
        printf("\n%lf",res2);
        return 0;

}
// проблема: если аргументы передаются через пробел, то 
// для завершения программы требуется двойное нажатие ctrl d

    