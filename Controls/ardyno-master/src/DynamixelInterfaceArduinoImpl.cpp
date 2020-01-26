
#include "DynamixelInterfaceArduinoImpl.h"

#if __AVR__
// define TXEN, RXEN and RXCIE
#if !defined(TXEN)
#if defined(TXEN0)
#define TXEN TXEN0
#define RXEN RXEN0
#define RXCIE RXCIE0
#elif defined(TXEN1) // Some devices have uart1 but no uart0 (leonardo)
#define TXEN TXEN1
#define RXEN RXEN1
#define RXCIE RXCIE1
#endif
#endif
#endif //__AVR__

template<class T>
void DynamixelInterfaceImpl<T>::readMode()
{
	if(mDirectionPin!=NO_DIR_PORT)
	{
		digitalWrite(mDirectionPin, LOW);
	}
	else
	{
		setReadMode(mStream, mTxPin);
	}
}
	
template<class T>
void DynamixelInterfaceImpl<T>::writeMode()
{
	if(mDirectionPin!=NO_DIR_PORT)
	{
		digitalWrite(mDirectionPin, HIGH);
	}
	else
	{
		setWriteMode(mStream, mTxPin);
	}
}
	
template<class T>
DynamixelInterfaceImpl<T>::DynamixelInterfaceImpl(T &aStream, uint8_t aTxPin, uint8_t aDirectionPin=NO_DIR_PORT):
	mStream(aStream), mDirectionPin(aDirectionPin), mTxPin(aTxPin)
{
	if(mDirectionPin!=NO_DIR_PORT)
	{
		digitalWrite(mDirectionPin, LOW);
		pinMode(mDirectionPin, OUTPUT);
	}
}

template<class T>
DynamixelInterfaceImpl<T>::~DynamixelInterfaceImpl()
{
}

template<class T>
void DynamixelInterfaceImpl<T>::begin(unsigned long aBaud)
{
	mStream.begin(aBaud);
	mStream.setTimeout(50); //warning : response delay seems much higher than expected for some operation (eg writing eeprom)
	readMode();
}

template<class T>
void DynamixelInterfaceImpl<T>::sendPacket(const DynamixelPacket &aPacket)
{
	writeMode();
	// empty receive buffer, in case of a error in previous transaction
	while(mStream.available())
		mStream.read();

	mStream.write(0xFF);
	mStream.write(0xFF);
	mStream.write(aPacket.mID);
	mStream.write(aPacket.mLength);
	mStream.write(aPacket.mInstruction);
	uint8_t n=0;
	if(aPacket.mAddress!=255)
	{
		mStream.write(aPacket.mAddress);
		++n;
	}
	if(aPacket.mDataLength!=255)
	{
		mStream.write(aPacket.mDataLength);
		++n;
	}
	if(aPacket.mLength>(2+n))
	{
		if(aPacket.mIDListSize==0)
		{
			mStream.write(aPacket.mData, aPacket.mLength-2-n);
		}
		else
		{
			uint8_t *ptr=aPacket.mData;
			for(uint8_t i=0; i<aPacket.mIDListSize; ++i)
			{
				mStream.write(aPacket.mIDList[i]);
				mStream.write(ptr, aPacket.mDataLength);
				ptr+=aPacket.mDataLength;
			}
		}
	}
	mStream.write(aPacket.mCheckSum);
	mStream.flush();
	readMode();
}

template<class T>
void DynamixelInterfaceImpl<T>::receivePacket(DynamixelPacket &aPacket, uint8_t answerSize)
{
	static uint8_t buffer[3];
	aPacket.mIDListSize = 0;
	aPacket.mAddress = 255;
	aPacket.mDataLength = 255;
	if (mStream.readBytes(buffer, 2)<2)
	{
		aPacket.mStatus = DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
		return;
	}
	if (buffer[0] != 255 || buffer[1] != 255)
	{
		aPacket.mStatus = DYN_STATUS_COM_ERROR;
		return;
	}
	if (mStream.readBytes(buffer, 3)<3)
	{
		aPacket.mStatus = DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
		return;
	}
	if (aPacket.mID != buffer[0])
	{
		aPacket.mStatus = DYN_STATUS_COM_ERROR;
		return;
	}
	aPacket.mLength = buffer[1];
	if ((aPacket.mLength - 2) != answerSize)
	{
		aPacket.mStatus = DYN_STATUS_COM_ERROR;
		return;
	}
	aPacket.mStatus = buffer[2];
	if (aPacket.mLength>2 && (int)mStream.readBytes(reinterpret_cast<char*>(aPacket.mData), aPacket.mLength - 2)<(aPacket.mLength - 2))
	{
		aPacket.mStatus = DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
		return;
	}
	if (mStream.readBytes(reinterpret_cast<char*>(&(aPacket.mCheckSum)), 1)<1)
	{
		aPacket.mStatus = DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
		return;
	}
	if (aPacket.checkSum() != aPacket.mCheckSum)
	{
		aPacket.mStatus = DYN_STATUS_COM_ERROR | DYN_STATUS_CHECKSUM_ERROR;
	}
}

template<class T>
void DynamixelInterfaceImpl<T>::end()
{
	mStream.end();
}

template class DynamixelInterfaceImpl<HardwareSerial>;
template class DynamixelInterfaceImpl<SoftwareSerial>;

//determine txpin number from hardware serial interface
uint8_t TxPinFromHardwareSerial(const HardwareSerial &aSerial)
{
#if defined ARDUINO_AVR_UNO || defined ARDUINO_AVR_DUEMILANOVE || defined ARDUINO_AVR_NANO || defined ARDUINO_AVR_MEGA2560 || defined ARDUINO_AVR_MEGA || defined ARDUINO_AVR_ADK || defined ARDUINO_AVR_MINI || defined ARDUINO_AVR_ETHERNET || defined ARDUINO_AVR_BT || defined ARDUINO_AVR_PRO
	if((&aSerial)==(&Serial))
		return 1;
#endif
#if defined ARDUINO_AVR_LEONARDO || defined ARDUINO_AVR_MICRO || defined ARDUINO_AVR_ROBOT_CONTROL || defined ARDUINO_ARC32_TOOLS
	if((&aSerial)==(&Serial1))
		return 1;
#endif
#if defined ARDUINO_AVR_MEGA2560 || defined ARDUINO_AVR_MEGA || defined ARDUINO_AVR_ADK 
	if((&aSerial)==(&Serial1))
		return 18;
	if((&aSerial)==(&Serial2))
		return 16;
	if((&aSerial)==(&Serial3))
		return 14;
#endif
	return -1;
}


HardwareDynamixelInterface::HardwareDynamixelInterface(HardwareSerial &aSerial, uint8_t aDirectionPin):
	DynamixelInterfaceImpl(aSerial, TxPinFromHardwareSerial(aSerial), aDirectionPin)
{}

HardwareDynamixelInterface::~HardwareDynamixelInterface()
{}


SoftwareDynamixelInterface::SoftwareDynamixelInterface(uint8_t aRxPin, uint8_t aTxPin, uint8_t aDirectionPin):
	DynamixelInterfaceImpl(mSoftSerial, aTxPin, aDirectionPin),
	mSoftSerial(aRxPin, aTxPin)
{}

SoftwareDynamixelInterface::~SoftwareDynamixelInterface()
{}


template<class T>
void setReadMode(T &aStream, uint8_t aTxPin);
template<class T>
void setWriteMode(T &aStream, uint8_t aTxPin);

#if __AVR__
namespace {
//dirty trick to access protected member
class HardwareSerialAccess:public HardwareSerial
{
	public:
	volatile uint8_t * const ucsrb(){return _ucsrb;}
};
}

template<>
void setReadMode<HardwareSerial>(HardwareSerial &aStream, uint8_t aTxPin)
{
	HardwareSerialAccess &stream=reinterpret_cast<HardwareSerialAccess&>(aStream);
	*(stream.ucsrb()) &= ~_BV(TXEN);
	*(stream.ucsrb()) |= _BV(RXEN);
	*(stream.ucsrb()) |= _BV(RXCIE);
	pinMode(aTxPin, INPUT_PULLUP);
}

template<>
void setWriteMode<HardwareSerial>(HardwareSerial &aStream, uint8_t mTxPin)
{
	HardwareSerialAccess &stream=reinterpret_cast<HardwareSerialAccess&>(aStream);
	*(stream.ucsrb()) &= ~_BV(RXEN);
	*(stream.ucsrb()) &= ~_BV(RXCIE);
	*(stream.ucsrb()) |= _BV(TXEN);
}

#elif __arc__

//Arduino 101 specific code

template<>
void setReadMode<HardwareSerial>(HardwareSerial &aStream, uint8_t mTxPin)
{
	//enable pull up to avoid noise on the line
	pinMode(1, INPUT);
	digitalWrite(1, HIGH);
	// disconnect UART TX and connect UART RX
	SET_PIN_MODE(16, GPIO_MUX_MODE);
	SET_PIN_MODE(17, UART_MUX_MODE);
	pinMode(aTxPin, INPUT_PULLUP);
}

template<>
void setWriteMode<HardwareSerial>(HardwareSerial &aStream, uint8_t mTxPin)
{
	// disconnect UART RX and connect UART TX
	SET_PIN_MODE(17, GPIO_MUX_MODE);
	SET_PIN_MODE(16, UART_MUX_MODE);
}

#endif

template<>
void setReadMode<SoftwareSerial>(SoftwareSerial &aStream, uint8_t mTxPin)
{
	pinMode(mTxPin, INPUT_PULLUP);
	aStream.listen();
}

template<>
void setWriteMode<SoftwareSerial>(SoftwareSerial &aStream, uint8_t mTxPin)
{
	aStream.stopListening();
	pinMode(mTxPin, OUTPUT);
}

