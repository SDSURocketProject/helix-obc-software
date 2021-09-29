#ifndef CANIDS_H_
#define CANIDS_H_

#include <linux/can.h>
#include <stdint.h>

enum STATES {
	STATE_IDLE,
	STATE_DEBUG,
	STATE_DRY_SYSTEMS,
	STATE_LEAK_CHECK,
	STATE_FUELING,
	STATE_LAUNCH,
	STATE_STAGE_TWO,
	STATE_RECOVERY,
	STATE_GROUND_SAFE,
	STATE_FLIGHT_SAFE,
	STATE_MAX_STATES
};

enum CANIDs : uint32_t {
	CANIDS_PRIMARY_CLOCK_SYNC = 0UL,
	CANIDS_OBC_CLOCK_SYNC = 1UL,
	CANIDS_EMERGENCY_SIGNAL = 10UL,
	CANIDS_TRANSITION_CONE_STATUS = 50UL,
	CANIDS_ARM_RECOVERY = 60UL,
	CANIDS_DETATCH_SECOND_STAGE = 61UL,
	CANIDS_HELIUM_PRESSURE_PT_DATA = 100UL,
	CANIDS_LOX_PRESSURE_PT_DATA = 101UL,
	CANIDS_ETHANOL_PRESSURE_PT_DATA = 102UL,
	CANIDS_CHAMBER_PRESSURE_PT_DATA = 103UL,
	CANIDS_HELIUM_FILL_VALVE_HALL_EFFECT_STATE = 200UL,
	CANIDS_LOX_FILL_VALVE_HALL_EFFECT_STATE = 201UL,
	CANIDS_ETHANOL_FILL_VALVE_HALL_EFFECT_STATE = 202UL,
	CANIDS_LOX_TANK_LIQUID_LEVEL_DATA = 250UL,
	CANIDS_ETHANOL_TANK_LIQUID_LEVEL_DATA = 251UL,
	CANIDS_LOX_TANK_TEMPERATURE_DATA = 300UL,
	CANIDS_ETHANOL_TANK_TEMPERATURE_DATA = 301UL,
	CANIDS_NOZZLE_TEMPERATURE_DATA = 302UL,
	CANIDS_UPPER_AIR_FRAME_TEMPERATURE_DATA = 303UL,
	CANIDS_ITC_TEMPERATURE_DATA = 304UL,
	CANIDS_LOWER_AIR_FRAME_TEMPERATURE_DATA = 305UL,
	CANIDS_HELIUM_PRESSURE_PT_CURRENT = 400UL,
	CANIDS_LOX_PRESSURE_PT_CURRENT = 401UL,
	CANIDS_ETHANOL_PRESSURE_PT_CURRENT = 402UL,
	CANIDS_CHAMBER_PRESSURE_PT_CURRENT = 403UL,
	CANIDS_HELIUM_FILL_VALVE_HALL_EFFECT_CURRENT = 404UL,
	CANIDS_LOX_FILL_VALVE_HALL_EFFECT_CURRENT = 405UL,
	CANIDS_ETHANOL_FILL_VALVE_HALL_EFFECT_CURRENT = 406UL,
	CANIDS_UPPER_AIR_FRAME_VIN_CURRENT = 407UL,
	CANIDS_ITC_VIN_CURRENT = 408UL,
	CANIDS_LOWER_AIR_FRAME_VIN_CURRENT = 409UL,
	CANIDS_UPPER_AIR_FRAME_VIN_VOLTAGE = 500UL,
	CANIDS_ITC_VIN_VOLTAGE = 501UL,
	CANIDS_LOWER_AIR_FRAME_VIN_VOLTAGE = 502UL,
	CANIDS_MAX_CANID
};

struct primaryClockSync {
	uint32_t timeSinceSystemStart;
} __attribute((aligned (1)));

struct obcClockSync {
	uint32_t timeSinceSystemStart;
} __attribute((aligned (1)));

#define EMERGENCY_SIGNAL_STATUS_SYSTEM_STATUS 0x03
struct emergencySignal {
	uint8_t status;
} __attribute((aligned (1)));

#define TRANSITION_CONE_STATUS_STATUS_ARMED 0x01
struct transitionConeStatus {
	uint8_t status;
} __attribute((aligned (1)));

#define ARM_RECOVERY_STATUS_ARM_RECOVERY 0x01
struct armRecovery {
	uint8_t status;
} __attribute((aligned (1)));

#define DETATCH_SECOND_STAGE_STATUS_DETATCH_SECOND_STAGE 0x01
struct detatchSecondStage {
	uint8_t status;
} __attribute((aligned (1)));

struct heliumPressurePtData {
	uint32_t timeSinceSystemStart;
	uint16_t heliumPressure;
	uint16_t rawHeliumPressureMeasurement;
} __attribute((aligned (1)));

struct loxPressurePtData {
	uint32_t timeSinceSystemStart;
	uint16_t loxPressure;
	uint16_t rawLoxPressureMeasurement;
} __attribute((aligned (1)));

struct ethanolPressurePtData {
	uint32_t timeSinceSystemStart;
	uint16_t ethanolPressure;
	uint16_t rawEthanolPressureMeasurement;
} __attribute((aligned (1)));

struct chamberPressurePtData {
	uint32_t timeSinceSystemStart;
	uint16_t chamberPressure;
	uint16_t rawChamberPressureMeasurement;
} __attribute((aligned (1)));

struct heliumFillValveHallEffectState {
	uint32_t timeSinceSystemStart;
	uint8_t heliumFillValveHallEffectState;
} __attribute((aligned (1)));

struct loxFillValveHallEffectState {
	uint32_t timeSinceSystemStart;
	uint8_t loxFillValveHallEffectState;
} __attribute((aligned (1)));

struct ethanolFillValveHallEffectState {
	uint32_t timeSinceSystemStart;
	uint8_t ethanolFillValveHallEffectState;
} __attribute((aligned (1)));

struct loxTankLiquidLevelData {
	uint32_t timeSinceSystemStart;
	uint8_t loxTankLiquidLevel;
	uint16_t rawLoxTankLiquidLevelMeasurement;
} __attribute((aligned (1)));

struct ethanolTankLiquidLevelData {
	uint32_t timeSinceSystemStart;
	uint8_t ethanolTankLiquidLevel;
	uint16_t rawEthanolTankLiquidLevelMeasurement;
} __attribute((aligned (1)));

struct loxTankTemperatureData {
	uint32_t timeSinceSystemStart;
	int16_t loxTankTemperature;
	uint16_t rawLoxTankTemperatureMeasurement;
} __attribute((aligned (1)));

struct ethanolTankTemperatureData {
	uint32_t timeSinceSystemStart;
	int16_t ethanolTankTemperature;
	uint16_t rawEthanolTankTemperatureMeasurement;
} __attribute((aligned (1)));

struct nozzleTemperatureData {
	uint32_t timeSinceSystemStart;
	int16_t nozzleTemperature;
	uint16_t rawNozzleTemperatureMeasurement;
} __attribute((aligned (1)));

struct upperAirFrameTemperatureData {
	uint32_t timeSinceSystemStart;
	int16_t upperAirFrameTemperature;
	uint16_t rawUpperAirFrameTemperatureMeasurement;
} __attribute((aligned (1)));

struct itcTemperatureData {
	uint32_t timeSinceSystemStart;
	int16_t itcTemperature;
	uint16_t rawItcTemperatureMeasurement;
} __attribute((aligned (1)));

struct lowerAirFrameTemperatureData {
	uint32_t timeSinceSystemStart;
	int16_t lowerAirFrameTemperature;
	uint16_t rawLowerAirFrameTemperatureMeasurement;
} __attribute((aligned (1)));

struct heliumPressurePtCurrent {
	uint32_t timeSinceSystemStart;
	int16_t heliumPressurePtCurrent;
	uint16_t rawHeliumPressurePtCurrentMeasurement;
} __attribute((aligned (1)));

struct loxPressurePtCurrent {
	uint32_t timeSinceSystemStart;
	int16_t loxPressurePtCurrent;
	uint16_t rawLoxPressurePtCurrentMeasurement;
} __attribute((aligned (1)));

struct ethanolPressurePtCurrent {
	uint32_t timeSinceSystemStart;
	int16_t ethanolPressurePtCurrent;
	uint16_t rawEthanolPressurePtCurrentMeasurement;
} __attribute((aligned (1)));

struct chamberPressurePtCurrent {
	uint32_t timeSinceSystemStart;
	int16_t chamberPressurePtCurrent;
	uint16_t rawChamberPressurePtCurrentMeasurement;
} __attribute((aligned (1)));

struct heliumFillValveHallEffectCurrent {
	uint32_t timeSinceSystemStart;
	int16_t heliumFillValveHallEffectCurrent;
	uint16_t rawHeliumFillValveHallEffectCurrentMeasurement;
} __attribute((aligned (1)));

struct loxFillValveHallEffectCurrent {
	uint32_t timeSinceSystemStart;
	int16_t loxFillValveHallEffectCurrent;
	uint16_t rawLoxFillValveHallEffectCurrentMeasurement;
} __attribute((aligned (1)));

struct ethanolFillValveHallEffectCurrent {
	uint32_t timeSinceSystemStart;
	int16_t ethanolFillValveHallEffectCurrent;
	uint16_t rawEthanolFillValveHallEffectCurrentMeasurement;
} __attribute((aligned (1)));

struct upperAirFrameVinCurrent {
	uint32_t timeSinceSystemStart;
	int16_t upperAirFrameBoardCurrent;
	uint16_t rawUpperAirFrameBoardCurrentMeasurement;
} __attribute((aligned (1)));

struct itcVinCurrent {
	uint32_t timeSinceSystemStart;
	int16_t itcBoardCurrent;
	uint16_t rawItcBoardCurrentMeasurement;
} __attribute((aligned (1)));

struct lowerAirFrameVinCurrent {
	uint32_t timeSinceSystemStart;
	int16_t lowerAirFrameBoardCurrent;
	uint16_t rawLowerAirFrameBoardCurrentMeasurement;
} __attribute((aligned (1)));

struct upperAirFrameVinVoltage {
	uint32_t timeSinceSystemStart;
	int16_t upperAirFrameBoardVinVoltage;
	uint16_t rawUpperAirFrameBoardVinVoltageMeasurement;
} __attribute((aligned (1)));

struct itcVinVoltage {
	uint32_t timeSinceSystemStart;
	int16_t itcBoardVinVoltage;
	uint16_t rawItcBoardVinVoltageMeasurement;
} __attribute((aligned (1)));

struct lowerAirFrameVinVoltage {
	uint32_t timeSinceSystemStart;
	int16_t lowerAirFrameBoardVinVoltage;
	uint16_t rawLowerAirFrameBoardVinVoltageMeasurement;
} __attribute((aligned (1)));

#endif // CANIDS_H_
