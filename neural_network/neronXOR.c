#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>
#include "save_and_load.h"
//#include "data_struc.h"

//Sigmoid function
double sig(double z)
{
	return 1 / (1 + exp(-z));
}

//Derivate of sigmoid function
double Dsig(double z)
{
	return z * (1 - z);
}

//Randomly mix an array
void mix(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

#define nbIn 225
#define nbHidLay 1
#define nbHidNod 122
#define nbOut 9
/*
double testMat6[225] =   	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
							 0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
							 0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,
							 0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
							 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
							 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
							 0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
							 0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
							 0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,
							 0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,
							 0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,
							 0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,
							 0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

double testMat3[225] =   	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							 0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
							 0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
							 0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
							 0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
							 0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
							 0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
							 0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
							 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
*/

//The network look like this :
// Input 1 -- O -- O
// 	           \ /  \
//              X    O -- Output
//             / \  /
// Input 2 -- O -- O 

//Créer fonctions qui créer le réseau et l'init

#define nbTest 2

//Proceeding the creation and the application of neural network for XOR
void proceed(int limit)
{
	//Setting learning rate
	double pas = 0.1f;
	
	//Arrays of neurons
	double *hidLay = malloc(nbHidNod * sizeof(double));
	double *outLay = malloc(nbOut * sizeof(double));

	//Arrays of neurons bias
	double *hidLayBias = malloc(nbHidNod * sizeof(double));
	double *outLayBias = malloc(nbOut * sizeof(double));

	//Arrays of Weight
	double *hidWght = malloc((nbIn * nbHidNod) * sizeof(double));
	double *outWght = malloc((nbHidNod * nbOut) * sizeof(double));
	
	//Inputs and expected outputs
	double trainIn[nbTest][nbIn] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							 		0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
							 		0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
							 		0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,
							 		0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
							 		0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
							 		0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
							 		0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
							 		0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,
							 		0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,
							 		0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,
							 		0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,
							 		0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
							 		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 

							   		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							 		0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
							 		0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
							 		0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
							 		0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
							 		0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
							 		0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
							 		0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
							 		0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
							 		0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
							 		0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
							 		0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
							 		0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
							 		0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
							 		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
								   };

    double trainOut[nbTest][nbOut] = {{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f},
									  {0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}};
								// 	    1     2   3    4    5    6    7    8    9
	int order[2] = {0,1};
	
	//Initialize layers with random value close to 0
	for(int i = 0; i < nbIn; i++)
	{
		for(int j = 0; j < nbHidNod; j++)
		{
			hidWght[i * nbHidNod + j] = (double)rand()/(double)RAND_MAX;
		}
	}

	for(int i = 0; i < nbHidNod; i++)
	{
		hidLayBias[i] = (double)rand()/(double)RAND_MAX;
		for(int j = 0; j < nbOut; j++)
		{
			outWght[i * nbOut + j] = (double)rand()/(double)RAND_MAX;

		}
	}

	for(int i = 0; i < nbOut; i++)
	{
		outLayBias[i] = (double)rand()/(double)RAND_MAX;
	}

	//-----
	printf("rand : %f\n",outLayBias[0]);
	printf("rand : %f\n",outLayBias[1]);
	printf("rand : %f\n",outLayBias[2]);
	printf("rand : %f\n",outLayBias[3]);
	printf("rand : %f\n",outLayBias[4]);
	printf("rand : %f\n",outLayBias[5]);
	printf("rand : %f\n",outLayBias[6]);

	for(int step = 0; step < limit; step++)
	{
		mix(order, nbTest);

		for(int t = 0; t<nbTest; t++)
		{
			int i = order[t];

			//START FORWARD PROPAGATION
			
			for(int j = 0; j < nbHidNod; j++)
			{
				double z = hidLayBias[j];
				for(int k = 0; k < nbIn; k++)
				{
					z +=  trainIn[i][k] * hidWght[k * nbHidNod + j];
				}
				hidLay[j] = sig(z);
			}

			for(int j = 0; j < nbOut; j++)
			{
				double z = outLayBias[j];
				for(int k = 0; k < nbHidNod; k++)
				{
					z += hidLay[k] * outWght[k*nbOut + j];
				}
				outLay[j] = sig(z);
			}

			//Printing results as they come
			//if(limit-step <= 1)
			//{
		    	printf ("Input : %d \nOutputs :\n 1: %f\n 2: %f\n 3: %f\n 4: %f\n 5: %f\n 6: %f\n 7: %f\n 8: %f\n 9: %f\n Expected Output: %g\n\n",
                    	i, outLay[0], outLay[1], outLay[2], outLay[3], outLay[4], outLay[5], outLay[6], outLay[7], outLay[8],
						trainOut[i][0]);
			//}
			//-----

			//BACKPROPAGATION

			//Compute difference between result and expected
			double *deltaOut = malloc(nbOut * sizeof(double));
			for(int j =0; j < nbOut; j++)
			{
				double errorOut = (trainOut[i][j] - outLay[j]); 
				deltaOut[j] = errorOut * Dsig(outLay[j]);
			}

			double *deltaHid = malloc(nbHidNod * sizeof(double));
			for(int j = 0; j<nbHidNod; j++)
			{
				double errorHid = 0.0f;
				for(int k = 0; k < nbOut; k++)
				{
					errorHid += deltaOut[k] * outWght[j * nbOut + k];
				}
				deltaHid[j] = errorHid * Dsig(hidLay[j]);
			}
			
			//Update weights between nodes
			for(int j = 0; j < nbOut; j++)
			{
				outLayBias[j] += deltaOut[j] * pas;
				for(int k = 0; k<nbHidNod; k++)
				{
					outWght[k * nbOut + j] += hidLay[k] * deltaOut[j] * pas;
				}
			}

			for(int j = 0; j < nbHidNod; j++)
			{
				hidLayBias[j] += deltaHid[j] * pas;
				for(int k = 0; k<nbIn; k++)
				{
					hidWght[k * nbHidNod + j] += trainIn[i][k] * deltaHid[j] * pas;
				}
			}
		}
	}
	//Save neurons, weights and biases in "neurones/" folder
	save(hidWght,1,4);
	save(hidLay,2,2);
	save(hidLayBias,3,2);
	save(outWght,4,2);
	save(outLay,5,1);
	save(outLayBias,6,1);

	//Release memory
	free(hidLay);
	free(hidLayBias);
	free(outLay);
	free(outLayBias);
	free(hidWght);
	free(outWght);
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		errx(1,"invoke command with 1 arguments !\n");
	}
	
	int limit = atoi(argv[1]);
	printf("SETTED LIMIT : %d\n\n",limit);

	if(limit < 0 || limit > 50000)
	{
		errx(1,"limit is too high !\n");
	}
	
	proceed(limit);
	return 0;

}
