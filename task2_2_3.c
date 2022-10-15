#include <stdio.h>

int main(){
    int n;
    double coef;
    double x;
    double res = 0;
    scanf("%d",&n);
    scanf("%lf",&x);
    scanf("%lf",&coef);
    res =(x*coef)/(n+1);
    n--;
    for (;n>=0;n--){
        scanf("%lf",&coef);
        res = (res+coef/(n+1))*x;
    }
    printf("\n");
    printf("%.10g",res);
    return 0;
}
