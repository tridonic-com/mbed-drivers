/*
 * Flash.cpp
 *
 *  Created on: 11. Apr. 2016
 *      Author: martin.tomasini2
 */


extern "C"
{
    #include "internal_flash_api.h"
}
//
#include "mbed-drivers/InternalFlash.h"


namespace mbed {

    InternalFlash::InternalFlash(void)
    {
        InstanceCounter++;
    }

    int InternalFlash::_Write(uint32_t ui32Address, uint8_t* pData, uint32_t ui32DataSize)
    {
        return WriteFlash(ui32Address, pData, ui32DataSize);
    }

    int InternalFlash::_read(uint32_t ui32Address, uint8_t* pData, uint32_t ui32DataSize)
    {
        return ReadFlash(ui32Address, pData, ui32DataSize);
    }

    int InternalFlash::_EraseSector(uint32_t ui32SectorNumber)
    {
        return EraseSector(ui32SectorNumber);
    }

    int InternalFlash::_GetSectorInfo(uint32_t ui32SectorNumber, InternalFlash::SectorInfo_td* psInfo)
    {
        if(0 != GetNumOfSectors((uint32_t*)&(psInfo->ui32NumOfSectors)))
        {
            // Error
            return -1;
        }
        if(0 != GetSectorStartAdd(ui32SectorNumber, &(psInfo->ui32StartAddress)))
        {
            // Error
            return -1;
        }
        if(0 != GetSectorEndAdd(ui32SectorNumber, &(psInfo->ui32EndAddress)))
        {
            // Error
            return -1;
        }
        if(0 != GetNumOfSectors(&(psInfo->ui32NumOfSectors)))
        {
            // Error
            return -1;
        }

        return 0;
    }

    InternalFlash::~InternalFlash()
    {
        InstanceCounter--;
    }

    uint32_t InternalFlash::InstanceCounter = 0;

} // namespace mbed


