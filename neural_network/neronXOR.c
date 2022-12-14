#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <err.h>
#include <time.h>
#include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "save_and_load.h"
//#include "data_struc.h"

/*Converts a Surface pixel to a Pixel*/
Uint32 get_pixel2(SDL_Surface *surface, int x, int y)
{
    int b = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + x * surface->pitch + y * b;

    switch (b)
    {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;
    }
}

/*Creates an Image from the built surface*/
double* create_ar(SDL_Surface *surface)
{
    //Initializing parameters
    int w = surface->w;
    int h = surface->h;
    SDL_Color color;
    double* res = malloc(w * h * sizeof(double));
    //filling image pixels one by one from surface pixels
    for( int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            Uint32 pix = get_pixel2(surface, i, j);
            SDL_GetRGB(pix, surface->format, &color.r, &color.g, &color.b);
            if(color.r >=200)
                res[i * w +j] = 0;//- color.r / 255;
            else
                res[i*w+j] = 1;
        }
    }
    return res;
}

//get the maximum value in in the "arr" double array
int get_maxi(double* arr)
{
    int i = 0;
    int res = 0;
    while(i < 10)
    {
        printf("mx = %f\n",arr[i]);
        if(arr[i] >= arr[res])
        {
            res = i;
        }
        i++;
    }
    return res;
}


/* generate a random floating point number from min to max */
double randfrom(double min, double max)
{

    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

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

#define nbIn 625
#define nbHidLay 1
#define nbHidNod 122
#define nbOut 10

#define nbData 17
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

#define nbTest 1

//Open the "num" folder and return the double array of 0 and 1 representing
//its files
double** get_trainset(char* num )
{
    double** res = malloc(nbData * sizeof(625 * sizeof(double)));
    struct dirent **namelist;
   	int n;
	int i = 0;
    int count = 0;

    char* dir = malloc(10*sizeof(char));
    strcpy(dir,"dataset/");
    strcat(dir,num);
    strcat(dir,"/");

   	n = scandir(dir, &namelist, NULL, alphasort);
   	if (n < 0)
       	perror("scandir");
   	else
	{
       	while (i<n)
		{
            if(namelist[i]->d_name[0] != '.')
            {
                char* fi = malloc(20 * sizeof(char));
                strcpy(fi,namelist[i]->d_name);

                char* tmpdi = malloc(30 * sizeof(char));
                strcpy(tmpdi,dir);
                strcat(tmpdi,fi);
                SDL_Surface *surface = IMG_Load(tmpdi);

                if(surface == NULL)
                    printf("hah loser\n");

                double* tmp = create_ar(surface);
                res[count] = tmp;

                count++;

                free(surface);
                free(fi);
                free(tmpdi);
            }
            i++;
       	}
       	free(namelist);
   	}
	return res;


}

//Proccess the training until "limit" is reached for the file num
void proceed(int limit, char* num)
{
    int nub = atoi(num);
    if(nub > nbData || nub < 0)
    {
        errx(1,"num must be between 0 and 9");
    }

	//Setting learning rate
	double pas = 0.01f;

	//Arrays of neurons
	double *hidLay = load("neurones/nerons2.txt");
	double *outLay = load("neurones/nerons5.txt");

	//Arrays of neurons bias
	double *hidLayBias = load("neurones/nerons3.txt");
	double *outLayBias = load("neurones/nerons6.txt");

	//Arrays of Weight
	double *hidWght = load("neurones/nerons1.txt");
	double *outWght = load("neurones/nerons4.txt");

	//Inputs and expected outputs
	double** trainIn = get_trainset(num);

    double trainOut[nbOut][nbOut] = {{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}};
                               //      0    1     2   3    4    5    6    7    8    9
    for(int i = 0; i < nbOut; i++)
    {
        trainOut[i][i] = 1.0f;
    }
	int order[18] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};

	//-----
	for(int step = 0; step < limit; step++)
	{
		mix(order, nbData);

		for(int t = 0; t < nbOut /*nbData*/ ; t++)
		{
			int i = t;
			//START FORWARD PROPAGATION
			for(int j = 0; j < nbHidNod; j++)
			{
				double z = hidLayBias[j];
				for(int k = 0; k < nbIn; k++)
				{
					z +=  trainIn[i][k] * hidWght[k * nbHidNod + j];//old : k * nbhidnod + k
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
			if(limit - step <= 1)
			{
		    	printf ("Input : %d \nOutputs :\n 0 : %f\n 1: %f\n 2: %f\n 3: %f\n 4: %f\n 5: %f\n 6: %f\n 7: %f\n 8: %f\n 9: %f\n",
                    	i, outLay[0], outLay[1], outLay[2], outLay[3], outLay[4], outLay[5], outLay[6], outLay[7], outLay[8], outLay[9]);
                printf("files : %s\n",num);
			}
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
	//and release memory
	save(hidWght,1,nbIn*nbHidNod);
	free(hidWght);
	save(hidLay,2,nbHidNod);
	free(hidLay);
	save(hidLayBias,3,nbHidNod);
	free(hidLayBias);
	save(outWght,4,nbOut*nbHidNod);
	free(outWght);
	save(outLay,5,nbOut);
	free(outLay);
	save(outLayBias,6,nbOut);
	free(outLayBias);

    free(trainIn);
}

//Reset the file ton random value between -0,5 and 0,5
void set_scratch()
{
	//Arrays of neurons
	//double *hidLay = load("neurones/nerons2.txt");
    double *hidLay = malloc(nbHidNod * sizeof(double));
	//double *outLay = load("neurones/nerons5.txt");
    double *outLay = malloc(nbOut * sizeof(double));

	//Arrays of neurons bias
	//double *hidLayBias = load("neurones/nerons3.txt");
    double *hidLayBias = malloc(nbHidNod * sizeof(double));
	//double *outLayBias = load("neurones/nerons6.txt");
    double *outLayBias = malloc(nbOut * sizeof(double));

	//Arrays of Weight
	//double *hidWght = load("neurones/nerons1.txt");
    double *hidWght = malloc((nbIn * nbHidNod) * sizeof(double));
	//double *outWght = load("neurones/nerons4.txt");
    double *outWght = malloc((nbHidNod * nbOut) * sizeof(double));

	//Initialize layers with random value close to 0
	for(int i = 0; i < nbIn; i++)
	{
		for(int j = 0; j < nbHidNod; j++)
		{
			hidWght[i * nbHidNod + j] = randfrom(-0.5,0.5);//(double)rand()/(double)RAND_MAX;
		}
	}

	for(int i = 0; i < nbHidNod; i++)
	{
		hidLayBias[i] = randfrom(-0.5,0.5);//(double)rand()/(double)RAND_MAX;
		for(int j = 0; j < nbOut; j++)
		{
			outWght[i * nbOut + j] = randfrom(-0.5,0.5);//(double)rand()/(double)RAND_MAX;

		}
	}

	for(int i = 0; i < nbOut; i++)
	{
		outLayBias[i] = randfrom(-0.5,0.5);//(double)rand()/(double)RAND_MAX;
	}

   	save(hidWght,1,nbIn*nbHidNod);
	free(hidWght);
	save(hidLay,2,nbHidNod);
	free(hidLay);
	save(hidLayBias,3,nbHidNod);
	free(hidLayBias);
	save(outWght,4,nbOut*nbHidNod);
	free(outWght);
	save(outLay,5,nbOut);
	free(outLay);
	save(outLayBias,6,nbOut);
	free(outLayBias);
}

//Proccess a prediction
int forward(double* input)
{
    double* hidwgt = load("neural_network/neurones/nerons1.txt");
    double* hidlay = load("neural_network/neurones/nerons2.txt");
    double* hidlayBias = load("neural_network/neurones/nerons3.txt");

    double* outwgt = load("neural_network/neurones/nerons4.txt");
    double* outlay = load("neural_network/neurones/nerons5.txt");
    double* outlayBias = load("neural_network/neurones/nerons6.txt");

    for(int j = 0; j < nbHidNod; j++)
    {
        double z = hidlayBias[j];
        for(int k = 0; k < nbIn; k++)
        {
            z +=  input[k] * hidwgt[k * nbHidNod + j];
        }
        hidlay[j] = sig(z);
    }

    for(int j = 0; j < nbOut; j++)
    {
        double z = outlayBias[j];
        for(int k = 0; k < nbHidNod; k++)
        {
            z += hidlay[k] * outwgt[k*nbOut + j];
        }
        outlay[j] = sig(z);
    }

   int res = get_maxi(outlay);
   printf("res = %d\n",res);
   free(hidwgt);
   free(hidlay);
   free(hidlayBias);
   free(outwgt);
   free(outlay);
   free(outlayBias);
   return res;
}

//Train the network with the database
void train_dataset()
{
    set_scratch();
    for(int i = 0; i < 50; i++)
    {
        printf("going for %deme iter\n",i+1);
        for(int j = 1; j <= nbData; j++)
        {
            char* p = malloc(sizeof(char));
            sprintf(p,"%d",j);
            proceed(100,p);
        }
        printf("done %d iter\n",i+1);
    }

}

//The final function that return the sudoku grid form the sperated cases
//in "inputs"
int* final_fun(double** inputs)
{
    int c = 0;
    int* res = malloc(81 * sizeof(int));
    for(int i = 0; i < 81 ; i++)
    {
        if(inputs[i] == NULL)
        {
            res[i] = 0;
            c++;
        }
        else
        {
            res[i] = forward(inputs[i]);
        }
    }
    printf("c = %d\n",c);
    return res;
}

/*
int main()
{
    train_dataset();
}
*/
