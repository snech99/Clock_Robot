/*
*   Gerrit Hinrichs 06.2024
*   github.com/snech99
*
*   Clock_Robot
*   calc of the kinetics
*/

#include "Robotik.h"

void inverse (float, float, float, float *);

// calc of the velocity
void V_Profil(float *P_start, float* P_end)
{
	float t_array[101];

	t_array[0] = 0;

	for(int i=1; i<=100; i++)
	{
		t_array[i] = t_array[i-1]+0.01;
	}

	float inter_points[101][3] = {0};

    for(int i=0; i<=100; i++)
	{
		for(int k=0; k<3; k++)
		{
			inter_points[i][k] = (1-t_array[i])*P_start[k]+t_array[i]*P_end[k];
		}
	}

	float erg[3] = {};
	inverse(inter_points[0][0], inter_points[0][1], inter_points[0][2], erg);

	float delt[100][3] = {};

	float t1_old = erg[0];
	float t2_old = erg[1];
	float d4_old = erg[2];

	for(int i=1; i<=100; i++)
	{
		inverse(inter_points[i][0], inter_points[i][1], inter_points[i][2], erg);
		delt[i-1][0] = fabs(erg[0]-t1_old);
		delt[i-1][1] = fabs(erg[1]-t2_old);
		delt[i-1][2] = fabs(erg[2]-d4_old);

		t1_old = erg[0];
		t2_old = erg[1];
		d4_old = erg[2];
	}
}

// calc of the invers-kinetics
void inverse (float x, float y, float z, float *erg)
{
	int d1 = 237;
	int d2 = 108;
	int d3 = 20;

	//Theta_1
	float theta_1_1 = atan2(y,x);
	float s = sqrt(pow(x,2)+ pow(y,2));

	float cos_t1_2 = (float)d2/s;
	float sin_t1_2 = sqrt(1-pow(cos_t1_2,2));
	float theta_1 = theta_1_1 + atan2(sin_t1_2, cos_t1_2);

	//Hypothenuse fuer d3/d4
	float KS2_x = cos(theta_1)*d2;
	float KS2_y = sin(theta_1)*d2;
	float KS2_z = d1;

	float h = sqrt(pow((KS2_x-x),2)+pow((KS2_y-y),2)+pow((KS2_z-z),2));

	//d4
	float d4 = sqrt(pow(h,2)-pow(d3,2));

	//Theta_2
	float theta_2;
	if(z <= d1)
	{
		theta_2 = asin(fabs((KS2_z-z))/h) - atan2(d3, d4);
	}
	else
	{
		theta_2 = - asin(fabs((KS2_z-z))/h) - atan2(d3, d4);
	}

	erg[0] = theta_1 - M_PI_2;
	erg[1] = theta_2;
	erg[2] = d4;
}


