#ifndef FLIGHTPATTERNGENERATOR_H
#define FLIGHTPATTERNGENERATOR_H


#include <windows.h>
#include <math.h>

#include <iostream>
using std::ios;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <iomanip>
using std::setiosflags;
using std::resetiosflags;
using std::setw;
using std::setprecision;


#include "DFCS_ClassAndStructData.h"
#include "list.h"

using std::endl;

const double pi = 3.1415926535897932384626433832795;
const double RadPerDeg = (pi/180.0);
const double DegPerRad = (180.0/pi);
const int dfp_straight = 0;
const int dfp_clock_wise = -1;
const int dfp_counter_clock_wise = 1;



class FlightPatternGenerator
{

private:
	int m_step;
	double m_droneVelocity;	// To be used later for automating the computation of the step
	double m_turnRadius;	// Should equal m_step for now
	double m_droneMSL;		// For the flight Patterns altitude
	double m_droneHeading;		// For the flight Patterns altitude

public:
	FlightPatternGenerator():
		m_droneVelocity(0.0),
		m_turnRadius(0.0), m_droneMSL(-100000.0),	m_step(-1){}

	FlightPatternGenerator(
		double droneVelocity, double turnRadius,
		double droneMSL, int step )
	{
		m_droneVelocity = droneVelocity;
		m_turnRadius = turnRadius;
		m_droneMSL = droneMSL;
		m_step = step;
	}

	~FlightPatternGenerator()
	{
		//MessageBox(NULL,"Message from destructor\n\n","FlightPatternGenerator Destroyed", MB_OK);
	}

	void ResetProperties()
	{
		m_droneVelocity = (0.0);
		m_turnRadius = (0.0);
		m_droneMSL = (-100000.0);
		m_step = (-1);
	}
	void SetProperties(	double rise, double run,
		double droneVelocity, double turnRadius,
		double droneMSL, int step )
	{
		m_droneVelocity = droneVelocity;
		m_turnRadius = turnRadius;
		m_droneMSL = droneMSL;
		m_step = step;
	}

	void GetPathListFromPathFile(const char*, List<DFCS_Coord>*);
	void GetFPatListFromFPatFile(const char*, List<DFCS_FPatSegment>*);

	double getHSI_Angle(DFCS_Coord Start,  DFCS_Coord End);

	void GetVectorListFromPathList(List<DFCS_Coord>*, List<DFCS_Vector>*);
	void GetVectorListFromPathList(List<DFCS_Coord>*, List<DFCS_Vector>*, const char* );
	void GetWaypointListFromVectorList(List<DFCS_Vector>*, List<DFCS_Vector>*);
	void GetWaypointListFromVectorList(List<DFCS_Vector>*, List<DFCS_Vector>*, const char*);

	void GetFPatListFromWaypointList(List<DFCS_Vector>*, List<DFCS_FPatSegment>*);
	void GetFPatFileFromFPatList(List<DFCS_FPatSegment>*, const char*);

	void SetDroneVelocity(double droneVelocity);
	void SetStepDistance(double stepDistance);
	void SetTurnRadius(double turnRadius);
	void SetDroneMSL(double droneMSL);
	int GetInvalidTilesListFromVectorList(List<DFCS_Vector>* vecList, List<DFCS_Coord>* invalidTilesList, DFCS_Coord Start_Door, DFCS_Coord Goal_Door);
	int GetInvalidTilesListFromPathList(List<DFCS_Coord>* pthList, List<DFCS_Coord>* invalidTilesList, double UAV_InitialHeading);
};

#endif