
#ifndef DYNAMIXEL_INTERFACE_IMPL_H
#define DYNAMIXEL_INTERFACE_IMPL_H

#include "DynamixelInterface.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

template<class T>
class DynamixelInterfaceImpl:public DynamixelInterface
{
	private:
	/** \brief Switch stream to read (receive)) mode */
	void readMode();
	
	/** \brief Switch stream to write (send) mode */
	void writeMode();
	
	public:
	
	/**
	 * \brief Constructor
	 * \param[in] aStreamController : stream controller that abstract real stream
	 * \param[in] aTxPin : pin number of the tx pin
	 * \param[in] aDirectionPin : direction pin, use NO_DIR_PORT if you do not use one (default)
	*/
	DynamixelInterfaceImpl(T &aStream, uint8_t aTxPin, uint8_t aDirectionPin);
	
	/**
	 * \brief Destructor
	 * Delete stream if it is owned by the instance
	 */
	~DynamixelInterfaceImpl();
	
	/**
	 * \brief Start interface
	 * \param[in] aBaud : Baudrate
	 *
	 * Start the interface with desired baudrate, call once before using the interface
	*/
	void begin(unsigned long aBaud);
	
	/**
	 * \brief Send a packet on bus
	 * \param[in] aPacket : Packet to send
	 *
	 * The function wait for the packet to be completly sent (using Stream.flush)
	*/
	void sendPacket(const DynamixelPacket &aPacket);

	/**
	 * \brief Receive a packet on bus
	 * \param[out] aPacket : Received packet. mData field must be previously allocated
	 * \param[in] answerSize : the size of the memory allocated to the mData field
	 *
	 * The function wait for a new packet on the bus. Timeout depends of timeout of the underlying stream.
	 * Return error code in case of communication error (timeout, checksum error, ...)
	*/
	void receivePacket(DynamixelPacket &aPacket, uint8_t answerSize = 0);

	/**
         * \brief Stop interface
        */
        void end();
	
	static const uint8_t NO_DIR_PORT=255;
	
	private:
	
	T &mStream;
	const uint8_t mDirectionPin;

	protected:
	const uint8_t mTxPin;
};

class HardwareDynamixelInterface:public DynamixelInterfaceImpl<HardwareSerial>
{
	public:
	HardwareDynamixelInterface(HardwareSerial &aSerial, uint8_t aDirectionPin=NO_DIR_PORT);
	~HardwareDynamixelInterface();
};

class SoftwareDynamixelInterface:public DynamixelInterfaceImpl<SoftwareSerial>
{
	public:
	SoftwareDynamixelInterface(uint8_t aRxPin, uint8_t aTxPin, uint8_t aDirectionPin=NO_DIR_PORT);
	~SoftwareDynamixelInterface();
	private:
	SoftwareSerial mSoftSerial;
};


#endif
