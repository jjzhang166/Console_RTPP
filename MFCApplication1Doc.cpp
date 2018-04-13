
// MFCApplication1Doc.cpp : implementation of the CMFCApplication1Doc class
//

//Apr 12, 2018//#include "stdafx.h"

#include "math.h"
#include "MFCApplication1Doc.h"

//Apr 12, 2018//#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMFCApplication1Doc construction/destruction

CMFCApplication1Doc::CMFCApplication1Doc()
: m_xyhBoundariesInFt_flag(false)
, m_xyhHeightsInFtList_flag(false)
, m_xyPathList_flag(false)
, m_NodesExpanded_flag(false)
, m_ClearPathOnly_flag(false)
, m_UAV_MSL(10000.0)
, m_SavePath_flag(false)
, m_RealTimeMode_flag(false)
, m_TimerIsOn_flag(false)
, m_getPathsAttempted(41)//41)//33)//0)//16)//0)//9)//49)
, m_AllowDiagonals_flag(true)
, m_UAV_groundSpeed(0)
, m_UAV_turnRadius(0)
, m_ClearStartAndGoalPos_flag(false)
, m_PathHasDiscontinuities(true)
, m_NumDiscardedFlightPatterns(0)
, m_UAV_StartHeading(-1)
//, m_PathHeadedInWrongDirection(true)
, m_IsVaidPath(false)
, m_xyhDFCS_SitesInFt_flag(false)
, m_UAV_GoalHeading(-1)
, m_SuccessfulFlightPatterns_Cnt(0)
{
	// TODO: add one-time construction code here
	m_MapForAStar.m_Array = NULL;
	m_startPos.index = 0;
	m_goalPos.index = 0;
	m_startPosDoor.index = 0;
	m_goalPosDoor.index = 0;

	m_RealTimeMode_flag = false; //April 3, 2018 SetRealTimeMode();

}

CMFCApplication1Doc::~CMFCApplication1Doc()
{
}

BOOL CMFCApplication1Doc::OnNewDocument()
{
	//Apr 12, 2018//if (!CDocument::OnNewDocument()) return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	if ((m_ClearPathOnly_flag == false) && (m_ClearStartAndGoalPos_flag == true))
	{	// leave the path  intact but clear the start and goal
		//////////////////////////////////////////////////////////
		ListNode< DFCS_Coord > *invldTile_currentPtr;
		DFCS_Coord invldTile_dataGetter;
		if (!(m_xyhInvalidTilesInFtList.isEmpty()))
		{
			while (!(m_xyhInvalidTilesInFtList.isEmpty()))
			{
				invldTile_currentPtr = m_xyhInvalidTilesInFtList.getFirtstPtr();//invalidTilesList->getFirtstPtr();
				invldTile_dataGetter = invldTile_currentPtr->getData();
				m_xyhInvalidTilesInFtList.removeFromFront(invldTile_dataGetter);//invalidTilesList->getNextPtr(invldTile_currentPtr);
			}
		}
		//////////////////////////////////////////////////////////
		if (!(m_xyhStartPositionInvalidTiles.isEmpty()))
		{
			while (!(m_xyhStartPositionInvalidTiles.isEmpty()))
			{
				invldTile_currentPtr = m_xyhStartPositionInvalidTiles.getFirtstPtr();//invalidTilesList->getFirtstPtr();
				invldTile_dataGetter = invldTile_currentPtr->getData();
				m_xyhStartPositionInvalidTiles.removeFromFront(invldTile_dataGetter);//invalidTilesList->getNextPtr(invldTile_currentPtr);
			}
		}
		//////////////////////////////////////////////////////////
		if (!(m_xyhGoalPositionInvalidTiles.isEmpty()))
		{
			while (!(m_xyhGoalPositionInvalidTiles.isEmpty()))
			{
				invldTile_currentPtr = m_xyhGoalPositionInvalidTiles.getFirtstPtr();//invalidTilesList->getFirtstPtr();
				invldTile_dataGetter = invldTile_currentPtr->getData();
				m_xyhGoalPositionInvalidTiles.removeFromFront(invldTile_dataGetter);//invalidTilesList->getNextPtr(invldTile_currentPtr);
			}
		}
		//////////////////////////////////////////////////////////
		m_startPos.index = 0;
		m_goalPos.index = 0;
		m_startPosDoor.index = 0;
		m_goalPosDoor.index = 0;
		m_NumDiscardedFlightPatterns = 0;
		m_ClearStartAndGoalPos_flag = false;//3-14-2007 Very Necessary

		return true;
	}
	m_NodesExpanded_flag = false;

	// For cleaning the fpatList
	ListNode< DFCS_FPatSegment > *currentFpatPtr;
	DFCS_FPatSegment fpatDataGetter;
	if (!(m_fpatList.isEmpty()))
	{
		while (!m_fpatList.isEmpty())
		{
			currentFpatPtr = m_fpatList.getFirtstPtr();
			fpatDataGetter = currentFpatPtr->getData();	// Store the data of the begining point of the DFCS Segment
			m_fpatList.removeFromFront(fpatDataGetter);	// Removing the point to get access to the end point which contains a discontinuity
		}
	}

	// For cleaning the segList
	ListNode< DFCS_Vector > *currentSegPtr;
	DFCS_Vector segDataGetter;
	if (!(m_WaypointList.isEmpty()))
	{
		while (!m_WaypointList.isEmpty())
		{
			currentSegPtr = m_WaypointList.getFirtstPtr();
			segDataGetter = currentSegPtr->getData();	// Store the data of the begining point of the DFCS Segment
			m_WaypointList.removeFromFront(segDataGetter);	// Removing the point to get access to the end point which contains a discontinuity
		}
	}

	// For cleaning the vecList	:  The cleaning of this linked list has not been empirically tested
	// since the processing removes the elements list.  It was only added as an ensurance.
	ListNode< DFCS_Vector > *currentVecPtr;
	DFCS_Vector vecDataGetter;
	if (!(m_VecList.isEmpty()))
	{
		while (!m_VecList.isEmpty())
		{
			currentVecPtr = m_VecList.getFirtstPtr();
			vecDataGetter = currentVecPtr->getData();	// Store the data of the begining point of the DFCS Segment
			m_VecList.removeFromFront(vecDataGetter);	// Removing the point to get access to the end point which contains a discontinuity
		}
	}

	// For cleaning the pthList
	ListNode< DFCS_Coord > *currentPthPtr;
	DFCS_Coord pthDataGetter;
	if (!(m_xyPathList.isEmpty()))
	{
		while (!(m_xyPathList.isEmpty()))
		{
			currentPthPtr = m_xyPathList.getFirtstPtr();
			pthDataGetter = currentPthPtr->getData();
			m_xyPathList.removeFromFront(pthDataGetter);
		}
		m_xyPathList_flag = true;
	}

	//if( (m_ClearPathOnly_flag == true) && (m_ClearStartAndGoalPos_flag == false))

	if ((m_ClearPathOnly_flag == true) && (m_ClearStartAndGoalPos_flag == true))
	{	// we want new path, new start and goal, and current map
		// m_xyhInvalidTilesInFtList must be cleaned here since the iterations for looking
		// for a flyable path are over, and they don't apply to the new start and goal.

		//////////////////////////////////////////////////////////
		ListNode< DFCS_Coord > *invldTile_currentPtr;
		DFCS_Coord invldTile_dataGetter;
		if (!(m_xyhInvalidTilesInFtList.isEmpty()))
		{
			while (!(m_xyhInvalidTilesInFtList.isEmpty()))
			{
				invldTile_currentPtr = m_xyhInvalidTilesInFtList.getFirtstPtr();//invalidTilesList->getFirtstPtr();
				invldTile_dataGetter = invldTile_currentPtr->getData();
				m_xyhInvalidTilesInFtList.removeFromFront(invldTile_dataGetter);//invalidTilesList->getNextPtr(invldTile_currentPtr);
			}
		}
		//////////////////////////////////////////////////////////
		if (!(m_xyhStartPositionInvalidTiles.isEmpty()))
		{
			while (!(m_xyhStartPositionInvalidTiles.isEmpty()))
			{
				invldTile_currentPtr = m_xyhStartPositionInvalidTiles.getFirtstPtr();//invalidTilesList->getFirtstPtr();
				invldTile_dataGetter = invldTile_currentPtr->getData();
				m_xyhStartPositionInvalidTiles.removeFromFront(invldTile_dataGetter);//invalidTilesList->getNextPtr(invldTile_currentPtr);
			}
		}
		//////////////////////////////////////////////////////////
		if (!(m_xyhGoalPositionInvalidTiles.isEmpty()))
		{
			while (!(m_xyhGoalPositionInvalidTiles.isEmpty()))
			{
				invldTile_currentPtr = m_xyhGoalPositionInvalidTiles.getFirtstPtr();//invalidTilesList->getFirtstPtr();
				invldTile_dataGetter = invldTile_currentPtr->getData();
				m_xyhGoalPositionInvalidTiles.removeFromFront(invldTile_dataGetter);//invalidTilesList->getNextPtr(invldTile_currentPtr);
			}
		}
		m_xyPathList_flag = true;  //3-28-2007 This may cause a RTPP crash
		//////////////////////////////////////////////////////////
		m_startPos.index = 0;
		m_goalPos.index = 0;
		m_startPosDoor.index = 0;
		m_goalPosDoor.index = 0;
		m_NumDiscardedFlightPatterns = 0;
		m_ClearStartAndGoalPos_flag = false;//3-14-2007 Very Necessary
		return true;
	}
	else if ((m_ClearPathOnly_flag == true) && (m_ClearStartAndGoalPos_flag == false))
	{	// we want new path, the current start and goal, with the existing map
		// the new path is needed due to a discontinuity in the flight pattern
		// so don't clean the m_xyhInvalidTilesInFtList
		return true;
	}
	else
	{	// we want new path, new start and goal, new map
		// m_xyhInvalidTilesInFtList must be cleaned here since the iterations for looking
		// for a flyable path are over, and they don't apply to the new start and goal.
		m_startPos.index = 0;
		m_goalPos.index = 0;
		m_startPosDoor.index = 0;
		m_goalPosDoor.index = 0;
		m_NumDiscardedFlightPatterns = 0;
		m_ClearStartAndGoalPos_flag = false;//3-14-2007 Very Necessary
		//////////////////////////////////////////////////////////
		ListNode< DFCS_Coord > *invldTile_currentPtr;
		DFCS_Coord invldTile_dataGetter;
		if (!(m_xyhInvalidTilesInFtList.isEmpty()))
		{
			while (!(m_xyhInvalidTilesInFtList.isEmpty()))
			{
				invldTile_currentPtr = m_xyhInvalidTilesInFtList.getFirtstPtr();//invalidTilesList->getFirtstPtr();
				invldTile_dataGetter = invldTile_currentPtr->getData();
				m_xyhInvalidTilesInFtList.removeFromFront(invldTile_dataGetter);//invalidTilesList->getNextPtr(invldTile_currentPtr);
			}
		}
		//////////////////////////////////////////////////////////
		if (!(m_xyhStartPositionInvalidTiles.isEmpty()))
		{
			while (!(m_xyhStartPositionInvalidTiles.isEmpty()))
			{
				invldTile_currentPtr = m_xyhStartPositionInvalidTiles.getFirtstPtr();//invalidTilesList->getFirtstPtr();
				invldTile_dataGetter = invldTile_currentPtr->getData();
				m_xyhStartPositionInvalidTiles.removeFromFront(invldTile_dataGetter);//invalidTilesList->getNextPtr(invldTile_currentPtr);
			}
		}
		//////////////////////////////////////////////////////////
		if (!(m_xyhGoalPositionInvalidTiles.isEmpty()))
		{
			while (!(m_xyhGoalPositionInvalidTiles.isEmpty()))
			{
				invldTile_currentPtr = m_xyhGoalPositionInvalidTiles.getFirtstPtr();//invalidTilesList->getFirtstPtr();
				invldTile_dataGetter = invldTile_currentPtr->getData();
				m_xyhGoalPositionInvalidTiles.removeFromFront(invldTile_dataGetter);//invalidTilesList->getNextPtr(invldTile_currentPtr);
			}
		}
		//////////////////////////////////////////////////////////
	}


	// Cleaning the map
	ListNode< DFCS_Coord > *currentMapPtr;
	DFCS_Coord mapDataGetter;
	if (!(m_xyhBoundariesInFtList.isEmpty()))
	{
		while (!(m_xyhBoundariesInFtList.isEmpty()))
		{
			currentMapPtr = m_xyhBoundariesInFtList.getFirtstPtr();
			mapDataGetter = currentMapPtr->getData();
			m_xyhBoundariesInFtList.removeFromFront(mapDataGetter);
		}
		m_xyhBoundariesInFt_flag = true;
		m_in_B_xyhFile.Reset();
	}
	//3-26-2007
	if (!(m_xyhDFCS_SitesInFtList.isEmpty()))
	{
		while (!(m_xyhDFCS_SitesInFtList.isEmpty()))
		{
			currentMapPtr = m_xyhDFCS_SitesInFtList.getFirtstPtr();
			mapDataGetter = currentMapPtr->getData();
			m_xyhDFCS_SitesInFtList.removeFromFront(mapDataGetter);
		}
		m_xyhDFCS_SitesInFt_flag = true;
		m_in_DFCS_Sites_xyhFile.Reset();
	}
	//3-26-2007
	if (!(m_xyhHeightsInFtList.isEmpty()))
	{
		while (!(m_xyhHeightsInFtList.isEmpty()))
		{
			currentMapPtr = m_xyhHeightsInFtList.getFirtstPtr();
			mapDataGetter = currentMapPtr->getData();
			m_xyhHeightsInFtList.removeFromFront(mapDataGetter);
		}
		m_xyhHeightsInFtList_flag = true;
		m_in_H_xyhFile.Reset();
	}
	m_MapForAStar.ClearSmartArray();
	m_MapForAStar.m_Array = NULL;
	m_UAV_turnRadius = 0.0;
	m_UAV_groundSpeed = 0.0;

	return TRUE;
}




// CMFCApplication1Doc serialization
//const char* xyhFileName
//4/13/2018//void CMFCApplication1Doc::Serialize(CStringA ar_m_strFileName)
void CMFCApplication1Doc::Serialize(const char* ar_m_strFileName)
{

	//Added this on 4/2/2018
	//CStringA ar_m_strFileName(ar.m_strFileName);



	if (false)
	{
		if (m_xyPathList.isEmpty())
		{
			//Apr 12, 2018//MessageBox(NULL, _T("No path data to save."), _T("Path Not Saved"), MB_ICONEXCLAMATION);
		}
		else
		{
			//4/13/2018//ar_m_strFileName += ".pth";
			m_out_Path_xyhFile.Path_xyhCoord_inFeet_Save(&m_xyPathList, ar_m_strFileName);
			//MessageBox(NULL, ar_m_strFileName, _T("Path File Saved"), MB_OK);
		}
	}
	else
	{
		if (m_xyhHeightsInFtList_flag == true)
		{
			if (m_xyhHeightsInFtList.isEmpty())
			{
				m_in_H_xyhFile.xyh_FileReader(&m_xyhHeightsInFtList, ar_m_strFileName, true);
				m_MapForAStar.Map_xyhCoord_inFeet_Generator(m_in_H_xyhFile.m_xmin, m_in_H_xyhFile.m_xmax,
					m_in_H_xyhFile.m_ymin, m_in_H_xyhFile.m_ymax, m_in_H_xyhFile.m_step);
				m_MapForAStar.SetHeightUsingList(&m_xyhHeightsInFtList);
#if DBG_OPENHEIGHTSFILE_MSGBOX
				MessageBoxA(NULL, ar_m_strFileName, "Open Heights File", MB_OK);
#endif
			}
			else
			{
#if DBG_OPENHEIGHTSFILE_MSGBOX
				MessageBoxA(NULL, "Error: Heights data is already loaded.", "Open Heights File Error", MB_ICONEXCLAMATION);
#endif
			}
		}
		else if (m_xyhBoundariesInFt_flag == true)
		{
			if (m_xyhBoundariesInFtList.isEmpty())
			{
				m_in_B_xyhFile.xyh_FileReader(&m_xyhBoundariesInFtList, ar_m_strFileName, false);
				//MessageBox(NULL, ar.m_strFileName, _T("Open Boundaries File"), MB_OK);
			}
			else
			{
				//Apr 12, 2018//MessageBox(NULL, _T("Error: Boundaries data is already loaded."), _T("Open Boundaries File Error"), MB_ICONEXCLAMATION);
			}

		}
		else if (m_xyPathList_flag == true)
		{
			if (m_xyPathList.isEmpty())
			{
				m_out_Path_xyhFile.xyh_FileReader(&m_xyPathList, ar_m_strFileName, false);
				//MessageBox(NULL, ar.m_strFileName, _T("Open Path File"), MB_OK);
			}
			else
			{
				//Apr 12, 2018//MessageBox(NULL, _T("Error: Path data is already loaded."), _T("Open Path File Error"), MB_ICONEXCLAMATION);
			}
		}
		//3-26-2007
		else if (m_xyhDFCS_SitesInFt_flag == true)
		{
			if (m_xyhDFCS_SitesInFtList.isEmpty())
			{
				m_in_DFCS_Sites_xyhFile.xyh_FileReader(&m_xyhDFCS_SitesInFtList, ar_m_strFileName, false);
				//MessageBox(NULL, ar.m_strFileName, _T("Open DFCS Sites File"), MB_OK);
			}
			else
			{
				//Apr 12, 2018//MessageBox(NULL, _T("Error: DFCS sites data is already loaded."), _T("Open DFCS Sites File Error"), MB_ICONEXCLAMATION);
			}

		}
		//3-26-2007



		else
		{
			//Apr 12, 2018//MessageBox(NULL, _T("No data was loaded or saved.\nThe data, e.g. boundaries, heights, and/or path was not set/stored."), _T("File Error"), MB_ICONEXCLAMATION);
			//if( !m_xyPathList.isEmpty() )
			//{
			//	m_out_Path_xyhFile.Path_xyhCoord_inFeet_Save(&m_xyPathList, ar.m_strFileName);
			//	MessageBox(NULL, ar.m_strFileName, "Path File Saved", MB_OK);
			//}
			//else
			//{
			//	MessageBox(NULL, "No data was loaded or saved.\nThe data, e.g. boundaries, heights, and/or path was not set/stored.", "File Error", MB_ICONEXCLAMATION);
			//	//exit(0);
			//}
		}

		//ar.m_strFileName;
		// TODO: add loading code here
	}
}


// CMFCApplication1Doc diagnostics


/*
//Apr 12, 2018//
#ifdef _DEBUG
void CMFCApplication1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCApplication1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
*/

// CMFCApplication1Doc commands


/*  April 2, 2018

void CMFCApplication1Doc::OnFiletypeBoundaries()
{
// TODO: Add your command handler code here
m_xyhBoundariesInFt_flag = true;
m_xyhHeightsInFtList_flag = false;
m_xyPathList_flag = false;
m_xyhDFCS_SitesInFt_flag = false;
}

void CMFCApplication1Doc::OnUpdateFiletypeBoundaries(CCmdUI *pCmdUI)
{
// TODO: Add your command update UI handler code here
pCmdUI->SetCheck( m_xyhBoundariesInFt_flag == true);
}
*/

//April 3, 2018
void CMFCApplication1Doc::OnFiletypeHeights()
{
// TODO: Add your command handler code here
m_xyhHeightsInFtList_flag = true;
m_xyhBoundariesInFt_flag = false;
m_xyPathList_flag = false;
m_xyhDFCS_SitesInFt_flag = false;
}


void CMFCApplication1Doc::MannyGetPath()
{
	if (!(m_xyPathList.isEmpty()))
	{
		//Apr 12, 2018//MessageBox(NULL, _T("The path list is not empty."), _T("Path List Error"), MB_ICONERROR);
		return;
	}

	int indexerForAStar = 0;
	int x_size = m_MapForAStar.get_Array_X_size();
	int y_size = m_MapForAStar.get_Array_Y_size();

	if (x_size < 1 || y_size < 1)
	{
		//Apr 12, 2018//MessageBox(NULL, _T("The map for the search engines is empty."), _T("Empty Map Error"), MB_ICONERROR);
		return;
	}

	if (x_size > y_size)
		indexerForAStar = x_size;
	else
		indexerForAStar = y_size;

	m_A_Star.SetRows(indexerForAStar);
	m_A_Star.SetStepDistance(m_in_H_xyhFile.m_step);
	m_fpat_Gen.SetStepDistance(m_in_H_xyhFile.m_step);
	m_fpat_Gen.SetDroneMSL(m_UAV_MSL);


	// TODO: Add your command handler code here
	if (m_startPos.index < 1 || m_goalPos.index < 1)
	{
		//Apr 12, 2018//MessageBox(NULL, _T("No start/destination position."), _T("Start/Destination Position Error"), MB_ICONERROR);
		return;
	}

	if ((m_UAV_turnRadius >= m_in_H_xyhFile.m_step) || ((int)m_UAV_turnRadius == 0))
	{
		//Apr 12, 2018//MessageBox(NULL, _T("The turn radius is out of range.\nChange the elevation file or the UAV's ground speed."), _T("Turn Radius Unrealistic"), MB_ICONERROR);
		return;
	}

	m_A_Star.udValid = AS_Valid;
	m_A_Star.udCost = AS_Cost;
	m_A_Star.udDistance = AS_Distance;
	m_A_Star.m_pCBData = reinterpret_cast<void *>(this);

	//if (m_A_Star.GeneratePath((int)m_startPos.x, (int)m_startPos.y,
	//	(int)m_goalPos.x, (int)m_goalPos.y))
	m_IsVaidPath = (m_A_Star.GeneratePath((int)m_startPos.x, (int)m_startPos.y,
		(int)m_goalPos.x, (int)m_goalPos.y));

	if (m_IsVaidPath == true)
	{
#if DBG_PATHFOUND_MSGBOX
		MessageBoxA(NULL, "A path has been found.", "Path Found", MB_OK);
#endif
		m_xyPathList_flag = true;

		// The following is for drawing and storing the path to a file
		_asNode *best = m_A_Star.GetBestNode();
		DFCS_Coord PathBuffer;

		while (best)
		{
			PathBuffer = *(m_MapForAStar.QueryArrayUsingWorld_Coord((int)best->x, (int)best->y));

			m_xyPathList.insertAtBack(PathBuffer);

			best = best->parent;
		}

#if 0
		//char fpat_id[25];
		int r = 10;
		char vec_buffer[65];
		_itoa(m_getPathsAttempted, vec_buffer, r);
		strcat(vec_buffer, ".uav.vec");
		char seg_buffer[65];
		_itoa(m_getPathsAttempted, seg_buffer, r);
		strcat(seg_buffer, ".uav.seg");
		char fpat_buffer[65];
		_itoa(m_getPathsAttempted, fpat_buffer, r);
		strcat(fpat_buffer, ".uav.fpat");

		//MessageBox(NULL, (LPCSTR)nodes_buffer, "Total Grid Points", MB_OK);
		m_fpat_Gen.GetVectorListFromPathList(&m_xyPathList, &m_VecList, vec_buffer);
		//New 3-1-2007
		m_fpat_Gen.GetInvalidTilesListFromVectorList(&m_VecList, &m_xyhInvalidTilesInFtList, m_startPosDoor, m_goalPosDoor);
		//End New 3-1-2007
		m_fpat_Gen.GetWaypointListFromVectorList(&m_VecList, &m_WaypointList, seg_buffer);
		m_fpat_Gen.GetFPatListFromWaypointList(&m_WaypointList, &m_fpatList);
		m_fpat_Gen.GetFPatFileFromFPatList(&m_fpatList, fpat_buffer);
#else
		if (false)//m_PathHeadedInWrongDirection == true )
		{
		}
		else
		{
			m_fpat_Gen.GetVectorListFromPathList(&m_xyPathList, &m_VecList);
			int discontinuities = 0;
			m_PathHasDiscontinuities = true;
			discontinuities = m_fpat_Gen.GetInvalidTilesListFromVectorList(&m_VecList, &m_xyhInvalidTilesInFtList, m_startPosDoor, m_goalPosDoor);
			++m_NumDiscardedFlightPatterns;
			if (discontinuities == 1)
			{
				//MessageBox(NULL,"Discontinuity in flight pattern detected.", "Discontinuity Notice", MB_ICONERROR );
				m_PathHasDiscontinuities = true;
			}
			else if (discontinuities == 0)
			{
				++m_SuccessfulFlightPatterns_Cnt;
				char fpat_buffer[65];
				//Apr 12, 2018//_itoa_s(m_SuccessfulFlightPatterns_Cnt, fpat_buffer, 10); //4/2/2018
				//Apr 12, 2018//strcat_s(fpat_buffer, ".uav.fpat");	//4/2/2018
#if 0
				MessageBox(NULL, (LPCTSTR)fpat_buffer, "Successful Flight Pattern", MB_OK);
#endif
				m_PathHasDiscontinuities = false;
				//Following three lines added on 4-2-2007
				m_fpat_Gen.GetWaypointListFromVectorList(&m_VecList, &m_WaypointList);
				m_fpat_Gen.GetFPatListFromWaypointList(&m_WaypointList, &m_fpatList);
				m_fpat_Gen.GetFPatFileFromFPatList(&m_fpatList, fpat_buffer);
			}
			else
			{
				//Apr 12, 2018//MessageBox(NULL, _T("Undefined error in flight pattern."), _T("Undefined Discontinuity Error"), MB_ICONERROR);
				m_PathHasDiscontinuities = true;
			}

			//Commented on 4-2-2007
			//m_fpat_Gen.GetWaypointListFromVectorList(&m_VecList, &m_WaypointList );
			//m_fpat_Gen.GetFPatListFromWaypointList( &m_WaypointList, &m_fpatList );
			//m_fpat_Gen.GetFPatFileFromFPatList(&m_fpatList, "uav.fpat" );
		}
#endif


		//Apr 12, 2018//UpdateAllViews(NULL);

	}
	else
	{
		//m_PathHasDiscontinuities = false;
		//m_PathHeadedInWrongDirection = true;
#if DBG_INVALID_FLIGHT_TRAJECTORY_MSGBOX
		MessageBox(NULL, _T("Flight Trajectory Error: A error occured in obtaining a valid path."), _T("Invalid Path Error"), MB_ICONERROR);
#endif
	}
}

double CMFCApplication1Doc::AS_Cost(_asNode* parent, _asNode* node, int data, void* pointer)
{
	//MannySoto: I don't want any cost associated yet
	// I could have set cost to zero by doing "int cost = 0
	// but I left the following node that way so that the map from the
	// smart array can be queried that way.
	CMFCApplication1Doc *me = reinterpret_cast<CMFCApplication1Doc *>(pointer);

	//int cost = me->m_debug_clrGrid[node->x][node->y];	// Ensure always cost > 1
	DFCS_Coord DbgBuffer = *(me->m_MapForAStar.QueryArrayUsingWorld_Coord(
		(int)node->x, (int)node->y));
	//New 2-28-2007
	int get_cost = 0;
	int x = node->x,
		y = node->y;
	int px = parent->x,
		py = parent->y;
	if (me->m_AllowDiagonals_flag == true)
	{
		if (((px - x) != 0) && ((py - y) != 0))
		{
			get_cost = 1;//me->m_in_H_xyhFile.m_step;//return 0;
		}
		else
			get_cost = 1;
	}
	//End New 2-28-2007

	//int cost = me->m_in_H_xyhFile.m_step;//1;//(int)DbgBuffer.h;
	return get_cost;
}
double CMFCApplication1Doc::AS_Distance(_asNode* parent, _asNode* node, int data, void* pointer)
{
	//MannySoto: I don't want any cost associated yet
	// I could have set cost to zero by doing "int cost = 0
	// but I left the following node that way so that the map from the
	// smart array can be queried that way.
	CMFCApplication1Doc *me = reinterpret_cast<CMFCApplication1Doc *>(pointer);

	//int cost = me->m_debug_clrGrid[node->x][node->y];	// Ensure always cost > 1
	DFCS_Coord DbgBuffer = *(me->m_MapForAStar.QueryArrayUsingWorld_Coord(
		(int)node->x, (int)node->y));

	double distance = sqrt((double)((node->x - me->m_goalPos.x)*(node->x - me->m_goalPos.x))
		+ (double)((node->y - me->m_goalPos.y)*(node->y - me->m_goalPos.y)));

	return distance;
}

int CMFCApplication1Doc::AS_Valid(_asNode* parent, _asNode* node, int data, void* pointer)
{
	int x = node->x,
		y = node->y;
	int px = parent->x,
		py = parent->y;

	CMFCApplication1Doc *me = reinterpret_cast<CMFCApplication1Doc *>(pointer);
	double h_plane = me->m_UAV_MSL;


	////m_xyhInvalidTilesInFtList//////////////////////////////////////////////////////
	ListNode< DFCS_Coord > *invldTile_currentPtr;
	DFCS_Coord invldTile_dataGetter;

	////m_xyhStartPositionInvalidTiles//////////////////////////////////////////////////////
	invldTile_currentPtr = me->m_xyhStartPositionInvalidTiles.getFirtstPtr();//invalidTilesList->getFirtstPtr();
	while (invldTile_currentPtr != 0)
	{
		invldTile_dataGetter = invldTile_currentPtr->getData();

		if (((int)node->x == (int)invldTile_dataGetter.x) && ((int)node->y == (int)invldTile_dataGetter.y))
			return 0;
		invldTile_currentPtr = me->m_xyhStartPositionInvalidTiles.getNextPtr(invldTile_currentPtr);//invalidTilesList->getNextPtr(invldTile_currentPtr);
	}
	////m_xyhGoalPositionInvalidTiles//////////////////////////////////////////////////////
	invldTile_currentPtr = me->m_xyhGoalPositionInvalidTiles.getFirtstPtr();//invalidTilesList->getFirtstPtr();
	while (invldTile_currentPtr != 0)
	{
		invldTile_dataGetter = invldTile_currentPtr->getData();

		if (((int)node->x == (int)invldTile_dataGetter.x) && ((int)node->y == (int)invldTile_dataGetter.y))
			return 0;
		invldTile_currentPtr = me->m_xyhGoalPositionInvalidTiles.getNextPtr(invldTile_currentPtr);//invalidTilesList->getNextPtr(invldTile_currentPtr);
	}
	////m_xyhInvalidTilesInFtList//////////////////////////////////////////////////////
	invldTile_currentPtr = me->m_xyhInvalidTilesInFtList.getFirtstPtr();//invalidTilesList->getFirtstPtr();
	while (invldTile_currentPtr != 0)
	{
		invldTile_dataGetter = invldTile_currentPtr->getData();

		if (((int)node->x == (int)invldTile_dataGetter.x) && ((int)node->y == (int)invldTile_dataGetter.y))
			return 0;
		invldTile_currentPtr = me->m_xyhInvalidTilesInFtList.getNextPtr(invldTile_currentPtr);//invalidTilesList->getNextPtr(invldTile_currentPtr);
	}
	//////////////////////////////////////////////////////////


	if (me->m_AllowDiagonals_flag == false)
	{
		if (((px - x) != 0) && ((py - y) != 0))
			return 0;
	}


	if (x < me->m_in_H_xyhFile.m_xmin || y < me->m_in_H_xyhFile.m_ymin || x > me->m_in_H_xyhFile.m_xmax || y > me->m_in_H_xyhFile.m_ymax)
	{
		return 0;
	}

	DFCS_Coord DbgBuffer = *(me->m_MapForAStar.QueryArrayUsingWorld_Coord(
		(int)node->x, (int)node->y));

	if (DbgBuffer.h > h_plane)
	{
		return 0;
	}

	if (!(me->m_UAV_minAGL.MinAGL_Maintained(me->m_UAV_MSL, DbgBuffer.h)))
	{
		return 0;
	}


	static long int m_NodesExpanded;
	++m_NodesExpanded;
	int remainder = m_NodesExpanded % 1000;
	char NodesExpanded_buffer[65];
	int r = 10;

	//Apr 12, 2018//_itoa_s(m_NodesExpanded, NodesExpanded_buffer, r);	//4/2/2018
	//Apr 12, 2018//strcat_s(NodesExpanded_buffer, " nodes expanded. ");	//4/2/2018

	if (remainder == 0 && me->m_NodesExpanded_flag == true); //Apr 12, 2018//
		//Apr 12, 2018//MessageBox(NULL, _T("(LPCSTR)NodesExpanded_buffer"), _T("Nodes Expanded"), MB_OK);


	return 1;

}

/*  April 2, 2018

void CMFCApplication1Doc::OnFileNewpath()
{
// TODO: Add your command handler code here
if( m_ClearPathOnly_flag == true )
{
m_ClearPathOnly_flag = false;
}
else
{
m_ClearPathOnly_flag = true;
}
}

void CMFCApplication1Doc::OnUpdateFileNewpath(CCmdUI *pCmdUI)
{
// TODO: Add your command update UI handler code here
pCmdUI->SetCheck( m_ClearPathOnly_flag == true );
}


void CMFCApplication1Doc::OnFiletypePaths()
{
// TODO: Add your command handler code here
m_xyPathList_flag = true;
m_xyhHeightsInFtList_flag = false;
m_xyhBoundariesInFt_flag = false;
m_xyhDFCS_SitesInFt_flag = false;
}

void CMFCApplication1Doc::OnUpdateFiletypePaths(CCmdUI *pCmdUI)
{
// TODO: Add your command update UI handler code here
pCmdUI->SetCheck( m_xyPathList_flag == true);
}
*/

void CMFCApplication1Doc::UAVFileProcesssing(char* uavFileName, bool Is_Storing)
{

	if (Is_Storing)
	{
		if (m_xyPathList.isEmpty())
		{
			//Apr 12, 2018//MessageBox(NULL, _T("No path data to save."), _T("Path Not Saved"), MB_ICONEXCLAMATION);
		}
		else
		{
			//ar.m_strFileName+=".pth";
			//strcat_s(uavFileName, ".pth");	//4/2/2018
			m_out_Path_xyhFile.Path_xyhCoord_inFeet_Save(&m_xyPathList, uavFileName);// use .pth as a file extension
			//Apr 12, 2018//MessageBox(NULL, _T("uavFileName"), _T("Path File Saved"), MB_OK);
		}
	}
	else
	{
		if (m_xyhHeightsInFtList_flag == true)
		{
			if (m_xyhHeightsInFtList.isEmpty())
			{
				m_in_H_xyhFile.xyh_FileReader(&m_xyhHeightsInFtList, uavFileName, true);
				m_MapForAStar.Map_xyhCoord_inFeet_Generator(m_in_H_xyhFile.m_xmin, m_in_H_xyhFile.m_xmax,
					m_in_H_xyhFile.m_ymin, m_in_H_xyhFile.m_ymax, m_in_H_xyhFile.m_step);
				m_MapForAStar.SetHeightUsingList(&m_xyhHeightsInFtList);
#if DBG_OPENHEIGHTSFILE_MSGBOX
				MessageBoxA(NULL, uavFileName, "Open Heights File", MB_OK);
#endif
			}
			else
			{
#if DBG_OPENHEIGHTSFILE_MSGBOX
				MessageBoxA(NULL, "Error: Heights data is already loaded.", "Open Heights File Error", MB_ICONEXCLAMATION);
#endif
			}
		}
		else if (m_xyhBoundariesInFt_flag == true)
		{
			if (m_xyhBoundariesInFtList.isEmpty())
			{
				m_in_B_xyhFile.xyh_FileReader(&m_xyhBoundariesInFtList, uavFileName, false);
				//Apr 12, 2018//MessageBox(NULL, _T("uavFileName"), _T("Open Boundaries File"), MB_OK);
			}
			else
			{
				//Apr 12, 2018//MessageBox(NULL, _T("Error: Boundaries data is already loaded."), _T("Open Boundaries File Error"), MB_ICONEXCLAMATION);
			}

		}
		else if (m_xyPathList_flag == true)
		{
			if (m_xyPathList.isEmpty())
			{
				m_out_Path_xyhFile.xyh_FileReader(&m_xyPathList, uavFileName, false);
				//Apr 12, 2018//MessageBox(NULL, _T("uavFileName"), _T("Open Path File"), MB_OK);
			}
			else
			{
				//Apr 12, 2018//MessageBox(NULL, _T("Error: Path data is already loaded."), _T("Open Path File Error"), MB_ICONEXCLAMATION);
			}
		}

		//3-26-2007
		else if (m_xyhDFCS_SitesInFt_flag == true)
		{
			if (m_xyhDFCS_SitesInFtList.isEmpty())
			{
				m_in_DFCS_Sites_xyhFile.xyh_FileReader(&m_xyhDFCS_SitesInFtList, uavFileName, false);
				//Apr 12, 2018//MessageBox(NULL, _T("uavFileName"), _T("Open DFCS Sites File"), MB_OK);
			}
			else
			{
				//Apr 12, 2018//MessageBox(NULL, _T("Error: DFCS sites data is already loaded."), _T("Open DFCS Sites File Error"), MB_ICONEXCLAMATION);
			}

		}
		//3-26-2007



		else
		{
			//Apr 12, 2018//MessageBox(NULL, _T("uavFileName"), _T("File Error"), MB_ICONEXCLAMATION);
			//Apr 12, 2018//MessageBox(NULL, _T("No data was loaded or saved.\nThe data, e.g. boundaries, heights, and/or path was not set/stored."), _T("File Error"), MB_ICONEXCLAMATION);
			//if( !m_xyPathList.isEmpty() )
			//{
			//	m_out_Path_xyhFile.Path_xyhCoord_inFeet_Save(&m_xyPathList, ar.m_strFileName);
			//	MessageBox(NULL, ar.m_strFileName, "Path File Saved", MB_OK);
			//}
			//else
			//{
			//	MessageBox(NULL, "No data was loaded or saved.\nThe data, e.g. boundaries, heights, and/or path was not set/stored.", "File Error", MB_ICONEXCLAMATION);
			//	//exit(0);
			//}
		}

		//ar.m_strFileName;
		// TODO: add loading code here
	}
}



int CMFCApplication1Doc::GetFlightPatternInRealTime()
{

	while (m_PathHasDiscontinuities == true)
	{
		MannyGetPath();
		if ((m_PathHasDiscontinuities == true))
		{
			if (m_NumDiscardedFlightPatterns <= 0)
			{
				m_ClearPathOnly_flag = true;	//added on 4-4-2007
				m_ClearStartAndGoalPos_flag = true;	//added on 4-4-2007
				OnNewDocument();	//added on 4-4-2007
				break;
			}

			if (m_IsVaidPath == false)
			{
				m_ClearPathOnly_flag = true;
				m_ClearStartAndGoalPos_flag = true;
				OnNewDocument();
				break;
			}

			m_ClearPathOnly_flag = (true);
			OnNewDocument();
#if DBG_DISCONTINUITY_IN_FL_PAT_DETECTED_MSGBOX
			MessageBoxA(NULL, "Discontinuity in flight pattern detected.", "Discontinuity Notice", MB_ICONERROR);
#endif
		}
		else//( m_PathHasDiscontinuities == false )
		{
#if DBG_DISCONTINUITY_IN_FL_PAT_DETECTED_MSGBOX
			MessageBoxA(NULL, "Detected no discontinuities in flight pattern.", "Successful Flight Pattern", MB_OK);
#endif
		}
	}

	m_PathHasDiscontinuities = true;
	return 0;

}

bool CMFCApplication1Doc::SetRealTimeMode(void)
{
	ifstream in_RealTimeMode("RealTimeMode.uav.rt", ios::in);

	int RealTimeMode = 0;

	if (!in_RealTimeMode)
	{
		cerr << "The system is not in real-time mode." << endl;
		//Apr 12, 2018//MessageBox(NULL, _T("The system is not in real-time mode.  The timer should be stoppeded at this point."), _T("Real Time Settings"), MB_OK);
		return false;
	}

	in_RealTimeMode >> RealTimeMode;

	if (RealTimeMode == 1)
	{
#if DBG_REALTIME_MODE_MSGBOX
		MessageBox(NULL, _T("The system is in real-time mode.  The timer should be started at this point."), _T("Real Time Settings"), MB_OK);
#endif
		return true;
	}
	else
	{
#if DBG_REALTIME_MODE_MSGBOX
		MessageBox(NULL, _T("The system is not in real-time mode.  The timer should be stoppeded at this point."), _T("Real Time Settings"), MB_OK);
#endif
		return false;

	}

}

/*  April 2, 2018

void CMFCApplication1Doc::OnFileNewstartandgoal()
{
// TODO: Add your command handler code here
if( m_ClearStartAndGoalPos_flag == true )
{
m_ClearStartAndGoalPos_flag = false;
m_ClearPathOnly_flag = false;
}
else
{
m_ClearStartAndGoalPos_flag = true;
m_ClearPathOnly_flag = true;
}
}

void CMFCApplication1Doc::OnUpdateFileNewstartandgoal(CCmdUI *pCmdUI)
{
// TODO: Add your command update UI handler code here
pCmdUI->SetCheck( m_ClearStartAndGoalPos_flag == true );
}


void CMFCApplication1Doc::OnGetFlightpattern()
{
// TODO: Add your command handler code here
GetFlightPatternInRealTime();
}

void CMFCApplication1Doc::OnFiletypeDfcssites()
{
// TODO: Add your command handler code here
m_xyhDFCS_SitesInFt_flag = true;
m_xyhBoundariesInFt_flag = false;
m_xyhHeightsInFtList_flag = false;
m_xyPathList_flag = false;
}

void CMFCApplication1Doc::OnUpdateFiletypeDfcssites(CCmdUI *pCmdUI)
{
// TODO: Add your command update UI handler code here
pCmdUI->SetCheck( m_xyhDFCS_SitesInFt_flag == true);
}
*/

//void CMFCApplication1Doc::OnHelpSetheading()
//{
//	// TODO: Add your command handler code here
//m_UAV_StartHeading=20;
//}
