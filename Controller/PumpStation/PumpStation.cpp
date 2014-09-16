
// PumpStation class implementation file

#include "PumpStation.h"

//************************************************************************
// @@ Function Name	: PumpStation Constructor
// @ Description	: Initialization and creation of a PumpStation object from DB
// @ Input			: TaskID - Relevant control thread (described at tasks.h) <-------- Defined at tasks.h as - PUMP_ST_SLOT
//					  PumpStationID - Index of DB record to be retrieved for the PumpStation object  
// @ Output			:
// @ Notes			: 
//************************************************************************
PumpStation::PumpStation(DBTask TaskID, DBIndex PumpStationID)
{
	m_TaskID = TaskID;
	m_PumpStationID = PumpStationID;

	// Check if the record exists in the DB 
    if (DB_Exist(DBPUMPSTATION_TYPE, TaskID, PumpStationID))	
    {
		// Allocate memory (on stack) for PumpStation structure
		DBPumpStation tmpPumpStation;

		// Retrieve the record from the database and store it in the tmpPumpStation structure
		if (DB_GetRecord(DBPUMPSTATION_TYPE, TaskID, PumpStationID, &tmpPumpStation))
		{
			// Initialize class members with retrieved data
			memcpy(m_WaterSourceID, tmpPumpStation.m_WaterSourceID, sizeof(DBIndex * MAX_WATER_SOURCES));
			memcpy(m_PumpID, tmpPumpStation.m_PumpID, sizeof(DBIndex * MAX_PUMPS_PER_STATION));
			
			m_PumpingMethod	= tmpPumpStation.m_PumpingMethod;
			m_Rotation		= tmpPumpStation.m_Rotation;

			return;
		}
	}

	// In case of error - set defaults
	memset (m_WaterSourceID, 0, sizeof(DBIndex * MAX_WATER_SOURCES));
	memset (m_PumpID, 0, sizeof(DBIndex * MAX_PUMPS_PER_STATION));
	m_PumpingMethod	= PUMPIMG_METHOD_CALCULATED;
	m_Rotation		= false;
}

//************************************************************************
// @@ Function Name	: PumpStation Copy Constructor
// @ Description	: Copy constructor
// @ Input			: pumpStation - Other PumpStation object   
// @ Output			:
// @ Notes			: 
//************************************************************************
PumpStation::PumpStation(const PumpStation& pumpStation)
{
	// Check for self-assignment!
	if ( this != &pumpStation )    
	{
		SetValues(pumpStation);
	}
}

//************************************************************************
// @@ Function Name	: PumpStation Overloaded Operator =
// @ Description	: Overloaded operator =
// @ Input			: pumpStation - Other PumpStation object   
// @ Output			:
// @ Notes			: 
//************************************************************************
const PumpStation& PumpStation::operator=(const PumpStation& pumpStation)
{
	// Check for self-assignment!
	if (this != &pumpStation)    
	{
		SetValues(pumpStation);
	}

	return *this;
}

//************************************************************************
// @@ Function Name	: PumpStation Destructor
// @ Description	: Destructor
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
PumpStation::~PumpStation();


/////////////////////////////////////////////////////////////
// Accessor Functions
/////////////////////////////////////////////////////////////

//************************************************************************
// @@ Function Name	: GetTaskID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
DBTask PumpStation::GetTaskID()
{
	return m_TaskID;
}

//************************************************************************
// @@ Function Name	: GetPumpStationID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
DBIndex PumpStation::GetPumpStationID()
{
	return m_PumpStationID;
}

//************************************************************************
// @@ Function Name	: GetWaterSourceID
// @ Description	: 
// @ Input			:   
// @ Output			: An array of all water sources assigned to this pump station
// @ Notes			: 
//************************************************************************
const DBIndex[] PumpStation::GetWaterSourceID()
{
	return m_WaterSourceID;
}

//************************************************************************
// @@ Function Name	: GetPumpID
// @ Description	: 
// @ Input			:   
// @ Output			: An array of all pumps assigned to this pump station
// @ Notes			: 
//************************************************************************
const DBIndex[] PumpStation::GetPumpID()
{
	return m_PumpID;
}

//************************************************************************
// @@ Function Name	: GetPumpingMethod
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
PUMPING_METHOD PumpStation::GetPumpingMethod()
{
	return m_PumpingMethod;
}

//************************************************************************
// @@ Function Name	: GetRotation
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
bool PumpStation::GetRotation()
{
	return m_Rotation;
}

/////////////////////////////////////////////////////////////
// Mutator functions
////////////////////////////////////////////////////////////

//************************************************************************
// @@ Function Name	: SetTaskID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void PumpStation::SetTaskID(DBTask TaskID)
{
	m_TaskID = TaskID;
}

//************************************************************************
// @@ Function Name	: SetPumpStationID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void PumpStation::SetPumpStationID(DBIndex PumpStationID)
{
	m_PumpStationID = PumpStationID;
}

//************************************************************************
// @@ Function Name	: SetWaterSourceID
// @ Description	: 
// @ Input			: An array of all connected water sources to this pump station
// @ Output			:
// @ Notes			: 
//************************************************************************
void PumpStation::SetWaterSourceID(DBIndex[] WaterSourceID)
{
	memcpy(m_WaterSourceID, WaterSourceID, sizeof(DBIndex * MAX_WATER_SOURCES));
}

//************************************************************************
// @@ Function Name	: SetPumpStationID
// @ Description	: 
// @ Input			: An array of all connected pumps to this pump station
// @ Output			:
// @ Notes			: 
//************************************************************************
void PumpStation::SetPumpID(DBIndex[] PumpID)
{
	memcpy(m_PumpID, PumpID, sizeof(DBIndex * MAX_PUMPS_PER_STATION));
}

//************************************************************************
// @@ Function Name	: SetPumpingMethod
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void PumpStation::SetPumpingMethod(PUMPING_METHOD PumpingMethod)
{
	m_PumpingMethod = PumpingMethod;
}

//************************************************************************
// @@ Function Name	: SetRotation
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void PumpStation::SetRotation(bool m_Rotation)
{
	m_Rotation = Rotation;
}

//************************************************************************
// @@ Function Name	: SetValues
// @ Description	: Copy other object values to current object
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void PumpStation::SetValues(const PumpStation& pumpStation)
{
	// Aliasing test
	if ( this != &waterSource )    
	{
		SetTaskID(pumpStation.TaskID);
		SetPumpStationID(pumpStation.PumpStationID);
		SetWaterSourceID(pumpStation.WaterSourceID);
		SetPumpID(pumpStation.PumpID);
		SetPumpingMethod(pumpStation.PumpingMethod);
		SetRotation(pumpStation.m_Rotation);
	}
}