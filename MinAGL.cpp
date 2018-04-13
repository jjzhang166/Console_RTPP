//Apr 12, 2018//#include "stdafx.h"

#include <iostream>
using std::cout;
using std::cin;
using std::endl;


#include "MinAGL.h"

bool MinAGL::MinAGL_Maintained(double UAV_MSL, double Terrain_MSL)
{
	double actual_AGL = UAV_MSL - Terrain_MSL;
	m_actual_AGL = actual_AGL;

	if(actual_AGL >= m_minAGL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MinAGL::MinAGL_Maintained(double UAV_MSL, double Terrain_MSL, double minAGL)
{
	double actual_AGL = UAV_MSL - Terrain_MSL;
	m_actual_AGL = actual_AGL;


	if(actual_AGL >= minAGL)
	{
		return true;
	}
	else
	{
		return false;
	}
}
