/***************************************************
*  InternalFlash.h
*
*  
*
*  Tridonic GmbH & Co KG
*  Färbergasse 15
*  6851 Dornbirn, Austria
*
***************************************************/

#ifndef YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_INTERNALFLASH_H_
#define YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_INTERNALFLASH_H_

/* Includes -----------------------------------------------------------------*/

#include <cstdint>

/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/
/* Public macro -------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/
namespace mbed
{
    class InternalFlash
    {

    public:

        typedef struct
        {
            uint32_t ui32NumOfSectors;
            uint32_t ui32StartAddress;
            uint32_t ui32EndAddress;
            uint32_t ui32SectorSize;
        }SectorInfo_td;

        InternalFlash();
        ~InternalFlash();
        int _Write(uint32_t ui32Address, const uint8_t* pData, uint32_t ui32DataSize);
        int _read(uint32_t ui32Address, uint8_t* pData, uint32_t pui32DataSize);
        int _EraseSector(uint16_t ui16SectorNumber);
        int _EraseSector(uint32_t ui32Address);
        int _GetSectorInfo(uint32_t ui32SectorNumber, InternalFlash::SectorInfo_td* psInfo);

    private:
        static uint32_t InstanceCounter;

    };
} // namespace mbed


#endif /* YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_INTERNALFLASH_H_ */
