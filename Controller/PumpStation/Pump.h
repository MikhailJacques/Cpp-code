
// Pump class interface file

#ifndef PUMP_H
#define PUMP_H

#include "DB.h"
#include "DBBase.h"
#include "DBDevices.h"		
#include "NMC_MML_Platform.h"

class Pump : public DBPump
{
	public:
		
		explicit Pump(DBTask TaskID, DBIndex PumpID); 
		explicit Pump(const Pump & pump);
		const Pump& operator=(const Pump& pump);
		~Pump( );

		/////////////////////////////////////////////////////////////
		// Accessor functions
		/////////////////////////////////////////////////////////////
		DBTask			GetTaskID();
		DBIndex			GetPumpID();
		DBIndex			GetPumpStationID();
		unsigned short	GetNominalFlow();
		long			GetOnDelayTimeMs();
		long			GetOffDelayTimeMs();
		long			GetStabilityTimeMs();
		byte			GetCapacity();

		/////////////////////////////////////////////////////////////
		// Mutator functions
		////////////////////////////////////////////////////////////
		void SetTaskID(DBTask TaskID);
		void SetPumpID(DBIndex PumpID);
		void SetPumpStationID(DBIndex PumpStationID);
		void SetNominalFlow(unsigned short NominalFlow);
		void SetOnDelayTimeMs(long OnDelayTimeMs);
		void SetOffDelayTimeMs(long OffDelayTimeMs);
		void SetStabilityTimeMs(long StabilityTimeMs);
		void SetCapacity(byte Capacity);
		
		void SetValues(const Pump& pump);

	private:

		DBTask			m_TaskID;
		
		DBIndex			m_PumpID;
		DBIndex			m_PumpStationID;	// Describes to which PumpStation current Pump belongs
		unsigned short	m_NominalFlow;		// The nominal flow of the pump
		long			m_OnDelayTimeMs;	// The time that takes for the pump motor to start. (for Diesel pump)
		long 			m_OffDelayTimeMs;	// Off time before switching off the next pump. Prevent water hummer (Off delay)
		long 			m_StabilityTimeMs;	// The time takes until the pump gets to the working point. Until this time is completed, the controller don't calculate flow (for alarm)
		byte			m_Capacity;			// The capacity of the pump. In KW
};

#endif


