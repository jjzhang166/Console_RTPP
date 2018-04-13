//Apr 12, 2018//#include "stdafx.h"

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <cassert>

#include "SmartArray.h"

#include "MessageBox_DebugDefines.h"

#include <windows.h>

SmartArray::~SmartArray()
{
	if(m_Array != NULL)
	{
		for ( int i = 0; i < del_x; ++i )
		{
			free (m_Array[ i ]);	// Memory allcoation for the rows
		}
		free(m_Array);
#if DBG_MAP_MEMORY_MSGBOX
		MessageBoxA(NULL, "The memory used by the map has been freed.", "Freeing Array Memory", MB_OK );
#endif
	}
	else
	{
#if DBG_MAP_MEMORY_MSGBOX
		MessageBoxA(NULL, "No memory was allocated to be used by the map.", "No Memory Freed", MB_OK );
#endif
	}


}

void SmartArray::Map_xyhCoord_inFeet_Generator(	const double x_min,
								   const double x_max,
								   const double y_min,
								   const double y_max,
								   const int step_arg )
{
	m_xmax = x_max;
	m_xmin = x_min;
	m_ymax = y_max;
	m_ymin = y_min;
	m_step = step_arg;
	//del_x = (int)m_xmax - (int)m_xmin + 1;
	//del_y = (int)m_ymax - (int)m_ymin + 1;
	del_x = (int)(((int)m_xmax - (int)m_xmin)/m_step) + 1;
	del_y = (int)(((int)m_ymax - (int)m_ymin)/m_step) + 1;

	int min_x = (int)m_xmin;
	int min_y = (int)m_ymin;
	int max_x = (int)m_xmax;
	int max_y = (int)m_ymax;
	int step = m_step;



	m_Array = (DFCS_Coord **)calloc( sizeof( DFCS_Coord * ), del_x );	// Allocating memory for the columns
	for ( int i = 0; i < del_x; ++i )
	{
		m_Array[ i ] = (DFCS_Coord *)calloc( sizeof( DFCS_Coord ), del_y );	// Memory allcoation for the rows
	}

	int J=max_y;
	int I=0;
	for(int j = (del_y-1); j >=0 ; j-=step)
	{
		I=min_x;
		for(int i = 0; i < del_x; i+=step)
		{
			m_Array[i][j].x = I;
			m_Array[i][j].y = J;
			cout<<"["<<m_Array[i][j].x<<"]["<< m_Array[i][j].y << "]["<< m_Array[i][j].h << "]"<<"   ";
			I+=step;
		}
		cout<<endl;
		J-=step;
	}

}


void SmartArray::Map_xyhCoord_inFeet_Generator(	)
{

	int min_x = (int)m_xmin;
	int min_y = (int)m_ymin;
	int max_x = (int)m_xmax;
	int max_y = (int)m_ymax;
	int step = m_step;


	m_Array = (DFCS_Coord **)calloc( sizeof( DFCS_Coord * ), del_x );	// Allocating memory for the columns
	for ( int i = 0; i < del_x; ++i )
	{
		m_Array[ i ] = (DFCS_Coord *)calloc( sizeof( DFCS_Coord ), del_y );	// Memory allcoation for the rows
	}

	int J=max_y;
	int I=0;
	for(int j = (del_y-1); j >=0 ; j-=step)
	{
		I=min_x;
		for(int i = 0; i < del_x; i+=step)
		{
			m_Array[i][j].x = I;
			m_Array[i][j].y = J;
			cout<<"["<<m_Array[i][j].x<<"]["<< m_Array[i][j].y << "]["<< m_Array[i][j].h << "]"<<"   ";
			I+=step;
		}
		cout<<endl;
		J-=step;
	}

}

DFCS_Coord* SmartArray::QueryArrayUsingWorld_Coord(int x, int y)
{
	int i = 0;
	int j = 0;
	double Numerator = 0;
	double Denominator = 0;
	double ArrayIndex = 0;

	if(x >= m_xmin && x <= m_xmax && y >= m_ymin && y <= m_ymax)
	{
		Numerator = (double)x + m_xmax - (double)m_step/2;
		Denominator = (double)m_step;
		ArrayIndex = Numerator/Denominator + 1.0;

		i = (int) ArrayIndex;

		Numerator = 0;
		Denominator = 0;
		ArrayIndex = 0;
		Numerator = (double)y + m_ymax - (double)m_step/2;
		Denominator = (double)m_step;
		ArrayIndex = Numerator/Denominator + 1.0;

		j = (int) ArrayIndex;

		if(i<0 || j<0); //Apr 12, 2018//
			//Apr 12, 2018//MessageBox(NULL, _T("Dynamic Array Index Error in SmartArray.cpp\nThis needs fixing."), _T("Array Index Error"), MB_ICONEXCLAMATION);

		return &m_Array[i][j];
	}
	else
	{
		//Apr 12, 2018//MessageBox(NULL, _T("Array Bounds Error"), _T("Array Bounds Error"), MB_ICONEXCLAMATION);
		return NULL;
	}

}

void SmartArray::SetHeightUsingWorld_Coord(	unsigned long int index, int x, int y, double h)
{

	int i = 0;
	int j = 0;
	double Numerator = 0;
	double Denominator = 0;
	double ArrayIndex = 0;

	if(x >= m_xmin && x <= m_xmax && y >= m_ymin && y <= m_ymax)
	{
		Numerator = (double)x + m_xmax - (double)m_step/2;
		Denominator = (double)m_step;
		ArrayIndex = Numerator/Denominator + 1.0;

		i = (int) ArrayIndex;

		Numerator = 0;
		Denominator = 0;
		ArrayIndex = 0;
		Numerator = (double)y + m_ymax - (double)m_step/2;
		Denominator = (double)m_step;
		ArrayIndex = Numerator/Denominator + 1.0;

		j = (int) ArrayIndex;

		if(i<0 || j<0); //Apr 12, 2018//
			//Apr 12, 2018//MessageBox(NULL, _T("Dynamic Array Index Error in SmartArray.cpp\nThis needs fixing."), _T("Array Index Error"), MB_ICONEXCLAMATION);

		*(&m_Array[i][j].x) = x;
		*(&m_Array[i][j].y) = y;
		*(&m_Array[i][j].h) = h;
		*(&m_Array[i][j].index) = index;

	}
	else
	{
		//Apr 12, 2018//MessageBox(NULL, _T("Height was not set due to array bounds error."), _T("Heights Not Set Error"), MB_ICONEXCLAMATION);
	}

}

void SmartArray::SetHeightUsingList(List<DFCS_Coord>* xyhHeightsInFtList)
{
#if 1
	int cnt=0;
	ListNode< DFCS_Coord > *currentPtr;
	DFCS_Coord dataGetter;
	unsigned long int index;
	int x;
	int y;
	double h;

	currentPtr = xyhHeightsInFtList->getFirtstPtr();
	while ( currentPtr != 0 )
	{
		dataGetter = currentPtr->getData();
		++cnt;
		index = dataGetter.index;
		x = (int)dataGetter.x;
		y = (int)dataGetter.y;
		h = (double)dataGetter.h;

		SetHeightUsingWorld_Coord( index, x, y, h );

		currentPtr = xyhHeightsInFtList->getNextPtr(currentPtr);
	}

#if DBG_TOTALGRIDPOINTS_MSGBOX
	char nodes_buffer[65];
	int r=10;

	_itoa_s( cnt, nodes_buffer, r );
	strcat_s( nodes_buffer, " populated height points in list., ");
	MessageBoxA(NULL, (LPCSTR)nodes_buffer, "Total Grid Points", MB_OK);
#endif

#endif


}

int SmartArray::get_Array_Y_size(void)
{
	return del_y;
}

int SmartArray::get_Array_X_size(void)
{
	return del_x;
}

void SmartArray::ClearSmartArray(void)
{
	if(m_Array != NULL)
	{
		for ( int i = 0; i < del_x; ++i )
		{
			free (m_Array[ i ]);	// Memory allcoation for the rows
		}
		free(m_Array);
#if DBG_MAP_MEMORY_MSGBOX
		MessageBox(NULL, _T("The memory used by the map has been freed."), _T("Freeing Array Memory"), MB_OK );
#endif
	}
	else
	{
#if DBG_MAP_MEMORY_MSGBOX
		MessageBox(NULL, _T("No memory was allocated to be used by the map."), _T("No Memory Freed"), MB_OK );
#endif
	}

}
