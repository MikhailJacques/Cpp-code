
// PumpStation class interface file

#ifndef PUMP_STATION_H
#define PUMP_STATION_H

#include "DB.h"
#include "DBBase.h"
#include "DBDevices.h"		
#include "NMC_MML_Platform.h"

class PumpStation : public DBPumpStation
{
	public:
		
		explicit PumpStation(DBTask TaskID, DBIndex WaterSourceID); 
		explicit PumpStation(const WaterSource & waterSource);
		const PumpStation& operator=(const WaterSource& waterSource);
		~PumpStation( );

		/////////////////////////////////////////////////////////////
		// Accessor functions
		/////////////////////////////////////////////////////////////
		DBTask			GetTaskID();
		DBIndex			GetPumpStationID();
		const DBIndex[] GetWaterSourceID();
		const DBIndex[] GetPumpID();
		PUMPING_METHOD	GetPumpingMethod()
		bool			GetRotation();

		/////////////////////////////////////////////////////////////
		// Mutator functions
		////////////////////////////////////////////////////////////
		void SetTaskID(DBTask TaskID);
		void SetPumpStationID(DBIndex PumpStationID);
		void SetWaterSourceID(DBIndex[] WaterSourceID);
		void SetPumpID(DBIndex[] PumpID);
		void SetPumpingMethod(PUMPING_METHOD PumpingMethod);
		void SetRotation(bool m_Rotation);
				
		void SetValues(const PumpStation& pumpStation);

	private:

		DBTask				m_TaskID;
		
		DBIndex				m_PumpStationID;
		DBIndex				m_WaterSourceID[MAX_WATER_SOURCES];	// Describes which water sources assigned to current pump station
		DBIndex				m_PumpID[MAX_PUMPS_PER_STATION];	// Describes which pumps assigned to current pump station
		PUMPING_METHOD		m_PumpingMethod;
		bool				m_Rotation;

};

#endif


