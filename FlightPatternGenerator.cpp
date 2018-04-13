
#include "FlightPatternGenerator.h"
#include ".\flightpatterngenerator.h"

double FlightPatternGenerator::getHSI_Angle(DFCS_Coord Start,  DFCS_Coord End)
{
	if(Start.x == End.x && Start.y == End.y)	// Values entered aret the same point
		return -1;

	double retAngle = -1;


	double xDisplacement = 0;
	double yDisplacement = 0;

	xDisplacement = Start.x - End.x;
	yDisplacement = Start.y - End.y;

	if(xDisplacement == 0)
	{
		if(End.y > Start.y)
		{
			retAngle = 0;
			return retAngle;
		}
		if(End.y < Start.y)
		{
			retAngle = 180;
			return retAngle;
		}
	}

	if(yDisplacement == 0)
	{
		if(End.x > Start.x)
		{
			retAngle = 90;
			return retAngle;
		}
		if(End.x < Start.x)
		{
			retAngle = 270;
			return retAngle;
		}
	}

	if(xDisplacement != 0 && yDisplacement != 0)
	{
		double QuadrantOffsetAngle = 0;

		//Quadrant I
		if(Start.x < End.x && Start.y < End.y)
		{
			QuadrantOffsetAngle = 0;
			retAngle = QuadrantOffsetAngle + atan(abs(xDisplacement/yDisplacement))*180/pi;
		}
		//Quadrant II
		if(Start.x < End.x && Start.y > End.y)
		{
			QuadrantOffsetAngle = 90;
			retAngle = QuadrantOffsetAngle + atan(abs(yDisplacement/xDisplacement))*180/pi;
		}
		//Quadrant III
		if(Start.x > End.x && Start.y > End.y)
		{
			QuadrantOffsetAngle = 180;
			retAngle = QuadrantOffsetAngle + atan(abs(xDisplacement/yDisplacement))*180/pi;
		}
		//Quadrant IV
		if(Start.x > End.x && Start.y < End.y)
		{
			//QuadrantOffsetAngle = 90;
			QuadrantOffsetAngle = 270;
			retAngle = QuadrantOffsetAngle + atan(abs(yDisplacement/xDisplacement))*180/pi;
		}

		return retAngle;
	}

	return -1;
}

// Constructs the pathList from a .uav.pth file
void FlightPatternGenerator::GetPathListFromPathFile(const char* pthFileName, List<DFCS_Coord>* pthList)
{
	unsigned long int in_coord_index = 0;
	double in_X = 0;
	double in_Y = 0;
	double in_Z = 0;

	DFCS_Coord in_Data;

	ifstream input_pthFile( pthFileName, ios::in );
	if ( !input_pthFile )
	{
		exit( 1 );
	}
	else
		cout<<"\n File: "<< pthFileName <<" Opened\n";

	cout << "Reading DFCS Coordinates onto pthList" << endl;

	while ( input_pthFile >> in_coord_index >> in_X >> in_Y >> in_Z )
	{
		in_Data.index = in_coord_index;
		in_Data.x = in_X;
		in_Data.y = in_Y;
		in_Data.h = in_Z;

		pthList->insertAtFront( in_Data );   // put items in the queue
	}

	input_pthFile.close();
	cout << endl;
}

void FlightPatternGenerator::GetFPatListFromFPatFile(const char* fpatFileName, List<DFCS_FPatSegment>* fpatList)
{
	int in_segment;
	int in_nxtSegment;
	int in_dfp;
	double in_hnfp;
	double in_xnfp;
	double in_x0fp;
	double in_ynfp;
	double in_y0fp;

	DFCS_FPatSegment in_Data;

	ifstream input_fpatFile( fpatFileName, ios::in );
	if ( !input_fpatFile )
	{
		exit( 1 );
	}
	else
		cout<<"\n File: "<< fpatFileName <<" Opened\n";

	cout << "Reading DFCS Coordinates onto pthList" << endl;

	while ( input_fpatFile >> in_segment >> in_nxtSegment >> in_dfp >> in_hnfp >> in_xnfp >> in_x0fp >> in_ynfp >> in_y0fp )
	{
		in_Data.segment = in_segment;
		in_Data.nxtSegment = in_nxtSegment;
		in_Data.dfp = in_dfp;
		in_Data.hnfp = in_hnfp;
		in_Data.xnfp = in_xnfp;
		in_Data.x0fp = in_x0fp;
		in_Data.ynfp = in_ynfp;
		in_Data.ynfp = in_y0fp;

		fpatList->insertAtFront( in_Data );   // put items in the queue
	}

	input_fpatFile.close();
	cout << endl;
}

// This function was modified so that the pthList  would not be modified in any  way.
void FlightPatternGenerator::GetVectorListFromPathList(List<DFCS_Coord>* pthList, List<DFCS_Vector>* vecList)
{

//////////////////////////////////////////////////////////
	List<DFCS_Coord> cpy_pthList;
	ListNode< DFCS_Coord > *cpy_currentPtr;
	DFCS_Coord cpy_dataGetter;
	cpy_currentPtr = pthList->getFirtstPtr();
	while ( cpy_currentPtr != 0 )
	{
		cpy_dataGetter = cpy_currentPtr->getData();
		cpy_pthList.insertAtFront(cpy_dataGetter);
		//cpy_pthList.insertAtBack(cpy_dataGetter);
		cpy_currentPtr = pthList->getNextPtr(cpy_currentPtr);
	}
//////////////////////////////////////////////////////////

	unsigned long int in_coord_index = 0;
	double in_X = 0;
	double in_Y = 0;
	double in_Z = 0;

	unsigned long int out_coord_index = 0;
	double out_X = 0;
	double out_Y = 0;
	double out_Z = 0;

	ListNode< DFCS_Coord > *currentPtr;
	DFCS_Coord dataGetter;
	DFCS_Coord Coord1;
	DFCS_Coord Coord2;

	cout << endl << "Dequeing DFCS Coordinates from cpy_pthList" << endl;

	DFCS_Vector myVector;
	double in_X1 = 0;
	double in_Y1 = 0;
	double in_X2 = 0;
	double in_Y2 = 0;
	double in_Angle = -1;
	DFCS_Vector in_Data;

	double angle = -1;

	while ( !cpy_pthList.isEmpty() )
	{
		currentPtr = cpy_pthList.getFirtstPtr();
		dataGetter = currentPtr->getData();
		Coord1 = dataGetter;

		cpy_pthList.removeFromFront(dataGetter);
			if( cpy_pthList.isEmpty() )
				break;

		currentPtr = cpy_pthList.getFirtstPtr();
		dataGetter = currentPtr->getData();
		Coord2 = dataGetter;

		angle = getHSI_Angle(Coord1,  Coord2);

		cout << Coord1.index << '\t' << Coord2.index << '\t' << angle << '\n';

		myVector.Start.index = 0;
		myVector.Start.h = -1000;
		myVector.Start.x = Coord1.x ;
		myVector.Start.y = Coord1.y ;
		myVector.End.index = 0;
		myVector.End.h = -1000;
		myVector.End.x = Coord2.x ;
		myVector.End.y = Coord2.y ;
		myVector.Angle = angle;

		in_Data = myVector;

		vecList->insertAtBack( in_Data );   // put items in the queue

	}

	cout << endl;
}

// This function was modified so that the pthList  would not be modified in any  way.
void FlightPatternGenerator::GetVectorListFromPathList(List<DFCS_Coord>* pthList, List<DFCS_Vector>* vecList, const char* vecFileName)
{

//////////////////////////////////////////////////////////
	List<DFCS_Coord> cpy_pthList;
	ListNode< DFCS_Coord > *cpy_currentPtr;
	DFCS_Coord cpy_dataGetter;
	cpy_currentPtr = pthList->getFirtstPtr();
	while ( cpy_currentPtr != 0 )
	{
		cpy_dataGetter = cpy_currentPtr->getData();
		//cpy_pthList.insertAtBack(cpy_dataGetter);
		cpy_pthList.insertAtFront(cpy_dataGetter);
		cpy_currentPtr = pthList->getNextPtr(cpy_currentPtr);
	}
//////////////////////////////////////////////////////////

	unsigned long int in_coord_index = 0;
	double in_X = 0;
	double in_Y = 0;
	double in_Z = 0;

	unsigned long int out_coord_index = 0;
	double out_X = 0;
	double out_Y = 0;
	double out_Z = 0;

	ListNode< DFCS_Coord > *currentPtr;
	DFCS_Coord dataGetter;
	DFCS_Coord Coord1;
	DFCS_Coord Coord2;

	ofstream output_vecFile( vecFileName, ios::out );
	if ( !output_vecFile)
	{
		exit( 1 );
	}
	else
		cout<<"\n File: "<< vecFileName <<" Created\n";

	output_vecFile<<setiosflags(ios::showpoint)<<setprecision(20);

	cout << endl << "Dequeing DFCS Coordinates from cpy_pthList" << endl;

	DFCS_Vector myVector;
	double in_X1 = 0;
	double in_Y1 = 0;
	double in_X2 = 0;
	double in_Y2 = 0;
	double in_Angle = -1;
	DFCS_Vector in_Data;

	double angle = -1;

	while ( !cpy_pthList.isEmpty() )
	{
		currentPtr = cpy_pthList.getFirtstPtr();
		dataGetter = currentPtr->getData();
		Coord1 = dataGetter;

		cpy_pthList.removeFromFront(dataGetter);
			if( cpy_pthList.isEmpty() )
				break;

		currentPtr = cpy_pthList.getFirtstPtr();
		dataGetter = currentPtr->getData();
		Coord2 = dataGetter;

		angle = getHSI_Angle(Coord1,  Coord2);

		cout << Coord1.index << '\t' << Coord2.index << '\t' << angle << '\n';

		myVector.Start.index = 0;
		myVector.Start.h = -1000;
		myVector.Start.x = Coord1.x ;
		myVector.Start.y = Coord1.y ;
		myVector.End.index = 0;
		myVector.End.h = -1000;
		myVector.End.x = Coord2.x ;
		myVector.End.y = Coord2.y ;
		myVector.Angle = angle;

		in_Data = myVector;

		vecList->insertAtBack( in_Data );   // put items in the queue
		output_vecFile << Coord1.x << '\t' << Coord1.y << '\t' << Coord2.x << '\t' << Coord2.y << '\t' << angle << '\n';

	}

	output_vecFile.close();
	cout << endl;
}

void FlightPatternGenerator::GetWaypointListFromVectorList(List<DFCS_Vector>* vecList, List<DFCS_Vector>* segList)
{
	unsigned long int in_coord_index = 0;
	double in_X1 = 0;
	double in_Y1 = 0;
	double in_X2 = 0;
	double in_Y2 = 0;
	double in_Angle = -1;


	cout << endl << "Dequeing DFCS Coordinates from vecList" << endl;

	DFCS_Vector dataGetter;		// Temporary buffer
	DFCS_Vector fp_Segment;		// Temporary buffer
	List<DFCS_Vector> fp_Segments_List;
	//List<DFCS_FltPat> mySegmentsList;
	ListNode< DFCS_Vector > *currentPtr;	// Temporary buffer, I think

	DFCS_Vector PreviousVector;
	PreviousVector.Angle = -1000;
	int AngleChanges = 0;
	DFCS_Vector out_Data;

	while ( !vecList->isEmpty() )
	{

		/* Get the small vectors from the list */
		currentPtr = vecList->getFirtstPtr();
		dataGetter = currentPtr->getData();	// Contains a smallDFCSVector, in which one of the coordinates
											// might be either the start or the end  of the segment.

		if( ((int)(abs(PreviousVector.Angle - dataGetter.Angle))) > 0 )	//Casting to truncate decimal part of degree
		{
			++AngleChanges;
			cout<<"\nAngle Changes: "<<AngleChanges;
			{
				if( fp_Segments_List.isEmpty() )
				{
					cout<<endl<<"Angle Change 1: Rem Not Zero: "<<dataGetter.Start.x <<", "<<dataGetter.Start.y<<endl;
					fp_Segment.Start = dataGetter.Start;
					fp_Segment.Angle = dataGetter.Angle;
					fp_Segments_List.insertAtBack(fp_Segment);	// Saving the current Buffer in Prep for its reuse.
				}
				else
				{
					//cout<<endl<<"Angle Change 2: Rem Not Zero: "<<PreviousVector.End.x <<", "<<PreviousVector.End.y<<endl;
					fp_Segment.Start = dataGetter.Start;
					fp_Segment.Angle = dataGetter.Angle;
					fp_Segments_List.insertAtBack(fp_Segment);	// Saving the current Buffer in Prep for its reuse.
				}

			}
		}
		PreviousVector = dataGetter;
		vecList->removeFromFront(dataGetter);	// Removing the small  DFCS vector from the  list

		if( vecList->isEmpty() )
		{
			//Store the last point into a large vector
			fp_Segment.Start = PreviousVector.End;
				fp_Segment.Angle = -1000;
				cout << endl << "FP Segment: " << fp_Segment.Start.x << ", " << fp_Segment.Start.y << ", "
					<< fp_Segment.Angle <<endl;
				fp_Segments_List.insertAtBack(fp_Segment);	// Saving the current Buffer in Prep for its reuse.
		}

	}


	while ( !fp_Segments_List.isEmpty() )
	{
		currentPtr = fp_Segments_List.getFirtstPtr();
		dataGetter = currentPtr->getData();	// Contains a smallDFCSVector, in which one of the coordinates
		cout << endl << "Reading FP Segment Buffer: " << dataGetter.Start.x << ", " << dataGetter.Start.y << ", "
			<< dataGetter.Angle <<endl;

		//OUTPUT_SEGMENT_FILE << dataGetter.Start.x << '\t' << dataGetter.Start.y << '\t'
		//	<< dataGetter.Angle <<'\n';	//OUTPUT_SEGMENT_FILE 4
		out_Data.Start.index = 0;
		out_Data.Start.h = -1000;
		out_Data.Start.x = dataGetter.Start.x;
		out_Data.Start.y = dataGetter.Start.y;
		out_Data.End.index = 0;
		out_Data.End.h = -1000;
		out_Data.End.x = 0;
		out_Data.End.y = 0;
		out_Data.Angle = dataGetter.Angle;


		segList->insertAtBack( out_Data );   // put items in the queue
		//segList->insertAtFront( out_Data );   // put items in the queue


		fp_Segments_List.removeFromFront(dataGetter);	// Removing the small  DFCS vector from the  list
	}

	//OUTPUT_SEGMENT_FILE.close();	//OUTPUT_SEGMENT_FILE 5
	cout << endl;
}

void FlightPatternGenerator::GetWaypointListFromVectorList(List<DFCS_Vector>* vecList, List<DFCS_Vector>* segList, const char* segFileName)
{
	unsigned long int in_coord_index = 0;
	double in_X1 = 0;
	double in_Y1 = 0;
	double in_X2 = 0;
	double in_Y2 = 0;
	double in_Angle = -1;

	ofstream OUTPUT_SEGMENT_FILE( segFileName, ios::out );	//OUTPUT_SEGMENT_FILE 1
	if ( !OUTPUT_SEGMENT_FILE)	//OUTPUT_SEGMENT_FILE 2
	{
		exit( 1 );
	}
	else
		cout<<"\n File: "<< segFileName <<" Created\n";

	OUTPUT_SEGMENT_FILE<<setiosflags(ios::showpoint)<<setprecision(20);	//OUTPUT_SEGMENT_FILE 3

	cout << endl << "Dequeing DFCS Coordinates from vecList" << endl;

	DFCS_Vector dataGetter;		// Temporary buffer
	DFCS_Vector fp_Segment;		// Temporary buffer
	List<DFCS_Vector> fp_Segments_List;
	ListNode< DFCS_Vector > *currentPtr;	// Temporary buffer, I think

	DFCS_Vector PreviousVector;
	PreviousVector.Angle = -1000;
	int AngleChanges = 0;
	DFCS_Vector out_Data;

	while ( !vecList->isEmpty() )
	{

		/* Get the small vectors from the list */
		currentPtr = vecList->getFirtstPtr();
		dataGetter = currentPtr->getData();	// Contains a smallDFCSVector, in which one of the coordinates
											// might be either the start or the end  of the segment.

		if( ((int)(abs(PreviousVector.Angle - dataGetter.Angle))) > 0 )	//Casting to truncate decimal part of degree
		{
			++AngleChanges;
			cout<<"\nAngle Changes: "<<AngleChanges;
			{
				if( fp_Segments_List.isEmpty() )
				{
					cout<<endl<<"Angle Change 1: Rem Not Zero: "<<dataGetter.Start.x <<", "<<dataGetter.Start.y<<endl;
					fp_Segment.Start = dataGetter.Start;
					fp_Segment.Angle = dataGetter.Angle;
					fp_Segments_List.insertAtBack(fp_Segment);	// Saving the current Buffer in Prep for its reuse.
				}
				else
				{
					//cout<<endl<<"Angle Change 2: Rem Not Zero: "<<PreviousVector.End.x <<", "<<PreviousVector.End.y<<endl;
					fp_Segment.Start = dataGetter.Start;
					fp_Segment.Angle = dataGetter.Angle;
					fp_Segments_List.insertAtBack(fp_Segment);	// Saving the current Buffer in Prep for its reuse.
				}

			}
		}
		PreviousVector = dataGetter;
		vecList->removeFromFront(dataGetter);	// Removing the small  DFCS vector from the  list

		if( vecList->isEmpty() )
		{
			//Store the last point into a large vector
			fp_Segment.Start = PreviousVector.End;
				fp_Segment.Angle = -1000;
				cout << endl << "FP Segment: " << fp_Segment.Start.x << ", " << fp_Segment.Start.y << ", "
					<< fp_Segment.Angle <<endl;
				fp_Segments_List.insertAtBack(fp_Segment);	// Saving the current Buffer in Prep for its reuse.
		}

	}


	while ( !fp_Segments_List.isEmpty() )
	{
		currentPtr = fp_Segments_List.getFirtstPtr();
		dataGetter = currentPtr->getData();	// Contains a smallDFCSVector, in which one of the coordinates
		cout << endl << "Reading FP Segment Buffer: " << dataGetter.Start.x << ", " << dataGetter.Start.y << ", "
			<< dataGetter.Angle <<endl;

		out_Data.Start.index = 0;
		out_Data.Start.h = -1000;
		out_Data.Start.x = dataGetter.Start.x;
		out_Data.Start.y = dataGetter.Start.y;
		out_Data.End.index = 0;
		out_Data.End.h = -1000;
		out_Data.End.x = 0;
		out_Data.End.y = 0;
		out_Data.Angle = dataGetter.Angle;


		segList->insertAtBack( out_Data );   // put items in the queue
		//segList->insertAtFront( out_Data );   // put items in the queue
		OUTPUT_SEGMENT_FILE << out_Data.Start.x << '\t' << out_Data.Start.y << '\t'
			<< out_Data.Angle <<'\n';	//OUTPUT_SEGMENT_FILE 4


		fp_Segments_List.removeFromFront(dataGetter);	// Removing the small  DFCS vector from the  list
	}

	OUTPUT_SEGMENT_FILE.close();	//OUTPUT_SEGMENT_FILE 5
	cout << endl;
}
// Does this modify the segList
void FlightPatternGenerator::GetFPatListFromWaypointList(List<DFCS_Vector>* segList, List<DFCS_FPatSegment>* fpatList)
{
	bool MannySotoTester4 = false;
	// Create copy of buffer to preserve the passed buffer
	List<DFCS_Vector> cpy_segList;
	ListNode< DFCS_Vector > *cpy_currentPtr;
	DFCS_Vector cpy_dataGetter;
	cpy_currentPtr = segList->getFirtstPtr();
	while ( cpy_currentPtr != 0 )
	{
		cpy_dataGetter = cpy_currentPtr->getData();
		//cpy_segList.insertAtFront(cpy_dataGetter);
		cpy_segList.insertAtBack(cpy_dataGetter);
		cpy_currentPtr = segList->getNextPtr(cpy_currentPtr);
	}

	// Declarations
	DFCS_Vector startVector;		// Temporary buffer
	DFCS_Vector endVector;		// Temporary buffer
	DFCS_FPatSegment segmentBuffer;
	ListNode< DFCS_Vector > *currentPtr;	// Temporary buffer, I think
	int segmentCount=0;
	int x0fp_rise = 1;	// for computing the arc center of circle
	int x0fp_run = 1;	// for computing the arc center of circle
	int xnfp_rise = 1;	// for computing the start point of the arc
	int xnfp_run = 1;	// for computing the start point of the arc
	int nxtSeg_run = 1;	// for computing the x point after the arc
	int nxtSeg_rise = 1;	// for computing the x point after the arc
	//const double rightAngle_135_angle = 90.0;
	const double bigAngle_135_angle = (135.0/2.0);
	const double smallAngle_135_angle = 180.0 - bigAngle_135_angle - 90.0;//rightAngle_135_angle;
	const double hyp_135_deg_angle = m_step/cos(RadPerDeg*smallAngle_135_angle);
	const double x_135_deg_angle = hyp_135_deg_angle*cos(RadPerDeg*smallAngle_135_angle);
	const double y_135_deg_angle = hyp_135_deg_angle*sin(RadPerDeg*smallAngle_135_angle);
////////
	const double hyp_90_deg_angle = m_step/cos(RadPerDeg*45);
	const double x_90_deg_angle = hyp_90_deg_angle*cos(RadPerDeg*45);
	const double y_90_deg_angle = hyp_90_deg_angle*sin(RadPerDeg*45);

	if( m_step < 0)
	{
		//Apr 12, 2018//MessageBox(NULL, _T("\'Turn Radius\' requires NED step size.\nFlight pattern was not generated"), _T("Flight Pattern Error"), MB_ICONEXCLAMATION);
		return;
	}

	if( m_droneMSL < -99999.0)
	{
		//Apr 12, 2018//MessageBox(NULL, _T("\'Drone MSL\' must be set.\nFlight pattern was not generated"), _T("Flight Pattern Error"), MB_ICONEXCLAMATION);
		return;
	}

	// Main processing loop
	while ( !cpy_segList.isEmpty() )
	{
		// Store the data of the begining point of the DFCS Segment
		currentPtr = cpy_segList.getFirtstPtr();
		startVector = currentPtr->getData();
		cpy_segList.removeFromFront(startVector);	// Removing the point to get access to the end point which contains a discontinuity

		if( cpy_segList.isEmpty() )
		{
			break;
		}

		// Assuming first part of segment will be a straight line.
		// This is a flaw in the fpat system, but necessary to get the momentum,
		// and will be corrected as the class matures.
		// Begin by storing the straight assumed line segment onto the startVector buffer
		++segmentCount;
		segmentBuffer.segment = segmentCount;
		segmentBuffer.nxtSegment = segmentCount + 1;
		segmentBuffer.xnfp = startVector.Start.x;
		segmentBuffer.ynfp = startVector.Start.y;
		segmentBuffer.dfp = dfp_straight;	// from the straight line assumption
		segmentBuffer.hnfp = m_droneMSL;
		segmentBuffer.x0fp = 0;
		segmentBuffer.y0fp = 0;
		fpatList->insertAtBack(segmentBuffer);

		currentPtr = cpy_segList.getFirtstPtr();	// Get access to the start of the list
		endVector = currentPtr->getData();	// Contains the point which is to be converted to an arc
		cpy_segList.removeFromFront(endVector);		// start point of the next segment, which must be converted to an arc

		// From the assumption of the previous segment being a straight line,
		// the next segment MUST be converted to an arc segment.
		// Store the arc part
		++segmentCount;
		segmentBuffer.segment = segmentCount;
		segmentBuffer.nxtSegment = segmentCount + 1;
		if( endVector.Angle < 0 )
		{
			segmentBuffer.xnfp = endVector.Start.x;
			segmentBuffer.ynfp = endVector.Start.y;
			segmentBuffer.dfp = dfp_straight;	// from the straight line assumption
			segmentBuffer.hnfp = m_droneMSL;
			segmentBuffer.x0fp = 0;
			segmentBuffer.y0fp = 0;
		}
		else
		{
			if( (((int)startVector.Start.x < (int)endVector.Start.x) && ((int)startVector.Start.y < (int)endVector.Start.y)) )
			{//1
				if ( startVector.Angle >= 360 )
					startVector.Angle = startVector.Angle - 360;

				if( (startVector.Angle > 270) && (endVector.Angle < 90) )	// using integer ranges to avoid floating point error
					endVector.Angle = endVector.Angle + 360;

				if( ((int)(startVector.Angle) > 0) && (startVector.Angle <= 90) && (endVector.Angle >= 270) )	// using integer ranges to avoid floating point error
					startVector.Angle = startVector.Angle + 360;



				if( (startVector.Angle - endVector.Angle) < 0 )	// Counter clock wise case
				{//1CW
					MannySotoTester4 = true;
					//segmentBuffer.dfp = dfp_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = -1;
						x0fp_run = 0;
						xnfp_rise = -1;
						xnfp_run = -1;
						nxtSeg_run = 1;
						nxtSeg_rise = -1;
					}
					else
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = -1;
						x0fp_run = 1;
						xnfp_rise = -1;
						xnfp_run = -1;
						nxtSeg_run = 1;
						nxtSeg_rise = 1;
					}
				}
				else	// Clock wise case
				{//1CCW
					//segmentBuffer.dfp = dfp_counter_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = 0;
						x0fp_run = -1;
						xnfp_rise = -1;
						xnfp_run = -1;
						nxtSeg_run = -1;
						nxtSeg_rise = 1;
					}
					else
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = 1;
						x0fp_run = -1;
						xnfp_rise = -1;
						xnfp_run = -1;
						nxtSeg_run = 0;
						nxtSeg_rise = 1;
					}
				}
			}
			if( (((int)startVector.Start.x > (int)endVector.Start.x) && ((int)startVector.Start.y > (int)endVector.Start.y)) )
			{//2
				if ( startVector.Angle >= 360 )
					startVector.Angle = startVector.Angle - 360;

				if( (startVector.Angle > 270) && (endVector.Angle < 90) )	// using integer ranges to avoid floating point error
					endVector.Angle = endVector.Angle + 360;

				if( ((int)(startVector.Angle) > 0) && (startVector.Angle <= 90) && (endVector.Angle >= 270) )	// using integer ranges to avoid floating point error
					startVector.Angle = startVector.Angle + 360;


				if( (startVector.Angle - endVector.Angle) < 0 )	// Counter clock wise case
				{//2CW
					MannySotoTester4 = true;
					//segmentBuffer.dfp = dfp_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = 1;
						x0fp_run = 0;
						xnfp_rise = 1;
						xnfp_run = 1;
						nxtSeg_run = -1;
						nxtSeg_rise = 1;
					}
					else
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = 1;
						x0fp_run = -1;
						xnfp_rise = 1;
						xnfp_run = 1;
						nxtSeg_run = -1;
						nxtSeg_rise = 1;
					}
				}
				else	// Counter Clock wise case
				{//2CCW
					//segmentBuffer.dfp = dfp_counter_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = 0;
						x0fp_run = 1;
						xnfp_rise = 1;
						xnfp_run = 1;
						nxtSeg_run = 1;
						nxtSeg_rise = -1;
					}
					else
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = -1;
						x0fp_run = 1;
						xnfp_rise = 1;
						xnfp_run = 1;
						nxtSeg_run = 0;
						nxtSeg_rise = -1;
					}
				}
			}
			if( (((int)startVector.Start.x < (int)endVector.Start.x) && ((int)startVector.Start.y > (int)endVector.Start.y)) )	// Testing on Quadrant IV
			{//3
				if ( startVector.Angle >= 360 )
					startVector.Angle = startVector.Angle - 360;

				if( (startVector.Angle > 270) && (endVector.Angle < 90) )	// using integer ranges to avoid floating point error
					endVector.Angle = endVector.Angle + 360;

				if( ((int)(startVector.Angle) > 0) && (startVector.Angle <= 90) && (endVector.Angle >= 270) )	// using integer ranges to avoid floating point error
					startVector.Angle = startVector.Angle + 360;


				if( (startVector.Angle - endVector.Angle) < 0 )	// Counter clock wise case
				{//3CCW
					//segmentBuffer.dfp = dfp_counter_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = 0;
						x0fp_run = -1;
						xnfp_rise = 1;
						xnfp_run = -1;
						nxtSeg_run = -1;
						nxtSeg_rise = -1;
					}
					else
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = -1;
						x0fp_run = -1;
						xnfp_rise = 1;
						xnfp_run = -1;
						nxtSeg_run = 0;
						nxtSeg_rise = -1;
					}
				}
				else	// Clock wise case
				{//3CW
					MannySotoTester4 = true;
					//segmentBuffer.dfp = dfp_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = 1;
						x0fp_run = 0;
						xnfp_rise = 1;
						xnfp_run = -1;
						nxtSeg_run = 1;
						nxtSeg_rise = 1;
					}
					else
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = 1;
						x0fp_run = 1;
						xnfp_rise = 1;
						xnfp_run = -1;
						nxtSeg_run = 1;
						nxtSeg_rise = 0;
					}
				}
			}
			if( (((int)startVector.Start.x > (int)endVector.Start.x) && ((int)startVector.Start.y < (int)endVector.Start.y)) )	// Testing on Quadrant I
			{//4
				if ( startVector.Angle >= 360 )
					startVector.Angle = startVector.Angle - 360;

				if( (startVector.Angle > 270) && (endVector.Angle < 90) )	// using integer ranges to avoid floating point error
					endVector.Angle = endVector.Angle + 360;

				if( ((int)(startVector.Angle) > 0) && (startVector.Angle <= 90) && (endVector.Angle >= 270) )	// using integer ranges to avoid floating point error
					startVector.Angle = startVector.Angle + 360;


				if( (startVector.Angle - endVector.Angle) < 0 )	// Counter clock wise case
				{//4CCW
					//segmentBuffer.dfp = dfp_counter_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = 0;
						x0fp_run = 1;
						xnfp_rise = -1;
						xnfp_run = 1;
						nxtSeg_run = 1;
						nxtSeg_rise = 1;
					}
					else
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = 1;
						x0fp_run = 1;
						xnfp_rise = -1;
						xnfp_run = 1;
						nxtSeg_run = -1;
						nxtSeg_rise = 1;
					}
				}
				else	// Clock wise case
				{//4CW
					MannySotoTester4 = true;
					//segmentBuffer.dfp = dfp_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = -1;
						x0fp_run = 0;
						xnfp_rise = -1;
						xnfp_run = 1;
						nxtSeg_run = -1;
						nxtSeg_rise = -1;
					}
					else
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = -1;
						x0fp_run = -1;
						xnfp_rise = -1;
						xnfp_run = 1;
						nxtSeg_run = -1;
						nxtSeg_rise = 0;
					}
				}
			}


			if( (((int)startVector.Start.x == (int)endVector.Start.x) && ((int)startVector.Start.y > (int)endVector.Start.y)) )	// Testing on Quadrant I
			{//5
				if ( startVector.Angle >= 360 )
					startVector.Angle = startVector.Angle - 360;

				if( (startVector.Angle > 270) && (endVector.Angle < 90) )	// using integer ranges to avoid floating point error
					endVector.Angle = endVector.Angle + 360;

				if( ((int)(startVector.Angle) > 0) && (startVector.Angle <= 90) && (endVector.Angle >= 270) )	// using integer ranges to avoid floating point error
					startVector.Angle = startVector.Angle + 360;


				if( (startVector.Angle - endVector.Angle) < 0 )	// Counter clock wise case
				{//5CCW
					//segmentBuffer.dfp = dfp_counter_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = 1;
						x0fp_run = -1;
						xnfp_rise = 1;
						xnfp_run = 0;//Experiment1;
						nxtSeg_rise = 0;
						nxtSeg_run = -1;
					}
					else
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = 1;
						x0fp_run = -1;
						xnfp_rise = 1;
						xnfp_run = 0;//Experiment1;
						nxtSeg_rise = -1;
						nxtSeg_run = -1;
					}
				}
				else	// Clock wise case
				{//5CW
					MannySotoTester4 = true;
					//segmentBuffer.dfp = dfp_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = 1;
						x0fp_run = 1;
						xnfp_rise = 1;
						xnfp_run = 0;
						nxtSeg_rise = 0;
						nxtSeg_run = 1;
					}
					else
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = 1;
						x0fp_run = 1;
						xnfp_rise = 1;
						xnfp_run = 0;
						nxtSeg_rise = -1;
						nxtSeg_run = 1;
					}
				}
			}


			if( (((int)startVector.Start.x == (int)endVector.Start.x) && ((int)startVector.Start.y < (int)endVector.Start.y)) )	// Testing on Quadrant I
			{//6
				//if( (startVector.Angle > 270) && (endVector.Angle < 1) )	// using integer ranges to avoid floating point error
				//	endVector.Angle = 360;
				if ( startVector.Angle >= 360 )
					startVector.Angle = startVector.Angle - 360;

				if( (endVector.Angle > 270) && (startVector.Angle < 90) )	// using integer ranges to avoid floating point error
				{
					//startVector.Angle = endVector.Angle + 360;
					startVector.Angle = startVector.Angle + 360;
				}
				if( ((int)(startVector.Angle) >= 0) && (startVector.Angle <= 90) && (endVector.Angle >= 270) )	// using integer ranges to avoid floating point error
					startVector.Angle = startVector.Angle + 360;


				if( (startVector.Angle - endVector.Angle) < 0 )	// Counter clock wise case
				{//6CCW
					//segmentBuffer.dfp = dfp_counter_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = -1;
						x0fp_run = 1;
						xnfp_rise = -1;
						xnfp_run = 0;
						nxtSeg_rise = 0;
						nxtSeg_run = 1;
					}
					else
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = -1;
						x0fp_run = 1;
						xnfp_rise = -1;
						xnfp_run = 0;
						nxtSeg_rise = 1;
						nxtSeg_run = 1;
					}
				}
				else	// Clock wise case
				{//6CW
					MannySotoTester4 = true;
					//segmentBuffer.dfp = dfp_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = -1;
						x0fp_run = -1;
						xnfp_rise = -1;
						xnfp_run = 0;
						nxtSeg_rise = 1;
						nxtSeg_run = -1;
					}
					else
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = -1;
						x0fp_run = -1;
						xnfp_rise = -1;
						xnfp_run = 0;
						nxtSeg_rise = 1;
						nxtSeg_run = -1;
					}
				}
			}


			if( (((int)startVector.Start.y == (int)endVector.Start.y) && ((int)startVector.Start.x > (int)endVector.Start.x)) )	// Testing on Quadrant I
			{//7
				if ( startVector.Angle >= 360 )
					startVector.Angle = startVector.Angle - 360;

				if( (startVector.Angle >= 270) && (endVector.Angle < 90) )	// using integer ranges to avoid floating point error
					endVector.Angle = endVector.Angle + 360;

				if( ((int)(startVector.Angle) > 0) && (startVector.Angle <= 90) && (endVector.Angle >= 270) )	// using integer ranges to avoid floating point error
					startVector.Angle = startVector.Angle + 360;


				if( (startVector.Angle - endVector.Angle) < 0 )	// Counter clock wise case
				{//7CCW
					//segmentBuffer.dfp = dfp_counter_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = 1;
						x0fp_run = 1;
						xnfp_rise = 0;
						xnfp_run = 1;
						nxtSeg_rise = 1;
						nxtSeg_run = -1;
					}
					else
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = 1;
						x0fp_run = 1;
						xnfp_rise = 0;
						xnfp_run = 1;
						nxtSeg_rise = 1;
						nxtSeg_run = -1;
					}
				}
				else	// Clock wise case
				{//7CW
					MannySotoTester4 = true;
					//segmentBuffer.dfp = dfp_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = -1;
						x0fp_run = 1;
						xnfp_rise = 0;
						xnfp_run = 1;
						nxtSeg_rise = -1;
						nxtSeg_run = 0;
					}
					else
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = -1;
						x0fp_run = 1;
						xnfp_rise = 0;
						xnfp_run = 1;
						nxtSeg_rise = -1;
						nxtSeg_run = -1;
					}
				}
			}


			if( (((int)startVector.Start.y == (int)endVector.Start.y) && ((int)startVector.Start.x < (int)endVector.Start.x)) )	// Testing on Quadrant I
			{//8
				if ( startVector.Angle >= 360 )
					startVector.Angle = startVector.Angle - 360;

				if( (startVector.Angle > 270) && (endVector.Angle < 90) )	// using integer ranges to avoid floating point error
					endVector.Angle = endVector.Angle + 360;

				if( ((int)(startVector.Angle) > 0) && (startVector.Angle <= 90) && (endVector.Angle >= 270) )	// using integer ranges to avoid floating point error
					startVector.Angle = startVector.Angle + 360;


				if( (startVector.Angle - endVector.Angle) < 0 )	// Counter clock wise case
				{
					//segmentBuffer.dfp = dfp_counter_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = -1;
						x0fp_run = -1;
						xnfp_rise = 0;
						xnfp_run = -1;
						nxtSeg_rise = -1;
						nxtSeg_run = 0;
					}
					else
					{
						segmentBuffer.dfp = dfp_clock_wise;
						x0fp_rise = -1;
						x0fp_run = -1;
						xnfp_rise = 0;
						xnfp_run = -1;
						nxtSeg_rise = -1;
						nxtSeg_run = 1;
					}
				}
				else	// Clock wise case
				{//8cw
					MannySotoTester4 = true;
					//segmentBuffer.dfp = dfp_clock_wise;
					if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = 1;
						x0fp_run = -1;
						xnfp_rise = 0;
						xnfp_run = -1;
						nxtSeg_rise = 1;
						nxtSeg_run = 0;
					}
					else
					{
						segmentBuffer.dfp = dfp_counter_clock_wise;
						x0fp_rise = 1;
						x0fp_run = -1;
						xnfp_rise = 0;
						xnfp_run = -1;
						nxtSeg_rise = 1;
						nxtSeg_run = 1;
					}
				}
			}



///////////
			if( (((int)startVector.Start.x == (int)endVector.Start.x) || ((int)startVector.Start.y == (int)endVector.Start.y)) )
			{
				if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
				{
					if (MannySotoTester4 == false)
					{// CCW only ?
						if( ((int)startVector.Start.x == (int)endVector.Start.x) )	//works
						{
							segmentBuffer.x0fp = endVector.Start.x + x0fp_run*m_step;
							segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*m_step;
							segmentBuffer.hnfp = m_droneMSL;
							segmentBuffer.xnfp = endVector.Start.x;
							segmentBuffer.ynfp = endVector.Start.y + xnfp_rise*m_step;
							segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
						}

						if( ((int)startVector.Start.y == (int)endVector.Start.y) )
						{
							segmentBuffer.x0fp = endVector.Start.x + x0fp_run*m_step;
							segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*m_step;
							segmentBuffer.hnfp = m_droneMSL;
							segmentBuffer.xnfp = endVector.Start.x + xnfp_run*m_step;
							segmentBuffer.ynfp = endVector.Start.y;
							segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
						}

					}
					else
					{// CW only ?
						if( ((int)startVector.Start.x == (int)endVector.Start.x) )	//works
						{
							segmentBuffer.x0fp = endVector.Start.x + x0fp_run*m_step;
							segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*m_step;
							segmentBuffer.hnfp = m_droneMSL;
							segmentBuffer.xnfp = endVector.Start.x;
							segmentBuffer.ynfp = endVector.Start.y + xnfp_rise*m_step;
							segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
						}

						if( ((int)startVector.Start.y == (int)endVector.Start.y) )
						{
							segmentBuffer.x0fp = endVector.Start.x + x0fp_run*m_step;
							segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*m_step;
							segmentBuffer.hnfp = m_droneMSL;
							segmentBuffer.xnfp = endVector.Start.x + xnfp_run*m_step;
							segmentBuffer.ynfp = endVector.Start.y;
							segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
						}
					}


				}
				else
				{
					if (MannySotoTester4 == false)
					{// CCWonly ?
						if( ((int)startVector.Start.x == (int)endVector.Start.x) )	//works
						{
							segmentBuffer.xnfp = endVector.Start.x;// + xnfp_run*(m_step/2.0);
							segmentBuffer.ynfp = endVector.Start.y + xnfp_rise*y_135_deg_angle;
							segmentBuffer.hnfp = m_droneMSL;
							segmentBuffer.x0fp = endVector.Start.x + x0fp_run*x_135_deg_angle;//(m_step/2.0);//(m_step/sqrt(2.0));
							segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*y_135_deg_angle;// + x0fp_rise*(m_step/sqrt(2.0));
							segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
						}

						if( ((int)startVector.Start.y == (int)endVector.Start.y) )
						{
							segmentBuffer.xnfp = endVector.Start.x + xnfp_run*y_135_deg_angle;
							segmentBuffer.ynfp = endVector.Start.y;// + xnfp_rise*(m_step/2.0);
							segmentBuffer.hnfp = m_droneMSL;
							segmentBuffer.x0fp = endVector.Start.x + x0fp_run*y_135_deg_angle;// + x0fp_run*(m_step/2.0);//(m_step/sqrt(2.0));
							segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*x_135_deg_angle;//(m_step/(2.0));
							segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
						}

					}
					else
					{// CWonly ?
						if( ((int)startVector.Start.x == (int)endVector.Start.x) )	//works
						{
							segmentBuffer.xnfp = endVector.Start.x + xnfp_run;//*(m_step/2.0);
							segmentBuffer.ynfp = endVector.Start.y + xnfp_rise*y_135_deg_angle ;// *(m_step/2.0);
							segmentBuffer.hnfp = m_droneMSL;
							segmentBuffer.x0fp = endVector.Start.x + x0fp_run*x_135_deg_angle;//(m_step/2.0);//(m_step/sqrt(2.0));
							segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*y_135_deg_angle;// + x0fp_rise*(m_step/sqrt(2.0));
							segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
						}

						if( ((int)startVector.Start.y == (int)endVector.Start.y) )
						{
							segmentBuffer.xnfp = endVector.Start.x + xnfp_run*y_135_deg_angle;
							segmentBuffer.ynfp = endVector.Start.y;
							segmentBuffer.hnfp = m_droneMSL;
							segmentBuffer.x0fp = endVector.Start.x + x0fp_run*y_135_deg_angle;// + x0fp_run*(m_step/2.0);//(m_step/sqrt(2.0));
							segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*x_135_deg_angle;//(m_step/(2.0));
							segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
						}
					}

				}
			}
			else
			{
				if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
				{
					if (MannySotoTester4 == false)
					{// CCW only ?
						segmentBuffer.xnfp = endVector.Start.x + xnfp_run*x_90_deg_angle*sin(RadPerDeg*45.0);
						segmentBuffer.ynfp = endVector.Start.y + xnfp_rise*x_90_deg_angle*cos(RadPerDeg*45.0);
						segmentBuffer.hnfp = m_droneMSL;
						segmentBuffer.x0fp = endVector.Start.x + x0fp_run*hyp_90_deg_angle;
						segmentBuffer.y0fp = endVector.Start.y;
						segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
					}
					else
					{
						segmentBuffer.xnfp = endVector.Start.x + xnfp_run*x_90_deg_angle*cos(RadPerDeg*45.0);
						segmentBuffer.ynfp = endVector.Start.y + xnfp_rise*y_90_deg_angle*cos(RadPerDeg*45.0);
						segmentBuffer.hnfp = m_droneMSL;
						segmentBuffer.x0fp = endVector.Start.x;
						segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*hyp_90_deg_angle;
						segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
					}

				}
				else
				{
					if (MannySotoTester4 == false)
					{
						segmentBuffer.xnfp = endVector.Start.x + xnfp_run*y_135_deg_angle*cos(RadPerDeg*45.0);//m_step/(2.0);//*sin(3.1416/180*45.0);//(m_step/2.0);
						segmentBuffer.ynfp = endVector.Start.y + xnfp_rise*y_135_deg_angle*cos(RadPerDeg*45.0);//m_step/(2.0);//*cos(3.1416/180*45.0);//(m_step/2.0);
						segmentBuffer.hnfp = m_droneMSL;
						segmentBuffer.x0fp = endVector.Start.x + x0fp_run*x_135_deg_angle;//m_step*cos(3.1416/180*14.50);//(m_step/sqrt(2.0));
						segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*y_135_deg_angle;//m_step*sin(3.1416/180*14.50);//(m_step/sqrt(2.0));
						segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
						//segmentBuffer.x0fp = segmentBuffer.xnfp + x0fp_run*cos(15.0);//(m_step/sqrt(2.0));
						//segmentBuffer.y0fp = segmentBuffer.ynfp + x0fp_rise*sin(15.0);//(m_step/sqrt(2.0));
					}
					else
					{//cw?
						segmentBuffer.xnfp = endVector.Start.x + xnfp_run*y_135_deg_angle*cos(RadPerDeg*45.0);//(m_step/2.0);
						segmentBuffer.ynfp = endVector.Start.y + xnfp_rise*y_135_deg_angle*cos(RadPerDeg*45.0);//*(m_step/2.0);
						segmentBuffer.hnfp = m_droneMSL;
						segmentBuffer.x0fp = endVector.Start.x + x0fp_run*y_135_deg_angle;//(m_step/sqrt(2.0));
						segmentBuffer.y0fp = endVector.Start.y + x0fp_rise*x_135_deg_angle;//(m_step/sqrt(2.0));
						segmentBuffer.f_spare[0] = abs(startVector.Angle - endVector.Angle);
					}

				}
			}
		}

		fpatList->insertAtBack(segmentBuffer);
		// Modify endVector and put modified end vector at the segment list
		// to be used as the next start point.
		if( (((int)startVector.Start.x == (int)endVector.Start.x) || ((int)startVector.Start.y == (int)endVector.Start.y)) )
		{
			if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
			{

				if (MannySotoTester4 == false)
				{// CCW only?
					if( ((int)startVector.Start.x == (int)endVector.Start.x) )	//works
					{
						endVector.Start.x = endVector.Start.x + nxtSeg_run*m_step;
						endVector.Start.y = endVector.Start.y;
					}

					if( ((int)startVector.Start.y == (int)endVector.Start.y) )	//works
					{
						endVector.Start.x = endVector.Start.x;
						endVector.Start.y = endVector.Start.y + nxtSeg_rise*m_step;

					}
				}
				else
				{// CW only?
					if( ((int)startVector.Start.x == (int)endVector.Start.x) )	//works
					{
						endVector.Start.x = endVector.Start.x+ nxtSeg_run*m_step;
						endVector.Start.y = endVector.Start.y;
						MannySotoTester4 = false;
					}

					if( ((int)startVector.Start.y == (int)endVector.Start.y) )	//works
					{
						endVector.Start.x = endVector.Start.x;
						endVector.Start.y = endVector.Start.y + nxtSeg_rise*m_step;
						MannySotoTester4 = false;
					}
				}

			}
			else
			{

				if (MannySotoTester4 == false)
				{// CCW only?
					if( ((int)startVector.Start.x == (int)endVector.Start.x) )	//works
					{
						endVector.Start.x = endVector.Start.x + nxtSeg_run*y_135_deg_angle*sin(RadPerDeg*45.0);
						endVector.Start.y = endVector.Start.y + nxtSeg_rise*y_135_deg_angle*cos(RadPerDeg*45.0);
					}

					if( ((int)startVector.Start.y == (int)endVector.Start.y) )	//works
					{
						endVector.Start.x = endVector.Start.x + nxtSeg_run*y_135_deg_angle*cos(RadPerDeg*45.0);//cos(45.0);// + nxtSeg_run*m_step;
						endVector.Start.y = endVector.Start.y + nxtSeg_rise*y_135_deg_angle*sin(RadPerDeg*45.0);//sin(45.0);

					}
				}
				else
				{// CW only?
					if( ((int)startVector.Start.x == (int)endVector.Start.x) )	//works
					{
						endVector.Start.x = endVector.Start.x+ nxtSeg_run*y_135_deg_angle*sin(RadPerDeg*45.0);
						endVector.Start.y = endVector.Start.y + nxtSeg_rise*y_135_deg_angle*cos(RadPerDeg*45.0);
						MannySotoTester4 = false;
					}

					if( ((int)startVector.Start.y == (int)endVector.Start.y) )	//works
					{
						endVector.Start.x = endVector.Start.x + nxtSeg_run*y_135_deg_angle*sin(RadPerDeg*45.0);
						endVector.Start.y = endVector.Start.y + nxtSeg_rise*y_135_deg_angle*cos(RadPerDeg*45.0);
						MannySotoTester4 = false;
					}
				}

			}
		}
		else
		{
			if( (abs((int)startVector.Angle - (int)endVector.Angle) < 100 )  && (abs((int)startVector.Angle - (int)endVector.Angle) > 80 )  )
			{
				if (MannySotoTester4 == false)
				{
					endVector.Start.x = endVector.Start.x + nxtSeg_run*x_90_deg_angle*sin(RadPerDeg*45.0);
					endVector.Start.y = endVector.Start.y + nxtSeg_rise*x_90_deg_angle*cos(RadPerDeg*45.0);
				}
				else
				{
					endVector.Start.x = endVector.Start.x + nxtSeg_run*x_90_deg_angle*cos(RadPerDeg*45.0);
					endVector.Start.y = endVector.Start.y + nxtSeg_rise*x_90_deg_angle*sin(RadPerDeg*45.0);
					MannySotoTester4 = false;
				}
			}
			else
			{
				if (MannySotoTester4 == false)
				{
					endVector.Start.x = endVector.Start.x;// + nxtSeg_run*m_step/2.0;
					endVector.Start.y = endVector.Start.y + nxtSeg_rise*y_135_deg_angle;//m_step/2.0;
				}
				else
				{
					endVector.Start.x = endVector.Start.x + nxtSeg_run*y_135_deg_angle;
					endVector.Start.y = endVector.Start.y;// + nxtSeg_run*m_step;
					MannySotoTester4 = false;
				}

			}
		}
		cpy_segList.insertAtFront(endVector);
	}
	return;
}
void FlightPatternGenerator::GetFPatFileFromFPatList(List<DFCS_FPatSegment>* fpatList, const char* fpatFileName)
{

	ofstream FPAT_SEG_FILE( fpatFileName, ios::out );
	if ( !FPAT_SEG_FILE)
	{
		exit( 1 );
	}
	else
		cout<<"\n File: 5dis.uav.fpat Created\n";

	FPAT_SEG_FILE<<setiosflags(ios::showpoint)<<setprecision(20);

	cout << endl << "Dequeing DFCS Coordinates from fpatList" << endl;

	DFCS_FPatSegment dataGetter;		// Temporary buffer
	ListNode< DFCS_FPatSegment > *currentPtr;	// Temporary buffer, I think

	currentPtr = fpatList->getFirtstPtr();
	while ( currentPtr != 0 )
	{
		dataGetter = currentPtr->getData();	// Contains a smallDFCSVector, in which one of the coordinates

		cout << dataGetter.segment << ", " << '\t'
			<< dataGetter.nxtSegment << ", " << '\t'
			<< dataGetter.dfp << ", " << '\t'
			<< dataGetter.hnfp << ", " << '\t'
			<< dataGetter.xnfp << ", " << '\t'
			<< dataGetter.x0fp << ", " << '\t'
			<< dataGetter.ynfp << ", " << '\t'
			<< dataGetter.y0fp << '\n' ;

		FPAT_SEG_FILE << dataGetter.segment << '\t'
			<< dataGetter.nxtSegment << '\t'
			<< dataGetter.dfp << '\t'
			<< dataGetter.hnfp << '\t'
			<< dataGetter.xnfp << '\t'
			<< dataGetter.x0fp << '\t'
			<< dataGetter.ynfp << '\t'
			<< dataGetter.y0fp << '\n' ;

		currentPtr = fpatList->getNextPtr( currentPtr );
	}

	FPAT_SEG_FILE.close();
	cout << endl;
}


void FlightPatternGenerator::SetDroneVelocity(double droneVelocity)
{
		m_droneVelocity = droneVelocity;
}

void FlightPatternGenerator::SetStepDistance(double stepDistance)
{
		m_step = stepDistance;
}

void FlightPatternGenerator::SetTurnRadius(double turnRadius)
{
		m_turnRadius = turnRadius;
}

void FlightPatternGenerator::SetDroneMSL(double droneMSL)
{
		m_droneMSL = droneMSL;
}

int FlightPatternGenerator::GetInvalidTilesListFromVectorList(List<DFCS_Vector>* vecList, List<DFCS_Coord>* invalidTilesList, DFCS_Coord Start_Door, DFCS_Coord Goal_Door)
{
	int totalInvalidVectors = 0;
	double validityTestAngles[3];

	DFCS_Coord invalidTile;
	List<DFCS_Vector> cpy_vecList;
	ListNode< DFCS_Vector > *cpy_currentPtr;
	DFCS_Vector cpy_dataGetter;
	cpy_currentPtr = vecList->getFirtstPtr();

	while ( cpy_currentPtr != 0 )
	{
		cpy_dataGetter = cpy_currentPtr->getData();
		cpy_vecList.insertAtBack(cpy_dataGetter);
		cpy_currentPtr = vecList->getNextPtr(cpy_currentPtr);
	}

	cpy_currentPtr = cpy_vecList.getFirtstPtr();
	while ( cpy_currentPtr != 0 )
	{
		cpy_dataGetter = cpy_currentPtr->getData();
		validityTestAngles[0] = cpy_dataGetter.Angle;

		cpy_currentPtr = cpy_vecList.getNextPtr(cpy_currentPtr);
		if( cpy_currentPtr == 0 )
			break;
		else
			cpy_dataGetter = cpy_currentPtr->getData();	//2st vector
		validityTestAngles[1] = cpy_dataGetter.Angle;

		if( validityTestAngles[0] != validityTestAngles[1] )
		{
			// Test for the third angle to be the same.
			cpy_currentPtr = cpy_vecList.getNextPtr(cpy_currentPtr);
			if( cpy_currentPtr == 0 )
				break;
			else
				cpy_dataGetter = cpy_currentPtr->getData();	//2st vector
			validityTestAngles[2] = cpy_dataGetter.Angle;

			if( validityTestAngles[0] == validityTestAngles[2] )
			{
				//Put the location of the tile to be penalized onto the invalid tiles list
				//if( ((int)Start_Door.x == (int)cpy_dataGetter.Start.x) && ((int)Start_Door.y == (int)cpy_dataGetter.Start.y) ||
				//	((int)Goal_Door.x == (int)cpy_dataGetter.Start.x) && ((int)Goal_Door.y == (int)cpy_dataGetter.Start.y) )
				//{
				//	MessageBox(NULL, "About to penalize a door", "Penalizing Door", MB_ICONEXCLAMATION );
				//	break;
				//}
				invalidTile.x = cpy_dataGetter.Start.x;
				invalidTile.y = cpy_dataGetter.Start.y;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				totalInvalidVectors++;
				break;
			}

		}

		cpy_currentPtr = cpy_vecList.getFirtstPtr();
		cpy_dataGetter = cpy_currentPtr->getData();
		cpy_vecList.removeFromFront( cpy_dataGetter );
		if( cpy_vecList.isEmpty() )
			break;
		else
			cpy_currentPtr = cpy_vecList.getFirtstPtr();

	}

	return totalInvalidVectors;
}


int FlightPatternGenerator::GetInvalidTilesListFromPathList(List<DFCS_Coord>* pthList, List<DFCS_Coord>* invalidTilesList, double UAV_InitialHeading)
{
	int invalidHeading = 0;
	DFCS_Coord invalidTile;
	m_droneHeading = UAV_InitialHeading;

//////////////////////////////////////////////////////////
	List<DFCS_Coord> cpy_pthList;
	ListNode< DFCS_Coord > *cpy_currentPtr;
	DFCS_Coord cpy_dataGetter;
	cpy_currentPtr = pthList->getFirtstPtr();
	while ( cpy_currentPtr != 0 )
	{
		cpy_dataGetter = cpy_currentPtr->getData();
		cpy_pthList.insertAtFront(cpy_dataGetter);
		cpy_currentPtr = pthList->getNextPtr(cpy_currentPtr);
	}
//////////////////////////////////////////////////////////

	ListNode< DFCS_Coord > *currentPtr;
	DFCS_Coord dataGetter;
	DFCS_Coord Coord1;
	DFCS_Coord Coord2;

	cout << endl << "Dequeing DFCS Coordinates from cpy_pthList" << endl;

	double angle = -1;

		currentPtr = cpy_pthList.getFirtstPtr();
		dataGetter = currentPtr->getData();
		Coord1 = dataGetter;
		cpy_pthList.removeFromFront(dataGetter);

		currentPtr = cpy_pthList.getFirtstPtr();
		dataGetter = currentPtr->getData();
		Coord2 = dataGetter;

		angle = getHSI_Angle(Coord1,  Coord2);

		cout << Coord1.index << '\t' << Coord2.index << '\t' << angle << '\n';
#if 1
		int r=10;
		char angle_buffer[65];

		//04-02-2018
		//Apr 12, 2018//_itoa_s( (int)angle, angle_buffer, r );
		//Apr 12, 2018//strcat_s( angle_buffer, " is the ANGLE");
#endif

		switch( (int)angle )
		{
		case 0:
			if(((int)m_droneHeading >= 0) && ((int)m_droneHeading <= 22))
			{
				//MessageBox( NULL, (LPCSTR)angle_buffer, "Good Angle", MB_OK);
			}
			else
			{
				// add location of Coord2 to the invalid tile list
				invalidTile.x = Coord2.x;
				invalidTile.y = Coord2.y;;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				invalidHeading++;
#if DBG_INVALID_INITIAL_UAV_HEADING
				MessageBoxA( NULL, (LPCSTR)angle_buffer, "Bad Angle", MB_OK);
#endif
			}
			break;
		case 45:
			if(((int)m_droneHeading > 22) && ((int)m_droneHeading <= 67))
			{
				//MessageBox( NULL, (LPCSTR)angle_buffer, "Good Angle", MB_OK);
			}
			else
			{
				// add location of Coord2 to the invalid tile list
				invalidTile.x = Coord2.x;
				invalidTile.y = Coord2.y;;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				invalidHeading++;
#if DBG_INVALID_INITIAL_UAV_HEADING
				MessageBoxA( NULL, (LPCSTR)angle_buffer, "Bad Angle", MB_OK);
#endif
			}

			break;
		case 90:
			if(((int)m_droneHeading > 67) && ((int)m_droneHeading <= 112))
			{
				//MessageBox( NULL, (LPCSTR)angle_buffer, "Good Angle", MB_OK);
			}
			else
			{
				// add location of Coord2 to the invalid tile list
				invalidTile.x = Coord2.x;
				invalidTile.y = Coord2.y;;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				invalidHeading++;
#if DBG_INVALID_INITIAL_UAV_HEADING
				MessageBoxA( NULL, (LPCSTR)angle_buffer, "Bad Angle", MB_OK);
#endif
			}

			break;
		case 135:
			if(((int)m_droneHeading > 112) && ((int)m_droneHeading <= 157))
			{
				//MessageBox( NULL, (LPCSTR)angle_buffer, "Good Angle", MB_OK);
			}
			else
			{
				// add location of Coord2 to the invalid tile list
				invalidTile.x = Coord2.x;
				invalidTile.y = Coord2.y;;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				invalidHeading++;
#if DBG_INVALID_INITIAL_UAV_HEADING
				MessageBoxA( NULL, (LPCSTR)angle_buffer, "Bad Angle", MB_OK);
#endif
			}

			break;
		case 180:
			if(((int)m_droneHeading > 157) && ((int)m_droneHeading <= 202))
			{
				//MessageBox( NULL, (LPCSTR)angle_buffer, "Good Angle", MB_OK);
			}
			else
			{
				// add location of Coord2 to the invalid tile list
				invalidTile.x = Coord2.x;
				invalidTile.y = Coord2.y;;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				invalidHeading++;
#if DBG_INVALID_INITIAL_UAV_HEADING
				MessageBoxA( NULL, (LPCSTR)angle_buffer, "Bad Angle", MB_OK);
#endif
			}

			break;
		case 225:
			if(((int)m_droneHeading > 202) && ((int)m_droneHeading <= 247))
			{
				//MessageBox( NULL, (LPCSTR)angle_buffer, "Good Angle", MB_OK);
			}
			else
			{
				// add location of Coord2 to the invalid tile list
				invalidTile.x = Coord2.x;
				invalidTile.y = Coord2.y;;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				invalidHeading++;
#if DBG_INVALID_INITIAL_UAV_HEADING
				MessageBoxA( NULL, (LPCSTR)angle_buffer, "Bad Angle", MB_OK);
#endif
			}

			break;
		case 270:
			if(((int)m_droneHeading > 247) && ((int)m_droneHeading <= 292))
			{
				//MessageBox( NULL, (LPCSTR)angle_buffer, "Good Angle", MB_OK);
			}
			else
			{
				// add location of Coord2 to the invalid tile list
				invalidTile.x = Coord2.x;
				invalidTile.y = Coord2.y;;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				invalidHeading++;
#if DBG_INVALID_INITIAL_UAV_HEADING
				MessageBoxA( NULL, (LPCSTR)angle_buffer, "Bad Angle", MB_OK);
#endif
			}

			break;
		case 315:
			if(((int)m_droneHeading > 292) && ((int)m_droneHeading <= 337))
			{
				//MessageBox( NULL, (LPCSTR)angle_buffer, "Good Angle", MB_OK);
			}
			else
			{
				// add location of Coord2 to the invalid tile list
				invalidTile.x = Coord2.x;
				invalidTile.y = Coord2.y;;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				invalidHeading++;
#if DBG_INVALID_INITIAL_UAV_HEADING
				MessageBoxA( NULL, (LPCSTR)angle_buffer, "Bad Angle", MB_OK);
#endif
			}

			break;
		case 360:
			if(((int)m_droneHeading > 337) && ((int)m_droneHeading <= 360))
			{
				//MessageBox( NULL, (LPCSTR)angle_buffer, "Good Angle", MB_OK);
			}
			else
			{
				// add location of Coord2 to the invalid tile list
				invalidTile.x = Coord2.x;
				invalidTile.y = Coord2.y;;
				invalidTile.h = -1000;
				invalidTile.index = 0;
				invalidTilesList->insertAtBack( invalidTile );
				invalidHeading++;
#if DBG_INVALID_INITIAL_UAV_HEADING
				MessageBoxA( NULL, (LPCSTR)angle_buffer, "Bad Angle", MB_OK);
#endif
			}

			break;
		default:

			break;

		}


/*
if(((int)m_droneHeading >= 0) && ((int)m_droneHeading <= 22))
if(((int)m_droneHeading > 22) && ((int)m_droneHeading <= 67))
if(((int)m_droneHeading > 67) && ((int)m_droneHeading <= 112))
if(((int)m_droneHeading > 112) && ((int)m_droneHeading <= 157))
if(((int)m_droneHeading > 157) && ((int)m_droneHeading <= 202))
if(((int)m_droneHeading > 202) && ((int)m_droneHeading <= 247))
if(((int)m_droneHeading > 247) && ((int)m_droneHeading <= 292))
if(((int)m_droneHeading > 292) && ((int)m_droneHeading <= 337))
if(((int)m_droneHeading > 337) && ((int)m_droneHeading <= 360))
*/
/*
else if( (m_droneHeading >= 0) && (m_droneHeading <= 22.5) )
else if((m_droneHeading > 22.5) && (m_droneHeading <= 67.5))
else if((m_droneHeading > 67.5) && (m_droneHeading <= 112.5))
else if((m_droneHeading > 112.5) && (m_droneHeading <= 157.5))
else if((m_droneHeading > 157.5) && (m_droneHeading <= 202.5))
else if((m_droneHeading > 202.5) && (m_droneHeading <= 247.5))
else if((m_droneHeading > 247.5) && (m_droneHeading <= 292.5))
else if((m_droneHeading > 292.5) && (m_droneHeading <= 337.5))
else if((m_droneHeading > 337.5) && (m_droneHeading <= 360))
*/
	return invalidHeading;
}
