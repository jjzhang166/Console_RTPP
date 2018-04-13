//Apr 12, 2018//#include "stdafx.h"

#include "MapCoordsFileGenerator.h"

void MapCoordsFileGenerator::Path_xyhCoord_inFeet_Save(
	List<DFCS_Coord>* DFCS_Coord_List, const char* xyhFileName )
{

	ofstream outClientFile( xyhFileName, ios::out );
	if ( !outClientFile ) {  // overloaded ! operator
		cerr << "Output file could not be opened" << endl;
		exit( 1 );    // prototype in cstdlib
	}
	outClientFile<<setiosflags(ios::showpoint)<<setprecision(16);

/////////////////////////////////////////
	DFCS_Coord DFCS_Coord_Buffer;
	ListNode< DFCS_Coord > *currentPtr;
	DFCS_Coord dataGetter;

	currentPtr = DFCS_Coord_List->getFirtstPtr();
	while ( currentPtr != 0 )
	{
		dataGetter = currentPtr->getData();
		DFCS_Coord_Buffer.index = dataGetter.index;
		DFCS_Coord_Buffer.x = dataGetter.x;
		DFCS_Coord_Buffer.y = dataGetter.y;
		DFCS_Coord_Buffer.h = dataGetter.h;

		outClientFile << DFCS_Coord_Buffer.index << '\t'
			<< DFCS_Coord_Buffer.x << '\t'
			<< DFCS_Coord_Buffer.y << '\t'
			<< DFCS_Coord_Buffer.h << '\n';

		currentPtr = DFCS_Coord_List->getNextPtr(currentPtr);
	}
//////////////////////////////////////////////////////

}


////
void MapCoordsFileGenerator::Map_xyhCoord_inFeet_Generator(	const double x_min,
								   const double x_max,
								   const double y_min,
								   const double y_max,
								   const int step,
								   const char* xyhFileName )
{
	DFCS_Coord DFCS_Coord_Buffer;
	Queue<DFCS_Coord> DFCS_Coord_Queue;
	int row_index = 0;
	int col_index = 0;
	unsigned long int coord_index=0;

	ofstream outClientFile( xyhFileName, ios::out );
	if ( !outClientFile ) {  // overloaded ! operator
		cerr << "Output file could not be opened" << endl;
		exit( 1 );    // prototype in cstdlib
	}
	outClientFile<<setiosflags(ios::showpoint)<<setprecision(16);


	//int col_min = (int)(x_min-1);
	//int col_max = (int)(x_max+1);
	//int row_min = (int)(y_min-1);
	//int row_max = (int)(y_max+1);
	int col_min = (int)(x_min);
	int col_max = (int)(x_max);
	int row_min = (int)(y_min);
	int row_max = (int)(y_max);

	for(int row=row_min;row<=row_max;(row+=step))
	{
		for(int col=col_min;col<=col_max;(col+=step))
		{
			++coord_index;
			DFCS_Coord_Buffer.index = coord_index;
			DFCS_Coord_Buffer.x = col;
			DFCS_Coord_Buffer.y = row;
			DFCS_Coord_Buffer.h = 0.0;

			DFCS_Coord_Queue.enqueue(DFCS_Coord_Buffer);
			++col_index;
			//cout<<"( "<< row <<", "<< col <<" )\t";

		}
		col_index = 0;
		++row_index;
		//cout<<"--Row Ends--"<<endl;
	}

	if(!(DFCS_Coord_Queue.isEmpty()))
	{
		DFCS_Coord_Queue.print();
	}

	while(!(DFCS_Coord_Queue.isEmpty()))
	{
		DFCS_Coord_Buffer.index = 0;
		DFCS_Coord_Buffer.x = 0.0;
		DFCS_Coord_Buffer.y = 0.0;
		DFCS_Coord_Buffer.h = 0.0;

		DFCS_Coord_Buffer = DFCS_Coord_Queue.dequeue();
		outClientFile << DFCS_Coord_Buffer.index << '\t'
			<< DFCS_Coord_Buffer.x << '\t'
			<< DFCS_Coord_Buffer.y << '\t'
			<< DFCS_Coord_Buffer.h << '\n';
	}

}

void MapCoordsFileGenerator::Map_xyhCoord_inFeet_Generator(	)
{
	//int col_min = (int)(m_xmin-1);
	//int col_max = (int)(m_xmax+1);
	//int row_min = (int)(m_ymin-1);
	//int row_max = (int)(m_ymax+1);
	int col_min = (int)(m_xmin);
	int col_max = (int)(m_xmax);
	int row_min = (int)(m_ymin);
	int row_max = (int)(m_ymax);
	int step = m_step;
	const char* xyhFileName = m_xyhFileName;

	DFCS_Coord DFCS_Coord_Buffer;
	Queue<DFCS_Coord> DFCS_Coord_Queue;
	int row_index = 0;
	int col_index = 0;
	unsigned long int coord_index=0;

	ofstream outClientFile( xyhFileName, ios::out );
	if ( !outClientFile ) {  // overloaded ! operator
		cerr << "Output file could not be opened" << endl;
		exit( 1 );    // prototype in cstdlib
	}
	outClientFile<<setiosflags(ios::showpoint)<<setprecision(16);



	for(int row=row_min;row<=row_max;(row+=step))
	{
		for(int col=col_min;col<=col_max;(col+=step))
		{
			++coord_index;
			DFCS_Coord_Buffer.index = coord_index;
			DFCS_Coord_Buffer.x = col;
			DFCS_Coord_Buffer.y = row;
			DFCS_Coord_Buffer.h = 0.0;

			DFCS_Coord_Queue.enqueue(DFCS_Coord_Buffer);
			++col_index;
			//cout<<"( "<< row <<", "<< col <<" )\t";

		}
		col_index = 0;
		++row_index;
		//cout<<"--Row Ends--"<<endl;
	}

	if(!(DFCS_Coord_Queue.isEmpty()))
	{
		DFCS_Coord_Queue.print();
	}

	while(!(DFCS_Coord_Queue.isEmpty()))
	{
		DFCS_Coord_Buffer.index = 0;
		DFCS_Coord_Buffer.x = 0.0;
		DFCS_Coord_Buffer.y = 0.0;
		DFCS_Coord_Buffer.h = 0.0;

		DFCS_Coord_Buffer = DFCS_Coord_Queue.dequeue();
		outClientFile << DFCS_Coord_Buffer.index << '\t'
			<< DFCS_Coord_Buffer.x << '\t'
			<< DFCS_Coord_Buffer.y << '\t'
			<< DFCS_Coord_Buffer.h << '\n';
	}

}
//////////////////////////////
//void MapCoordsFileGenerator::xyh_FileReader( List<DFCS_Coord> DFCS_Coord_List, const char* xyhFileName )
void MapCoordsFileGenerator::xyh_FileReader( List<DFCS_Coord>* DFCS_Coord_List, const char* xyhFileName, bool MinMaxStepFlag )
{
	DFCS_Coord DFCS_Coord_Buffer;	// For loading the list

	ifstream m_in_xyhFile( xyhFileName, ios::in );
	if ( !m_in_xyhFile ) {  // overloaded ! operator
		cerr << "Input file could not be opened" << endl;
		exit( 1 );    // prototype in cstdlib
	}

	while ( m_in_xyhFile	>> DFCS_Coord_Buffer.index
		>> DFCS_Coord_Buffer.x
		>> DFCS_Coord_Buffer.y
		>> DFCS_Coord_Buffer.h )
	{
		DFCS_Coord_List->insertAtBack(DFCS_Coord_Buffer);

		if( MinMaxStepFlag)
		{
			if( m_xmin > DFCS_Coord_Buffer.x )
				m_xmin = DFCS_Coord_Buffer.x;
			if( m_xmax < DFCS_Coord_Buffer.x )
				m_xmax = DFCS_Coord_Buffer.x;
			if( m_ymin > DFCS_Coord_Buffer.y )
				m_ymin = DFCS_Coord_Buffer.y;
			if( m_ymax < DFCS_Coord_Buffer.y )
				m_ymax = DFCS_Coord_Buffer.y;
		}

		DFCS_Coord_Buffer.index = 0;
		DFCS_Coord_Buffer.x = 0.0;
		DFCS_Coord_Buffer.y = 0.0;
		DFCS_Coord_Buffer.h = 0.0;
	}


	m_in_xyhFile.close();


	if( MinMaxStepFlag)
	{

		ListNode< DFCS_Coord >* ptrStep_0; 	// For computing the step;
		ListNode< DFCS_Coord >* ptrStep_1; 	// For computing the step;
		DFCS_Coord Step_0;
		DFCS_Coord Step_1;
		int x_step = 0;
		int y_step = 0;

		ptrStep_0 = DFCS_Coord_List->getFirtstPtr();
		ptrStep_1 = DFCS_Coord_List->getNextPtr( ptrStep_0 );

		Step_0 = ptrStep_0->getData();
		Step_1 = ptrStep_1->getData();

		if( (int)Step_0.x != (int)Step_1.x )	// Casting truncates the decimal; it doesn't round up or down it truncates
		{										// e.g. (int)9.99999 becomes 9 due to casting
			x_step = (int)abs( Step_0.x - Step_1.x );
			m_step = x_step;
		}
		else if ( (int)Step_0.y != (int)Step_1.y )
		{
			y_step = (int)abs( Step_0.y - Step_1.y );
			m_step = y_step;
		}
		else
		{
			//Apr 12, 2018//MessageBox(NULL, _T("Critical error in step distance calculation which may be caused by decimal round off."), _T("Step Distance Calculation Error"), MB_ICONEXCLAMATION);
			exit(0);
		}

#if DBG_EXTREMUM_MSGBOX
		char num_buffer[50];
		char msg_buffer[100];
		int r=10;

		_itoa_s( m_xmin, msg_buffer, r );
		strcat_s( msg_buffer, " = x_min, ");
		_itoa_s( m_xmax, num_buffer, r );
		strcat_s( num_buffer, " = x_max,\n ");
		strcat_s( msg_buffer, num_buffer);
		_itoa_s( m_ymin, num_buffer, r );
		strcat_s( num_buffer, " = y_min, ");
		strcat_s( msg_buffer, num_buffer);
		_itoa_s( m_ymax, num_buffer, r );
		strcat_s( num_buffer, " = y_max, \n");
		strcat_s( msg_buffer, num_buffer);
		_itoa_s( m_step, num_buffer, r );
		strcat_s( num_buffer, " = step, ");
		strcat_s( msg_buffer, num_buffer);

		//std::string sss = msg_buffer;
		/*
		const char* sss;
		strcpy_s(msg_buffer, sss);
		*/
		MessageBoxA(NULL, (LPCSTR)msg_buffer, "Extremum", MB_OK);

#endif
	}

}
