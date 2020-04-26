#pragma once

#include <EuroScopePlugIn.h>
#include "defines.h"

using namespace std;
using namespace EuroScopePlugIn;

class coordination
{
public:
	coordination()
	{
		id = "\0";
		copnName = "\0";
		copnStatus = false;
		copxName = "\0";
		copxStatus = false;
	}
	coordination(const char* SystemId, const char* COPNNAME, bool COPNSTATUS, const char* COPXNAME, bool COPXSTATUS)
	{
		id = SystemId;
		copnName = COPNNAME;
		copnStatus = COPNSTATUS;
		copxName = COPXNAME;
		copxStatus = COPXSTATUS;
	}

	// GET
	const char* getId()   { return id; }
	const char* getCOPNName()   { return copnName; }
	bool  getCOPNStatus()  { return copnStatus; }
	const char* getCOPXName()   { return copxName; }
	bool  getCOPXStatus() { return copxStatus; }

	// SET
	void setId(const char* SystemId)    { id = SystemId; }
	void setCOPNName(const char* COPNNAME)    { copnName = COPNNAME; }
	void setCOPNStatus(bool COPNStatus) { copnStatus = COPNStatus; }
	void setCOPXName(const char* COPXNAME)    { copxName = COPXNAME; }
	void setCOPXStatus(bool COPXStatus) { copxStatus = COPXStatus; }

private:
	const char* id;

	const char* copnName;
	bool copnStatus;

	const char* copxName;
	bool copxStatus;
};



class tagPlugin : public EuroScopePlugIn::CPlugIn
{
private:
	CList <const char*> aclist;
	CList <coordination> coord;
	
	POSITION findById(const char* SystemId);
	bool isVFR(int squawk);

	bool sectorIndicator;

	

// Construction
public:
	tagPlugin();
	virtual ~tagPlugin();

protected:
// Overrides


	//---OnRadarScreenCreated-------------------------------------
	virtual EuroScopePlugIn::CRadarScreen * OnRadarScreenCreated(const char * sDisplayName,
																 bool NeedRadarContent,
																 bool GeoReferenced,
																 bool CanBeSaved,
															 	 bool CanBeCreated);
	//------------------------------------------------------------------
	//  
	//------------------------------------------------------------------


	//---OnGetTagItem-------------------------------------
	virtual void	OnGetTagItem( EuroScopePlugIn::CFlightPlan FlightPlan, EuroScopePlugIn::CRadarTarget RadarTarget, int ItemCode,
								  int TagData, char sItemString[16], int * pColorCode, COLORREF * pRGB, double * pFontSize);
	//------------------------------------------------------------------
	//  
	//------------------------------------------------------------------


	//---OnFunctionCall-------------------------------------

	virtual void    OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area);
	//------------------------------------------------------------------
	//  
	//------------------------------------------------------------------

};

