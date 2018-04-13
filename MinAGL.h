#ifndef MIN_AGL_H
#define MIN_AGL_H

//Apr 12, 2018//#include "stdafx.h"

class MinAGL
{
private:
	double m_UAV_MSL;
	double m_Terrain_MSL;
public:
	double m_minAGL;
	double m_actual_AGL;
	MinAGL()
		:m_UAV_MSL(-100000.0),
		m_Terrain_MSL(0.0),
		m_minAGL(165.0)
	{}
	MinAGL(double UAV_MSL, double Terrain_MSL, double minAGL)
	{
		m_UAV_MSL = UAV_MSL;
		m_Terrain_MSL = Terrain_MSL;
		m_minAGL = minAGL;
	}
	~MinAGL(){};

	void setMinAGL(double minAGL)
	{
		m_minAGL = minAGL;
	}
	bool MinAGL_Maintained(double, double);
	bool MinAGL_Maintained(double, double, double);

};
#endif
