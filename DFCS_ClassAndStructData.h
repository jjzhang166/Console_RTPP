#ifndef DFCS_CLASSANDSTRUCTDATA_H
#define DFCS_CLASSANDSTRUCTDATA_H

struct DFCS_Coord
{
	unsigned long int index;
	double x;
	double y;
	double h;
};


struct DFCS_Vector
{
	DFCS_Coord Start;
	DFCS_Coord End;
	double Angle;
};

struct DFCS_FPatSegment
{
	int segment;
	int nxtSegment;
	int dfp;
	int n_spare[2];
	double hnfp;
	double xnfp;
	double x0fp;
	double ynfp;
	double y0fp;
	double f_spare[4];
};

#endif