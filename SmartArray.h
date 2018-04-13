#ifndef SMARTARRAY_H
#define SMARTARRAY_H

//Apr 12, 2018//#include "stdafx.h"

#include "List.h"
#include "DFCS_ClassAndStructData.h"

class SmartArray
{
private:
	int m_step;
	int del_x;
	int del_y;
	double m_xmax;
	double m_xmin;
	double m_ymax;
	double m_ymin;
public:
	DFCS_Coord **m_Array;

public:
	~SmartArray();
	SmartArray():m_xmax(0.0),m_xmin(0.0), m_ymax(0.0), m_ymin(0.0),
	m_step(0), del_x(0), del_y(0){}
	SmartArray(	double x_min, double x_max,
		double y_min, double y_max,
		int step )
	{
		m_xmax = x_max;
		m_xmin = x_min;
		m_ymax = y_max;
		m_ymin = y_min;
		m_step = step;
		del_x = (int)(((int)m_xmax - (int)m_xmin)/m_step) + 1;
		del_y = (int)(((int)m_ymax - (int)m_ymin)/m_step) + 1;
	}

	void Map_xyhCoord_inFeet_Generator(	);
	void Map_xyhCoord_inFeet_Generator(	const double, const double, const double,
								   const double, const int);
	DFCS_Coord* QueryArrayUsingWorld_Coord(int x, int y);
	void SetHeightUsingWorld_Coord(unsigned long int index, int x, int y, double h);
	void SetHeightUsingList(List<DFCS_Coord>* xyhHeightsInFtList);
	int get_Array_Y_size(void);
	int get_Array_X_size(void);
	void ClearSmartArray(void);
};


#endif

