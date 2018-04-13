#include <iostream>

#include "MFCApplication1Doc.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;

 	//April 13, 2018
	CMFCApplication1Doc manny_MFC_AstarObject;

	//April 13, 2018 - Call the below to empty out data structures, but only if you know data has been loaded
	manny_MFC_AstarObject.OnNewDocument();

	//April 13, 2018
	manny_MFC_AstarObject.OnFiletypeHeights();

	//4/13/2018
	const char* HeightFileLocation = "C:\\Users\\sotomanu\\Desktop\\2018\\Repos\\A_Star\\Elevation_Files\\NED_7840.uav.elev";
	//4/13/2018//CStringA strHeightFileName(HeightFileLocation);

	manny_MFC_AstarObject.Serialize(HeightFileLocation);


    return 0;
}
