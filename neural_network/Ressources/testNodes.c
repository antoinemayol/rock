#include <stdio.h>
#include "nerons.h"

int main()
{	
	node tmp = give_node(145,3,1,6,5,"test");
	node tmp1 = give_node(9,8,7,6,5,"test1");
	node tmp2 = give_node(24,3,42,61,5,"test2");

	node test = fromfile(tmp.path);
	printf("alors ? %f", test.weight);
	printf("alors ? %f", test.x1);
	printf("alors ? %f", test.x2);
	printf("alors ? %f", test.w1);
	printf("alors ? %f", test.w2);

}