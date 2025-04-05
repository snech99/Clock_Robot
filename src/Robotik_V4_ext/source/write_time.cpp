/*
 * write_time.cpp
 *
 *  Created on: 23.07.2024
 *      Author: gerri
 */
#include "Robotik.h"

uint32_t MAX_FREQU_STICK = 2500;
uint32_t MAX_FREQU_AXIS = 500;
uint32_t MAX_FREQU_TOWER = 2000;

void move_and_save(float *temp_point, float *new_point)
{
	sync_interpol(temp_point, new_point);

	temp_point[0] = new_point[0];
	temp_point[1] = new_point[1];
	temp_point[2] = new_point[2];
}

void draw_number(uint8_t num, float *point_start)
{
	float new_point[3];

	new_point[0] = point_start[0];
	new_point[1] = point_start[1];
	new_point[2] = point_start[2];

	float temp_point[3];

	temp_point[0] = point_start[0];
	temp_point[1] = point_start[1];
	temp_point[2] = point_start[2];

	switch(num)
	{
		case 0:
			new_point[0] = temp_point[0]+19;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+1;
			new_point[2] = temp_point[2]-80;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[1] = temp_point[1]+35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[1] = temp_point[1]+2;
			new_point[2] = temp_point[2]+82;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);
			break;

		case 1:

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+20;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]-80;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);

			break;

		case 2:

			new_point[0] = temp_point[0]+20;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]-40;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[1] = temp_point[1]+35;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]-40;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);

			break;

		case 3:

			new_point[0] = temp_point[0]+20;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[2] = temp_point[2]-80;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[1] = temp_point[1]+35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]+42;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+20;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-34;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);

			break;

		case 4:

			new_point[0] = temp_point[0]+19;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]-40;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]+40;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+22;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[2] = temp_point[2]-80;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);

			break;

		case 5:

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+20;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]+35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[2] = temp_point[2]-40;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]-40;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]+35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);

			break;

		case 6:

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+21;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]+35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[2] = temp_point[2]-80;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+1;
			new_point[2] = temp_point[2]+42;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]+35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);

			break;

		case 7:

			new_point[0] = temp_point[0]+20;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[2] = temp_point[2]-80;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);

			break;

		case 8:

			new_point[0] = temp_point[0]+19;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[2] = temp_point[2]-80;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+1;
			new_point[1] = temp_point[1]+35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+1;
			new_point[1] = temp_point[1]+2;
			new_point[2] = temp_point[2]+81;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]-38;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+21;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-34;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);
			break;

		case 9:

			new_point[0] = temp_point[0]+20;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+2;
			new_point[2] = temp_point[2]-80;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+1;
			new_point[1] = temp_point[1]+35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]+80;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]+21;
			move_and_save(temp_point, new_point);

			new_point[2] = temp_point[2]-38;
			move_and_save(temp_point, new_point);

			new_point[1] = temp_point[1]-35;
			move_and_save(temp_point, new_point);

			new_point[0] = temp_point[0]-20;
			move_and_save(temp_point, new_point);

			move_sync_p_to_p (new_point, REF_POINT, true);
			break;
	}
}

void init_number_start_points()
{
    START_POINT_1[0] = START_X_1;
    START_POINT_1[1] = START_Y_1;
    START_POINT_1[2] = START_Z_1;

    START_POINT_2[0] = START_X_2;
    START_POINT_2[1] = START_Y_2;
    START_POINT_2[2] = START_Z_2;

    START_POINT_3[0] = START_X_3;
    START_POINT_3[1] = START_Y_3;
    START_POINT_3[2] = START_Z_3;

    START_POINT_4[0] = START_X_4;
    START_POINT_4[1] = START_Y_4;
    START_POINT_4[2] = START_Z_4;

    REF_POINT[0] = REF_X;
    REF_POINT[1] = REF_Y;
    REF_POINT[2] = REF_Z;
}

void move_to_start_numb(uint8_t num)
{
	MAX_FREQU_STICK = 6000;
	MAX_FREQU_AXIS = 1300;
	MAX_FREQU_TOWER = 5000;

	float p_b[3] = {P_BEGIN_X,P_BEGIN_Y,P_BEGIN_Z};
	move_sync_p_to_p (p_b, REF_POINT, true);

	MAX_FREQU_STICK = 3500;
	MAX_FREQU_AXIS = 700;
	MAX_FREQU_TOWER = 3000;

	switch(num)
	{
		case 1:
			move_sync_p_to_p (REF_POINT, START_POINT_1, true);
			break;
		case 2:
			move_sync_p_to_p (REF_POINT, START_POINT_2, true);
			break;
		case 3:
			move_sync_p_to_p (REF_POINT, START_POINT_3, true);
			break;
		case 4:
			move_sync_p_to_p (REF_POINT, START_POINT_4, true);
			break;
	}

	MAX_FREQU_STICK = 2500;
	MAX_FREQU_AXIS = 500;
	MAX_FREQU_TOWER = 2000;
}

void move_to_end(uint8_t num)
{
	MAX_FREQU_STICK = 6000;
	MAX_FREQU_AXIS = 1300;
	MAX_FREQU_TOWER = 5000;

	float min_befor_end_p[3] ={35, 120, 650};
	move_sync_p_to_p (REF_POINT,min_befor_end_p, true);

	MAX_FREQU_STICK = 2500;
	MAX_FREQU_AXIS = 500;
	MAX_FREQU_TOWER = 2000;

	do_homing(false);
}




