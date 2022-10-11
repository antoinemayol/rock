typedef struct node 
{
	char* name;
	double weight;
	double x1;
	double x2;
	double w1;
	double w2;
	char* path;
} node;

node give_node(double wht, double _x1, double _x2, double _w1, double _w2, char* _name);
void refresh(node *n);
