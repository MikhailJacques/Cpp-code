
// WaterSource class interface file

#ifndef WATER_SOURCE_H
#define WATER_SOURCE_H

#include "DB.h"
#include "DBBase.h"
#include "DBDevices.h"		
#include "NMC_MML_Platform.h"

class WaterSource : public DBWaterSource
{
	public:
		
		explicit WaterSource(DBTask TaskID, DBIndex WaterSourceID); 
		explicit WaterSource(const WaterSource & waterSource);
		const WaterSource& operator=(const WaterSource& waterSource);
		~WaterSource( );

		/////////////////////////////////////////////////////////////
		// Accessor functions
		/////////////////////////////////////////////////////////////
		DBTask				GetTaskID();
		DBIndex				GetWaterSourceID();
		const DBIndex[]		GetPumpStationID();
		byte				GetPressureMeter();
		byte				GetWaterTemperature();
		byte				GetRequiredWaterTemperature();
		WATER_SOURCE_LEVEL	GetWaterLevel();

		/////////////////////////////////////////////////////////////
		// Mutator functions
		////////////////////////////////////////////////////////////
		void SetTaskID(DBTask TaskID);
		void SetWaterSourceID(DBIndex WaterSourceID);
		void SetPumpStationID(DBIndex[] PumpStationID);
		void SetPressureMeter(byte PressureMeter);
		void SetWaterTemperature(byte WaterTemperature);
		void SetRequiredWaterTemperature(byte RequiredWaterTemperature);
		void SetWaterLevel(WATER_SOURCE_LEVEL WaterLevel);
				
		void SetValues(const WaterSource& waterSource);

	private:

		DBTask				m_TaskID;
		
		DBIndex				m_WaterSourceID;
		DBIndex				m_PumpStationID[MAX_PUMP_STATIONS];	// Describes which PumpStations assigned to current water source
		byte				m_PressureMeter;
		byte				m_WaterTemperature;
		byte				m_RequiredWaterTemperature;
		WATER_SOURCE_LEVEL	m_WaterLevel;

};

#endif


