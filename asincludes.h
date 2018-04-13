
//////////////////////////////////////////////////////////////////
// Class:	CAStar class (10/7/2002)
// File:	AsIncludes.h
// Author:	James Matthews
//
// Include file for A* Explorer
//
// Please visit http://www.generation5.org/ for the latest
// in Artificial Intelligence news, interviews, articles and
// discussion forums.
//

#ifndef _ASINCLUDES_H_
#define _ASINCLUDES_H_

/////////////////////////////////////////////////
//      V A R I O U S     D E F I N E S        //
/////////////////////////////////////////////////

#define ASE_VERSION 200

#ifndef NULL
#define NULL 0
#endif

#define ASNL_ADDOPEN		0
#define ASNL_STARTOPEN		1
#define ASNL_DELETEOPEN		2
#define ASNL_ADDCLOSED		3

#define ASNC_INITIALADD		0
#define ASNC_OPENADD_UP		1
#define ASNC_OPENADD		2
#define ASNC_CLOSEDADD_UP	3
#define ASNC_CLOSEDADD		4
#define ASNC_NEWADD			5
#define MAX_UNSUCCESSFUL_SPAWNINGS 8	//3-24-2007

#ifndef _ASNODE_DEFINED
#define _ASNODE_DEFINED
class _asNode {
	public:
		_asNode(int a = -1,int b = -1) : x(a), y(b), number(0), numchildren(0) {
			parent = next = NULL; dataptr = NULL;
			memset(children, 0, sizeof(children));
		}

		double		f,g,h;			// Fitness, goal, heuristic.
		int			x,y;			// Coordinate position
		int			numchildren;
		int			number;			// x*m_iRows+y
		_asNode		*parent;
		_asNode		*next;			// For Open and Closed lists
		_asNode		*children[8];	// Assumes square tiles
		void		*dataptr;		// Associated data
};
#endif

// Stack for propagation. FIXME: Change to STL?

struct _asStack {
	_asNode	 *data;
	_asStack *next;
};

/////////////////////////////////////////////////
//    F U N C T I O N    P R O T O T Y P E S   //
//                                             //
// These are the two function prototypes used  //
// within CAStar _asFuncB, _asFuncC are used   //
// within for the DLL extensions but not       //
// CAStar, so are not defined here.            //
//                                             //
/////////////////////////////////////////////////

typedef int(*_asFunc)(_asNode *, _asNode *, int, void *);
typedef double(*_asFuncD)(_asNode *, _asNode *, int, void *);

#endif
