#include "DynamixelMotor.h"

DynamixelDevice::DynamixelDevice(DynamixelInterface &aInterface, DynamixelID aID):
	mInterface(aInterface), mID(aID), mStatusReturnLevel(255)
{
	mStatus=DYN_STATUS_OK;
	if(mID==BROADCAST_ID)
	{
		mStatusReturnLevel=0;
	}
}

DynamixelStatus DynamixelDevice::changeId(uint8_t id)
{
	DynamixelStatus result;
	result=write(DYN_ADDRESS_ID, id);
	if(result==DYN_STATUS_OK)
	{
		mID=id;
	}
	return result;
}

uint8_t DynamixelDevice::statusReturnLevel()
{
	if(mStatusReturnLevel==255)
	{
		init();
	}
	return mStatusReturnLevel;
}

void DynamixelDevice::statusReturnLevel(uint8_t aSRL)
{
	write(DYN_ADDRESS_SRL, aSRL);
	if(status()==DYN_STATUS_OK)
	{
		mStatusReturnLevel=aSRL;
	}
}

uint16_t DynamixelDevice::model()
{
	uint16_t result;
	read(DYN_ADDRESS_ID, result);
	return result;
}

uint8_t DynamixelDevice::firmware()
{
	uint8_t result;
	read(DYN_ADDRESS_FIRMWARE, result);
	return result;
}

void DynamixelDevice::communicationSpeed(uint32_t aSpeed)
{
	uint8_t value=2000000/aSpeed-1;
	if(value!=0) // forbid 2MBd rate, because it is out of spec, and can be difficult to undo
	{
		write(DYN_ADDRESS_BAUDRATE, value);
	}
}

DynamixelStatus DynamixelDevice::init()
{
	mStatusReturnLevel=2;
	DynamixelStatus status=ping();
	if(status!=DYN_STATUS_OK)
	{
		return status;
	}
	status=read(DYN_ADDRESS_SRL, mStatusReturnLevel);
	if(status&DYN_STATUS_TIMEOUT)
	{
		mStatusReturnLevel=0;
	}
	return DYN_STATUS_OK;
}



DynamixelMotor::DynamixelMotor(DynamixelInterface &aInterface, DynamixelID aId):
	DynamixelDevice(aInterface, aId)
{}


void DynamixelMotor::wheelMode()
{
	jointMode(0,0);
}

void DynamixelMotor::jointMode(uint16_t aCWLimit, uint16_t aCCWLimit)
{
	uint32_t data= (aCWLimit | (uint32_t(aCCWLimit)<<16));
	write(DYN_ADDRESS_CW_LIMIT, data);
}

void DynamixelMotor::enableTorque(bool aTorque)
{
	write(DYN_ADDRESS_ENABLE_TORQUE, uint8_t(aTorque?1:0));
}

void DynamixelMotor::speed(int16_t aSpeed)
{
	if(aSpeed<0) 
	{
		aSpeed=-aSpeed | 1024;
	}
	write(DYN_ADDRESS_GOAL_SPEED, aSpeed);
}

void DynamixelMotor::goalPosition(uint16_t aPosition)
{
	write(DYN_ADDRESS_GOAL_POSITION, aPosition);
}

void DynamixelMotor::led(uint8_t aState)
{
	write(DYN_ADDRESS_LED, aState);
}

uint16_t DynamixelMotor::currentPosition()
{
	uint16_t currentPosition;
	read(DYN_ADDRESS_CURRENT_POSITION, currentPosition);
	return currentPosition;
}

DynamixelStatus DynamixelMotor::getCurrentPosition(uint16_t &pos)
{
	return read(DYN_ADDRESS_CURRENT_POSITION, pos);
}

DynamixelStatus DynamixelMotor::setComplianceMargins(uint8_t cw_margin, uint8_t ccw_margin, uint8_t cw_slope, uint8_t ccw_slope)
{
	DynamixelStatus status;

	status = write(DYN_ADDRESS_CW_COMP_MARGIN, cw_margin);
	if (DYN_STATUS_OK != status) { return status; }

	status = write(DYN_ADDRESS_CCW_COMP_MARGIN, ccw_margin);
	if (DYN_STATUS_OK != status) { return status; }

	status = write(DYN_ADDRESS_CW_COMP_SLOPE, cw_slope);
	if (DYN_STATUS_OK != status) { return status; }

	return write(DYN_ADDRESS_CCW_COMP_SLOPE, ccw_slope);
}
	
DynamixelStatus DynamixelMotor::getComplianceMargins(uint8_t &cw_margin, uint8_t &ccw_margin, uint8_t &cw_slope, uint8_t &ccw_slope)
{
	DynamixelStatus status;

	status = read(DYN_ADDRESS_CW_COMP_MARGIN, cw_margin);
	if (DYN_STATUS_OK != status) { return status; }

	status = read(DYN_ADDRESS_CCW_COMP_MARGIN, ccw_margin);
	if (DYN_STATUS_OK != status) { return status; }

	status = read(DYN_ADDRESS_CW_COMP_SLOPE, cw_slope);
	if (DYN_STATUS_OK != status) { return status; }
	
	return read(DYN_ADDRESS_CCW_COMP_SLOPE, ccw_slope);
}

DynamixelStatus DynamixelMotor::isMoving(uint8_t &moving) 
{
	return read(DYN_ADDRESS_MOVING, moving);
}
