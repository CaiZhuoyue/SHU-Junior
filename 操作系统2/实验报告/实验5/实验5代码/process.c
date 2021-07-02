#include<stdio.h>

extern int etext,edata,end;

main()

{

printf("etext:%6x \t edata:%6x \t end:%6 \n",&etext,&edata,&end);

}