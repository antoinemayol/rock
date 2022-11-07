#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>

void concat_path(char* res,char* s1, char* s2, char* s3, char *s4, int maxpath)
{
	int k = 0;
	for(size_t i = 0; k < maxpath && s1[i] != '\0'; i++)
	{
		res[k]=s1[i];
		k+=1;
	}

	for(size_t i = 0; k < maxpath && s2[i] != '\0'; i++)
	{
		res[k]=s2[i];
		k+=1;
	}

	for(size_t i = 0; k < maxpath && s3[i] != '\0'; i++)
	{
		printf("s3 : %c",s3[i]);
		res[k]=s3[i];
		k+=1;
	}

	for(size_t i = 0; k < maxpath && s4[i] != '\0'; i++)
	{
		res[k]=s4[i];
		k+=1;
	}
}


void save(double *a, int i, size_t len)
{
	char *num = malloc(5*sizeof(char));
	sprintf(num, "%d", i);
	char *path = malloc(25*sizeof(char));

	concat_path(path,"neurones/", "nerons", num,".txt",25);

	FILE *f = fopen(path,"w");
	if(f == NULL){
		errx(1,"Can't open file !");
	}

	for(size_t j = 0; j<len; j++)
	{
		fprintf(f,"%f\n",a[j]);
	}
	fclose(f);

}

//int main()
//{
//	double test[4] = {0.0,0.43,2.34,-65};
//	save(test,0,4);
//	return 0;
//}