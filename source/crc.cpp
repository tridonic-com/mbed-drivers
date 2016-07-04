/*
 * crc.cpp
 *
 *  Created on: 11. Apr. 2016
 *      Author: martin.tomasini2
 */


extern "C"
{
    #include "crc_api.h"
}
//
#include "mbed-drivers/crc.h"


namespace mbed {

    crc::crc(void)
    {
        crc::InstanceCounter++;
        Reset();
    }

    void crc::_Reset(void)
    {
        Reset();
    }

    int crc::_CalcSingle(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
    {
        return CalcSingle(pui8StartAddress, ui32DataSize, pui32CRC);
    }

    int crc::_CalcAccumulate(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC)
    {
        return CalcAccumulate(pui8StartAddress, ui32DataSize, pui32CRC);
    }

    crc::~crc()
    {
        crc::InstanceCounter--;
    }

    uint32_t crc::InstanceCounter = 0;


} // namespace mbed


