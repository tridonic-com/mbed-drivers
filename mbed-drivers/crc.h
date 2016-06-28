/***************************************************
*  crc.h
*
*  
*
*  Tridonic GmbH & Co KG
*  Färbergasse 15
*  6851 Dornbirn, Austria
*
***************************************************/

#ifndef YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_CRC_H_
#define YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_CRC_H_

/* Includes -----------------------------------------------------------------*/

#include <cstdint>

/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/
/* Public macro -------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/
namespace mbed
{
    class crc
    {

    public:

        typedef struct
        {
            uint32_t ui32NumOfSectors;
            uint32_t ui32StartAddress;
            uint32_t ui32EndAddress;
            uint32_t ui32SectorSize;
        }SectorInfo_td;

        crc();
        ~crc();
        void _Reset(void);
        int _CalcSingle(uint8_t* pui32StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC);
        int _CalcAccumulate(uint8_t* pui32StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC);

    private:
        static uint32_t InstanceCounter;

    };
} // namespace mbed


#endif /* YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_INTERNALFLASH_H_ */
