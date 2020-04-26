// tagPlugin.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//



#include "stdafx.h"

#include "tagPlugin.h"



//---CPrecisionApproachPlugIn------------------------------------------

tagPlugin::tagPlugin() : CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
									 MY_PLUGIN_NAME,
								 	 MY_PLUGIN_VERSION,
									 MY_PLUGIN_DEVELOPER,
									 MY_PLUGIN_COPYRIGHT)
{
	//------------------------------------------------------------------
	//  INIT:
	//------------------------------------------------------------------
	
	// Tag Item to Display AC Datalink Status
	RegisterTagItemType("Custom Callsign", 1);
	RegisterTagItemType("Custom Communication Type", 2);
	RegisterTagItemType("Custom Aicraft Type", 3);
	RegisterTagItemType("Custom Altitude", 4);
	RegisterTagItemType("Custom Speed", 5);
	RegisterTagItemType("Custom Destination Airport", 6);
	RegisterTagItemType("Custom COPN/COPX", 7);

	RegisterTagItemType("Custom Assigned Heading", 8);
	RegisterTagItemType("Custom Temporary Altitude", 9);
	
	RegisterTagItemType("Custom Sector ID", 10);
	RegisterTagItemType("Custom Vertical Speed Indicator", 11);
	RegisterTagItemType("Custom Vertical Speed", 12);
	RegisterTagItemType("Custom Scratch Pad", 13);
	RegisterTagItemType("Custom Squawk", 14);
	RegisterTagItemType("Custom Mode S Callsign", 15);

	RegisterTagItemFunction("Custom Sector ID Function", 1);

	sectorIndicator = false;
}


tagPlugin::~tagPlugin()
{
}


/////////////////////////////////////////////////////////////////////////////
// EuroScopePlugIn message handlers
//
//---OnRadarScreenCreated----------------------------------------------

EuroScopePlugIn::CRadarScreen * tagPlugin::OnRadarScreenCreated(const char * sDisplayName,
	bool NeedRadarContent,
	bool GeoReferenced,
	bool CanBeSaved,
	bool CanBeCreated)
{
	return NULL;
}


//---OnGetTagItem-----------------------------------------

void	tagPlugin::OnGetTagItem ( EuroScopePlugIn::CFlightPlan FlightPlan, EuroScopePlugIn::CRadarTarget RadarTarget, int ItemCode,
								    int TagData, char sItemString[16], int * pColorCode, COLORREF * pRGB, double * pFontSize )
{
	if (!FlightPlan.IsValid())
		return;
	
	if (FlightPlan.GetTrackingControllerIsMe() && aclist.Find(RadarTarget.GetSystemID()) == NULL)
		aclist.AddTail(RadarTarget.GetSystemID());

	/*

	*/
	*pColorCode = EuroScopePlugIn::TAG_COLOR_DEFAULT;
	int tmpSq = atoi(RadarTarget.GetPosition().GetSquawk());

	// IFR
	//else if (FlightPlan.GetFlightPlanData().GetPlanType()[0] == 'I')
	{
		// HANDOFF to other Station
		if (FlightPlan.GetState() == EuroScopePlugIn::FLIGHT_PLAN_STATE_TRANSFER_FROM_ME_INITIATED)
		{
			// BLACK-MAGENTA
			switch (ItemCode)
			{
			case 1:
			case 4:
			default: *pRGB = RGB(255, 255, 255); break;
			}
		}

		// HANDOFF to me
		else if (FlightPlan.GetState() == EuroScopePlugIn::FLIGHT_PLAN_STATE_TRANSFER_TO_ME_INITIATED)
		{
			// BLACK-BLUE
			switch (ItemCode)
			{
			case 1:
			case 4:
			default: *pRGB = RGB(255, 255, 255); break;
			}
		}

		// AC Currently Tracked
		else if (FlightPlan.GetTrackingControllerIsMe())
		{
			if (FlightPlan.GetSectorExitMinutes() == 0)		
				*pRGB = RGB(255, 255, 150);
			else
			// BLACK
			*pColorCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
		}
		
		// AC Handed Off (Passed Airspace)
		else if (FlightPlan.GetState() == EuroScopePlugIn::FLIGHT_PLAN_STATE_REDUNDANT || aclist.Find(RadarTarget.GetSystemID()) != NULL)
		{
			// MAGENTA
			*pRGB = RGB(0, 110, 220);
		}

		// AC will cross Airspace
		else if (FlightPlan.GetSectorEntryMinutes() >= 0)
		{
			// BLUE			
			*pRGB = RGB(255, 255, 150);
		}

		// AC won't cross Airspace (not concerned)
		else
		{
			// WHITE
			*pColorCode = EuroScopePlugIn::TAG_COLOR_DEFAULT;
		}

	}

	// OVERRIDE
	// COMM FAILURE
	if (strcmp(RadarTarget.GetPosition().GetSquawk(), "7600") == 0)
	{
		// DEFAULT-RED
		switch (ItemCode)
		{
		// ALL except FIRST ROW
		case 1:
		case 2: *pColorCode = EuroScopePlugIn::TAG_COLOR_EMERGENCY; break;
		default: break;
		}
	}

	// EMERGENCY
	else if (strcmp(RadarTarget.GetPosition().GetSquawk(), "7700") == 0)
	{
		// DEFAULT-RED
		switch (ItemCode)
		{
		// ALL except CALLSIGN
		case 1:
		case 2: break;
		default: *pColorCode = EuroScopePlugIn::TAG_COLOR_EMERGENCY; break;
		}
		
	}
	
	if (RadarTarget.GetPosition().GetTransponderI() && ItemCode == 1)
	{
		*pColorCode = EuroScopePlugIn::TAG_COLOR_INFORMATION;
	}

	// Calculate Vertical Speed (for usage below)
	int tmpVS = RadarTarget.GetVerticalSpeed() / 200;
	int tmpInt = FlightPlan.GetControllerAssignedData().GetClearedAltitude();
	char* tmpx = new char[5];

	// Check, which Items 
	switch (ItemCode)
	{
	// CALLSIGN
	case 1: 
		switch (atoi(RadarTarget.GetPosition().GetSquawk()))
		{
		case 0:
			strncpy(sItemString, "0000\0", 15); break;
		case 20:
			strncpy(sItemString, "RESCU\0",15); break;
		case 23:
			strncpy(sItemString, "BPO\0",15); break;
		case 24:
			strncpy(sItemString, "TFFN\0",15); break;
		case 25:
			strncpy(sItemString, "PJE\0",15); break;
		case 27:
			strncpy(sItemString, "ACRO\0",15); break;
		case 30:
			strncpy(sItemString, "CAL\0",15); break;
		case 31:
			strncpy(sItemString, "OPSKY\0",15); break;
		case 33:
			strncpy(sItemString, "VM\0",15); break;
		case 34:
			strncpy(sItemString, "SAR\0",15); break;
		case 35:
			strncpy(sItemString, "AIRCL\0",15); break;
		case 36:
			strncpy(sItemString, "POL\0",15); break;
		case 37:
			strncpy(sItemString, "BIV\0",15); break;
		case 76:
			strncpy(sItemString, "VFRCD\0",15); break;
		case 1200:
			strncpy(sItemString, "1200\0", 15); break;
		case 2000:
			strncpy(sItemString, "2000\0", 15); break;
		case 2200:
			strncpy(sItemString, "2200\0", 15); break;
		case 7000:
			strncpy(sItemString, "V\0",15); break;

		default:
			strncpy(sItemString, FlightPlan.GetCallsign(),15);  break;
		}
		break;

	// SSR Tag
	case 15:
		if (isVFR(atoi(RadarTarget.GetPosition().GetSquawk())))
			strncpy(sItemString, FlightPlan.GetCallsign(), 15);
		else
			strncpy(sItemString, "\0", 15);
		break;

	// COMM TYPE
	case 2:
		switch (FlightPlan.GetControllerAssignedData().GetCommunicationType())
		{
		case 'v': sItemString = '\0'; break;
		case 'r':
		case 't':
				sItemString[0] = '/';
				sItemString[1] = FlightPlan.GetControllerAssignedData().GetCommunicationType();
				sItemString[2] = '\0'; break;
		default:
			switch (FlightPlan.GetFlightPlanData().GetCommunicationType())
			{
			case 'v': sItemString = '\0'; break;
			case 'r':
			case 't':
				sItemString[0] = '/';
				sItemString[1] = FlightPlan.GetFlightPlanData().GetCommunicationType();
				sItemString[2] = '\0'; break;
			default: 
				sItemString[0] = '/';
				sItemString[1] = '?';
				sItemString[2] = '\0'; break;
			}
			break;
		}
		break;

	// AC TYPE 
	case 3: 
		strcpy(sItemString, FlightPlan.GetFlightPlanData().GetAircraftFPType()); 
		break;
	
	// Altitude (FL), Three Digit
	case 4:
		if (RadarTarget.GetPosition().GetFlightLevel() >= 10000)
			// 36000 -> 360
			_itoa(RadarTarget.GetPosition().GetFlightLevel() / 100, sItemString, 10);
		else if (RadarTarget.GetPosition().GetFlightLevel() >= 1000)
		{
			// 8000 -> 080
			char * tmp = new char[5];
			if (RadarTarget.GetPosition().GetPressureAltitude() <= GetTransitionAltitude())
				_itoa(RadarTarget.GetPosition().GetPressureAltitude() / 100, tmp, 10);
			else
				_itoa(RadarTarget.GetPosition().GetFlightLevel() / 100, tmp, 10);

			sItemString[0] = 'A';
			sItemString[1] = tmp[0];
			sItemString[2] = tmp[1];
			sItemString[3] = '\0';
		}
		else if (RadarTarget.GetPosition().GetFlightLevel() >= 100)
		{
			// 300 -> 003
			char * tmp = new char[5];
			if (RadarTarget.GetPosition().GetPressureAltitude() <= GetTransitionAltitude())
				_itoa(RadarTarget.GetPosition().GetPressureAltitude() / 100, tmp, 10);
			else
				_itoa(RadarTarget.GetPosition().GetFlightLevel() / 100, tmp, 10);
			sItemString[0] = 'A';
			sItemString[1] = '0';
			sItemString[2] = tmp[0];
			sItemString[3] = '\0';
		}
		else
		{
			// 52 -> 000
			sItemString[0] = '0';
			sItemString[1] = '0';
			sItemString[2] = '0';
			sItemString[3] = '\0';
		}
		break;

	// SPEED (+Assigned Speed)
	case 5: 
		if ((FlightPlan.GetFlightPlanData().GetPlanType()[0] != 'I' || FlightPlan.GetState() == EuroScopePlugIn::FLIGHT_PLAN_STATE_NON_CONCERNED) || FlightPlan.GetControllerAssignedData().GetAssignedSpeed() == 0)
		{
			if (RadarTarget.GetGS() / 10 < 10)
			{
				char * tmp = new char[5];
				_itoa(RadarTarget.GetGS() / 10, tmp, 10);
				strcat(sItemString, "0");
				strcat(sItemString, tmp);
			}
			else
				_itoa(RadarTarget.GetGS() / 10, sItemString, 10);
		}
		else
		{
			char * tmp = new char[5];
			_itoa(RadarTarget.GetGS() / 10, sItemString, 10);
			strcat(sItemString, "/");

			_itoa(FlightPlan.GetControllerAssignedData().GetAssignedSpeed() / 10, tmp, 10);
			strcat(sItemString, tmp);
		}
		break;
		
	// DESTINATION AIRPORT
	case 6: strcpy(sItemString, FlightPlan.GetFlightPlanData().GetDestination()); break;
	
	// COPN / COPX
	case 7:
		if (FlightPlan.GetFlightPlanData().GetPlanType()[0] != 'I' || FlightPlan.GetState() == EuroScopePlugIn::FLIGHT_PLAN_STATE_NON_CONCERNED) break;

		if (findById(RadarTarget.GetSystemID()) == NULL)
		{
			coordination tmpCoord = coordination(RadarTarget.GetSystemID(), FlightPlan.GetEntryCoordinationPointName(), false, FlightPlan.GetExitCoordinationPointName(), false);
			coord.AddTail(tmpCoord);
		}
		

		if (FlightPlan.GetTrackingControllerIsMe())
		{
			if (strlen(FlightPlan.GetExitCoordinationPointName()) <= 0)
				strcpy(sItemString, "COPX\0");
			else
			{
				if (FlightPlan.GetExitCoordinationNameState() == EuroScopePlugIn::COORDINATION_STATE_REQUESTED_BY_ME)
				{
					strcpy(sItemString, FlightPlan.GetExitCoordinationPointName());

					if (findById(RadarTarget.GetSystemID()) != NULL)
					{
						coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPXStatus(true);
						coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPXName(FlightPlan.GetExitCoordinationPointName());
					}
				}
				else if (FlightPlan.GetExitCoordinationNameState() == EuroScopePlugIn::COORDINATION_STATE_REQUESTED_BY_OTHER)
				{
					strcpy(sItemString, FlightPlan.GetExitCoordinationPointName());

					if (findById(RadarTarget.GetSystemID()) != NULL)
					{
						coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPXStatus(true);
						coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPXName(FlightPlan.GetExitCoordinationPointName());
					}
						
				}
				else if (FlightPlan.GetExitCoordinationNameState() == EuroScopePlugIn::COORDINATION_STATE_ACCEPTED || FlightPlan.GetExitCoordinationNameState() == EuroScopePlugIn::COORDINATION_STATE_MANUAL_ACCEPTED)
				{
					// ES BUG 546; WON'T ENTER
					strcpy(sItemString, "*");
					strcat(sItemString, FlightPlan.GetExitCoordinationPointName());
				}
				else if (findById(RadarTarget.GetSystemID()) != NULL)
				{
					if (coord.GetAt(findById(RadarTarget.GetSystemID())).getCOPXStatus() == true)
					{
						if (strcmp(coord.GetAt(findById(RadarTarget.GetSystemID())).getCOPXName(), FlightPlan.GetExitCoordinationPointName()) != 0)
						{
							//Request REFUSED
							strcpy(sItemString, FlightPlan.GetExitCoordinationPointName());
							coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPXStatus(false);
						}
						else
						{
							//Request ACCEPTED
							strcpy(sItemString, "•");
							strcat(sItemString, FlightPlan.GetExitCoordinationPointName());
						}
					}
					else
						strcpy(sItemString, FlightPlan.GetExitCoordinationPointName());
				}
				else
					strcpy(sItemString, FlightPlan.GetExitCoordinationPointName());
			}
		}
		else
		{
			if (strlen(FlightPlan.GetEntryCoordinationPointName()) <= 0)
				strcpy(sItemString, "COPN\0");
			else
			{
				if (FlightPlan.GetEntryCoordinationPointState() == EuroScopePlugIn::COORDINATION_STATE_REQUESTED_BY_ME)
				{
					strcpy(sItemString, FlightPlan.GetEntryCoordinationPointName());

					if (findById(RadarTarget.GetSystemID()) != NULL)
					{
						coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPNStatus(true);
						coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPNName(FlightPlan.GetEntryCoordinationPointName());
					}
				}
				else if (FlightPlan.GetEntryCoordinationPointState() == EuroScopePlugIn::COORDINATION_STATE_REQUESTED_BY_OTHER)
				{
					strcpy(sItemString, FlightPlan.GetEntryCoordinationPointName());

					if (findById(RadarTarget.GetSystemID()) != NULL)
					{
						coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPNStatus(true);
						coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPNName(FlightPlan.GetEntryCoordinationPointName());
					}
				}
				else if (FlightPlan.GetEntryCoordinationPointState() == EuroScopePlugIn::COORDINATION_STATE_ACCEPTED || FlightPlan.GetEntryCoordinationPointState() == EuroScopePlugIn::COORDINATION_STATE_MANUAL_ACCEPTED)
				{
					// ES BUG 546; WON'T ENTER
					strcpy(sItemString, "*");
					strcat(sItemString, FlightPlan.GetEntryCoordinationPointName());
				}
				else if (findById(RadarTarget.GetSystemID()) != NULL)
				{
					if (coord.GetAt(findById(RadarTarget.GetSystemID())).getCOPNStatus() == true)
					{
						if (strcmp(coord.GetAt(findById(RadarTarget.GetSystemID())).getCOPNName(), FlightPlan.GetEntryCoordinationPointName()) != 0)
						{
							//Request REFUSED
							strcpy(sItemString, FlightPlan.GetEntryCoordinationPointName());
							coord.GetAt(findById(RadarTarget.GetSystemID())).setCOPNStatus(false);
						}
						else
						{
							//Request ACCEPTED
							strcpy(sItemString, "•");
							strcat(sItemString, FlightPlan.GetEntryCoordinationPointName());
						}
					}
					else
						strcpy(sItemString, FlightPlan.GetEntryCoordinationPointName());
				}
				else
					strcpy(sItemString, FlightPlan.GetEntryCoordinationPointName());
				
			}
		}
		break;
	// ASSIGNED HEADING / DIRECT
	case 8:
		if (FlightPlan.GetFlightPlanData().GetPlanType()[0] != 'I' || FlightPlan.GetState() == EuroScopePlugIn::FLIGHT_PLAN_STATE_NON_CONCERNED) break;

		if (FlightPlan.GetControllerAssignedData().GetAssignedHeading() == 0)
		{
			if (strlen(FlightPlan.GetControllerAssignedData().GetDirectToPointName()) > 0)
				strcpy(sItemString, FlightPlan.GetControllerAssignedData().GetDirectToPointName());

			else if (FlightPlan.GetState() != EuroScopePlugIn::FLIGHT_PLAN_STATE_NON_CONCERNED)
				strcpy(sItemString, "AHDG\0");
		}
		else
			_itoa(FlightPlan.GetControllerAssignedData().GetAssignedHeading(), sItemString, 10); 
		break;
			
	// ASSIGNED/CLEARED ALTITUDE
	case 9: 
		if (FlightPlan.GetFlightPlanData().GetPlanType()[0] != 'I' || FlightPlan.GetState() == EuroScopePlugIn::FLIGHT_PLAN_STATE_NON_CONCERNED) break;
		
		if (tmpInt == 0)
			tmpInt = FlightPlan.GetFinalAltitude();

		if (tmpInt >= 10000)
			// 36000 -> 360
			_itoa(tmpInt / 100, sItemString, 10);
		else if (tmpInt >= 1000)
		{
			// 8000 -> 080
			char * tmp = new char[5];
			_itoa(tmpInt / 100, tmp, 10);

			sItemString[0] = '0';
			sItemString[1] = tmp[0];
			sItemString[2] = tmp[1];
			sItemString[3] = '\0';
		}
		else if (tmpInt >= 100)
		{
			// 300 -> 003
			char * tmp = new char[5];
			_itoa(tmpInt / 100, tmp, 10);

			sItemString[0] = '0';
			sItemString[1] = '0';
			sItemString[2] = tmp[0];
			sItemString[3] = '\0';
		}
		else if (tmpInt == 1)
		{
			// 52 -> 000
			sItemString[0] = '®';
			sItemString[1] = '\0';
		}
		else if (tmpInt == 2)
		{
			// 52 -> 000
			sItemString[0] = '©';
			sItemString[1] = '\0';
		}
		else
		{
			sItemString[0] = '0';
			sItemString[1] = '0';
			sItemString[2] = '0';
			sItemString[3] = '\0';
		}
		break;
			
	// CURRENT / NEXT SECTOR ID
	case 10:
		if (FlightPlan.GetFlightPlanData().GetPlanType()[0] != 'I') break;

		if (FlightPlan.GetState() != EuroScopePlugIn::FLIGHT_PLAN_STATE_ASSUMED && FlightPlan.GetState() != EuroScopePlugIn::FLIGHT_PLAN_STATE_TRANSFER_FROM_ME_INITIATED)
		{
			if (strlen(FlightPlan.GetTrackingControllerCallsign()) <= 0)
				strcpy(sItemString, "*\0");
			else if (sectorIndicator)
				_gcvt(ControllerSelect(FlightPlan.GetTrackingControllerCallsign()).GetPrimaryFrequency(), 6, sItemString);
			else
				strcpy(sItemString, FlightPlan.GetTrackingControllerId());
		}
		else if (strcmp(FlightPlan.GetCoordinatedNextController(), "UNICOM\0") == 0)
		{
			if (sectorIndicator)
				strcpy(sItemString, "122.80\0");
			else
				strcpy(sItemString, "*\0");
		}
		else if (strlen(FlightPlan.GetCoordinatedNextController()) == 0)
		{
			strcpy(sItemString, "*\0");
		}
		else
		{
			if (sectorIndicator)
				_gcvt(ControllerSelect(FlightPlan.GetCoordinatedNextController()).GetPrimaryFrequency(), 6, sItemString);
			else
				strcpy(sItemString, ControllerSelect(FlightPlan.GetCoordinatedNextController()).GetPositionId());
		}
		break;
	// VERTICAL SPEED INDICATOR (ARROW) 
	case 11: 
		if (FlightPlan.GetFlightPlanData().GetPlanType()[0] != 'I') break;
		
		if (tmpVS < -2)
			strcpy(sItemString, "|\0");
		else if (tmpVS > 2)
			strcpy(sItemString, "^\0");
		break;
			
	// VERTICAL SPEED (2 DIGIT)
	case 12:
		if (FlightPlan.GetFlightPlanData().GetPlanType()[0] != 'I') break;

		if (tmpVS < 0)
			tmpVS *= -1;

		if (tmpVS <= 2 && tmpVS >= -2)
			sItemString[0] = '\0';
		else if (tmpVS < 10)
		{
			char * tmp = new char[5];
			_itoa(tmpVS, tmp, 10);
			sItemString[0] = '0';
			sItemString[1] = _itoa(tmpVS, tmp, 10)[0];
			sItemString[2] = '\0';
		}
		else
			_itoa(tmpVS, sItemString, 10);
		break;
	
	//SCRATCHPAD
	case 13:
		if (strlen(FlightPlan.GetControllerAssignedData().GetScratchPadString()) == 0)
			strcpy(sItemString, "               \0");
		else
			strncpy(sItemString, FlightPlan.GetControllerAssignedData().GetScratchPadString(), 15);
		break;
	//SQUAWK
	case 14:
		strcpy(sItemString, RadarTarget.GetPosition().GetSquawk()); break;

	// NOTHING
	default: break;
	}
}


//---OnFunctionCall-----------------------------------------

void    tagPlugin::OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area)
{
	switch (FunctionId)
	{
	case 1: 
		if (sectorIndicator == true)
			sectorIndicator = false;
		else
			sectorIndicator = true;
		break;
	default: break;
	}
}

//---findById-----------------------------------------

POSITION tagPlugin::findById(const char* SystemId)
{
	POSITION tmp = coord.GetHeadPosition();

	for (int i = 0; i < coord.GetCount() && tmp != NULL; ++i)
	{
		if (strcmp(coord.GetAt(tmp).getId(), SystemId) == 0)
			return tmp;
		else
			coord.GetNext(tmp);
	}
	return NULL;
}

//
bool tagPlugin::isVFR(int squawk) 
{
	switch (squawk)
	{
	case 0:
	case 20:
	case 23:
	case 24:
	case 25:
	case 27:
	case 30:
	case 31:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 76:
	case 1200:
	case 2000:
	case 2200:
	case 7000:
		return true;
	default:
		return false;
	}
}