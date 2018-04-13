// DFCSCoords_for_AStarMap_Queue.cpp : Defines the entry point for the console application.
//
#ifndef MAPCOORDSFILEGENERATOR_H
#define MAPCOORDSFILEGENERATOR_H

//Apr 12, 2018//#include "stdafx.h"

#include <iostream>
using std::cout;
using std::cin;
using std::ios;
using std::cerr;
using std::endl;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <iomanip>
using std::setiosflags;
using std::resetiosflags;
using std::setw;
using std::setprecision;

#include <cstdlib>

#include "DFCS_ClassAndStructData.h"
#include "queue.h"
#include "listnd.h"
#include "list.h"

const double approx_DFCS_y_max = 270157.97;
const double approx_DFCS_y_min = -249663.92;
const double approx_DFCS_x_max = 67863.34;
const double approx_DFCS_x_min = -127871.35;
const double ft_per_meter = 3.280839895014;

const double DTED_ResolutionInFeet = 30*ft_per_meter;
const double ZoomOutFactor = 5.0;
const int StepDistanceInFeet = (int)(DTED_ResolutionInFeet*ZoomOutFactor);

class MapCoordsFileGenerator
{
private:
	const char* m_xyhFileName;

public:
	int m_step;
	double m_xmax;
	double m_xmin;
	double m_ymax;
	double m_ymin;
	void Path_xyhCoord_inFeet_Save(	List<DFCS_Coord>*, const char* );
	void xyh_FileReader(List<DFCS_Coord>*, const char*, bool);
	MapCoordsFileGenerator():m_xmax(0.0),m_xmin(0.0), m_ymax(0.0), m_ymin(0.0),
	m_step(0), m_xyhFileName(0){}
	MapCoordsFileGenerator(	double x_min, double x_max,
		double y_min, double y_max,
		int step, const char* xyhFileName )
	{
		m_xmax = x_max;
		m_xmin = x_min;
		m_ymax = y_max;
		m_ymin = y_min;
		m_step = step;
		m_xyhFileName = xyhFileName;

	}
	void Reset()
	{
		m_xmax = (0.0);
		m_xmin = (0.0);
		m_ymax = (0.0);
		m_ymin = (0.0);
		m_step = (0);
		//m_xyhFileName = (0);
	}
	void Map_xyhCoord_inFeet_Generator(	);
	void Map_xyhCoord_inFeet_Generator(	const double, const double, const double,
								   const double, const int, const char*);
};


#endif

