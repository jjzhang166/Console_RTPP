
// MFCApplication1Doc.h : interface of the CMFCApplication1Doc class
//


#pragma once
#include "DFCS_ClassAndStructData.h"
#include "Listnd.h"
#include "List.h"
#include "MapCoordsFileGenerator.h"
#include "SmartArray.h"
#include "pathfinder.h"
#include "MinAGL.h"
#include "FlightPatternGenerator.h"

class CMFCApplication1Doc //Apr 12, 2018//: public CDocument
{
public: // create from serialization only
	CMFCApplication1Doc();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(const char*);

// Implementation
public:
	virtual ~CMFCApplication1Doc();
#ifdef _DEBUG
	//Apr 12, 2018//virtual void AssertValid() const;
	//Apr 12, 2018//virtual void Dump(CDumpContext& dc) const;
#endif


public:
	bool m_xyhBoundariesInFt_flag;
	bool m_xyhHeightsInFtList_flag;
	MapCoordsFileGenerator m_in_H_xyhFile;
	MapCoordsFileGenerator m_in_B_xyhFile;
	MapCoordsFileGenerator m_out_Path_xyhFile;
	MapCoordsFileGenerator m_in_DFCS_Sites_xyhFile;
	List<DFCS_Coord> m_xyhInvalidTilesInFtList;
	List<DFCS_Coord> m_xyhGoalPositionInvalidTiles;	// To ensure correct heading
	List<DFCS_Coord> m_xyhStartPositionInvalidTiles;	// To ensure correct heading
	List<DFCS_Coord> m_xyhBoundariesInFtList;
	List<DFCS_Coord> m_xyhHeightsInFtList;
	List<DFCS_Coord> m_xyPathList;
	List<DFCS_Coord> m_xyhDFCS_SitesInFtList;	// 3-26-2007
	List<DFCS_Vector> m_VecList;	// New List
	List<DFCS_Vector> m_WaypointList;	// New List
	List<DFCS_FPatSegment> m_fpatList;	// New List

	SmartArray m_MapForAStar;
	DFCS_Coord m_startPos;
	DFCS_Coord m_goalPos;
	DFCS_Coord m_startPosDoor;
	DFCS_Coord m_goalPosDoor;
	MinAGL m_UAV_minAGL;

	/*  April 2, 2018
	afx_msg void OnFiletypeBoundaries();
	afx_msg void OnUpdateFiletypeBoundaries(CCmdUI *pCmdUI);
	*/
	void OnFiletypeHeights(); //April 3, 2018

	//	virtual void DeleteContents();

	CAStar m_A_Star;
	//  April 2, 2018 // afx_msg void OnGetPath();
	void MannyGetPath();
	static double AS_Cost(_asNode* parent, _asNode* node, int data, void* pointer);
	static double AS_Distance(_asNode* parent, _asNode* node, int data, void* pointer);
	static int AS_Valid(_asNode* parent, _asNode* node, int data, void* pointer);
	bool m_xyPathList_flag;
	bool m_NodesExpanded_flag;
	//  April 2, 2018 // afx_msg void OnFileNewpath();
	bool m_ClearPathOnly_flag;
	//  April 2, 2018 // afx_msg void OnUpdateFileNewpath(CCmdUI *pCmdUI);
	double m_UAV_MSL;
	bool m_SavePath_flag;
	//  April 2, 2018 // afx_msg void OnFiletypePaths();
	//  April 2, 2018 // afx_msg void OnUpdateFiletypePaths(CCmdUI *pCmdUI);
	void UAVFileProcesssing(char*, bool);
	bool m_RealTimeMode_flag;
	int GetFlightPatternInRealTime();
private:
	bool SetRealTimeMode(void);
public:
	bool m_TimerIsOn_flag;	// Starts the timer from the View class
	int m_getPathsAttempted;
	FlightPatternGenerator m_fpat_Gen;
	bool m_AllowDiagonals_flag;
	double m_UAV_groundSpeed;
	double m_UAV_turnRadius;
	bool m_ClearStartAndGoalPos_flag;
	//  April 2, 2018 // afx_msg void OnFileNewstartandgoal();
	//  April 2, 2018 // afx_msg void OnUpdateFileNewstartandgoal(CCmdUI *pCmdUI);
	bool m_PathHasDiscontinuities;
	int m_NumDiscardedFlightPatterns;
	//  April 2, 2018 // afx_msg void OnGetFlightpattern();
	double m_UAV_StartHeading;
	bool m_IsVaidPath;
	//  April 2, 2018 // afx_msg void OnFiletypeDfcssites();
	//  April 2, 2018 // afx_msg void OnUpdateFiletypeDfcssites(CCmdUI *pCmdUI);
	bool m_xyhDFCS_SitesInFt_flag;
	double m_UAV_GoalHeading;
	int m_SuccessfulFlightPatterns_Cnt;


};
