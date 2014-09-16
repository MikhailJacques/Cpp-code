
// Pump class implementation file

#include "Pump.h"

//************************************************************************
// @@ Function Name	: Pump Constructor
// @ Description	: Initialization and creation of a Pump object from DB
// @ Input			: TaskID - Relevant control thread (described at tasks.h) <-------- Should be defined at tasks.h - Speak to Vlad
//					  PumpID - Index of DB record to be retrieved for the pump object  
// @ Output			:
// @ Notes			: 
//************************************************************************
Pump::Pump(DBTask TaskID, DBIndex PumpID)
{
	m_TaskID = TaskID;
	m_PumpID = PumpID;

	// Check if the record exists in the DB 
    if (DB_Exist(DBPUMP_TYPE, TaskID, PumpID))	
    {
		// Allocate memory (on stack) for pump structure
		DBPump tmpPump;

		// Retrieve the record from the database and store it in the tmpPump structure
		if (DB_GetRecord(DBPUMP_TYPE, TaskID, PumpID, &tmpPump))
		{
			// Initialize class members with retrieved data
			m_PumpStationID		= tmpPump.PumpStationID;
			m_NominalFlow		= tmpPump.NominalFlow;	
			m_OnDelayTimeMs		= tmpPump.OnDelayTimeMs;	
			m_OffDelayTimeMs	= tmpPump.OffDelayTimeMs;	
			m_StabilityTimeMs	= tmpPump.StabilityTimeMs;  
			m_Capacity			= tmpPump.Capacity;

			return;
		}
	}

	// In case of error - set defaults
	m_PumpStationID		= 0;
	m_NominalFlow		= 250;	
	m_OnDelayTimeMs		= 0;	
	m_OffDelayTimeMs	= 0;	
	m_StabilityTimeMs	= 0;  
	m_Capacity			= 50;
}

//************************************************************************
// @@ Function Name	: Pump Copy Constructor
// @ Description	: Copy constructor
// @ Input			: pump - Other pump object   
// @ Output			:
// @ Notes			: 
//************************************************************************
Pump::Pump(const Pump& pump)
{
	// Check for self-assignment!
	if ( this != &pump )    
	{
		SetValues(pump);
	}
}

//************************************************************************
// @@ Function Name	: Pump Overloaded Operator =
// @ Description	: Overloaded operator =
// @ Input			: pump - Other pump object   
// @ Output			:
// @ Notes			: 
//************************************************************************
const Pump& Pump::operator=(const Pump& pump)
{
	// Check for self-assignment!
	if (this != &pump)    
	{
		SetValues(pump);
	}

	return *this;
}

//************************************************************************
// @@ Function Name	: Pump Destructor
// @ Description	: Destructor
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
Pump::~Pump( );


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
DBTask Pump::GetTaskID( )
{
	return m_TaskID;
}

//************************************************************************
// @@ Function Name	: GetPumpID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
DBIndex Pump::GetPumpID( )
{
	return m_PumpID;
}

//************************************************************************
// @@ Function Name	: GetPumpStationID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
DBIndex Pump::GetPumpStationID( )
{
	return m_PumpStationID;
}

//************************************************************************
// @@ Function Name	: GetNominalFlow
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
unsigned short Pump::GetNominalFlow()
{
	return m_NominalFlow;
}

//************************************************************************
// @@ Function Name	: GetOnDelayTimeMs
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
long Pump::GetOnDelayTimeMs()
{
	return m_OnDelayTimeMs
}

//************************************************************************
// @@ Function Name	: GetOffDelayTimeMs
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
long Pump::GetOffDelayTimeMs()
{
	return m_OffDelayTimeMs
}

//************************************************************************
// @@ Function Name	: GetStabilityTimeMs
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
long Pump::GetStabilityTimeMs()
{
	return m_StabilityTimeMs
}

//************************************************************************
// @@ Function Name	: GetCapacity
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
byte Pump::GetCapacity()
{
	return m_Capacity;
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
void Pump::SetTaskID(DBTask TaskID)
{
	m_TaskID = TaskID;
}

//************************************************************************
// @@ Function Name	: SetPumpID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void Pump::SetPumpID(DBIndex PumpID)
{
	m_PumpID = PumpID;
}

//************************************************************************
// @@ Function Name	: SetPumpStationID
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void Pump::SetPumpStationID(DBIndex PumpStationID)
{
	m_PumpStationID = PumpStationID;
}

//************************************************************************
// @@ Function Name	: SetNominalFlow
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void Pump::SetNominalFlow(unsigned short NominalFlow)
{
	m_NominalFlow = NominalFlow;
}

//************************************************************************
// @@ Function Name	: SetOnDelayTimeMs
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void Pump::SetOnDelayTimeMs(long OnDelayTimeMs)
{
	m_OnDelayTimeMs = OnDelayTimeMs;
}

//************************************************************************
// @@ Function Name	: SetOffDelayTimeMs
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void Pump::SetOffDelayTimeMs(long OffDelayTimeMs)
{
	m_OffDelayTimeMs = OffDelayTimeMs
}

//************************************************************************
// @@ Function Name	: SetStabilityTimeMs
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void Pump::SetStabilityTimeMs(long StabilityTimeMs)
{
	m_StabilityTimeMs = StabilityTimeMs;
}

//************************************************************************
// @@ Function Name	: SetCapacity
// @ Description	: 
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void Pump::SetCapacity(byte Capacity)
{
	m_Capacity = Capacity;
}

//************************************************************************
// @@ Function Name	: SetValues
// @ Description	: Copy other object values to current object
// @ Input			:   
// @ Output			:
// @ Notes			: 
//************************************************************************
void Pump::SetValues(const Pump& pump)
{
	// Aliasing test
	if ( this != &pump )    
	{
		SetTaskID(pump.GetTaskID());
		SetPumpID(pump.GetPumpID());			
		SetPumpStationID(pump.GetPumpStationID());
		SetNominalFlow(pump.GetNominalFlow());
		SetOnDelayTimeMs(pump.GetOnDelayTimeMs());		
		SetOffDelayTimeMs(pump.GetOffDelayTimeMs());
		SetStabilityTimeMs(pump.GetStabilityTimeMs()); 
		SetCapacity(pump.GetCapacity());
	}
}