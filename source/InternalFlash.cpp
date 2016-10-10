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

    int InternalFlash::_Write(uint32_t ui32Address, const uint8_t* pData, uint32_t ui32DataSize)
    {
        return WriteFlash(ui32Address, pData, ui32DataSize);
    }

    int InternalFlash::_read(uint32_t ui32Address, uint8_t* pData, uint32_t ui32DataSize)
    {
        return ReadFlash(ui32Address, pData, ui32DataSize);
    }

    int InternalFlash::_EraseSector(uint16_t ui16SectorNumber)
    {
        return EraseSectorByNumber(ui16SectorNumber);
    }

    int InternalFlash::_EraseSector(uint32_t ui32Address)
    {
        return EraseSectorByAddress(ui32Address);
    }

    int InternalFlash::_GetSectorInfo(uint32_t ui32SectorNumber, InternalFlash::SectorInfo_td* psInfo)
    {
         GetNumOfSectors(&psInfo->ui32NumOfSectors);

        GetSectorStartAdd(ui32SectorNumber, &(psInfo->ui32StartAddress));


        GetSectorEndAdd(ui32SectorNumber, &(psInfo->ui32EndAddress));

        psInfo->ui32SectorSize = psInfo->ui32EndAddress - psInfo->ui32StartAddress + 1;

        return 0;
    }

    InternalFlash::~InternalFlash()
    {
        InstanceCounter--;
    }

    uint32_t InternalFlash::InstanceCounter = 0;

} // namespace mbed


