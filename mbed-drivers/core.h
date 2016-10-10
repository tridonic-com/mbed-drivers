/***************************************************
*  core.h
*
*  
*
*  Tridonic GmbH & Co KG
*  Färbergasse 15
*  6851 Dornbirn, Austria
*
***************************************************/

#ifndef YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_CORE_H_
#define YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_CORE_H_

/* Includes -----------------------------------------------------------------*/
#include <cstdint>

/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/
/* Public macro -------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/
namespace mbed
{
    class core
    {

    public:
        core();
        ~core();
        static void _SwitchEndian32(uint32_t* pui32Value);
        static void _SwitchEndian16(uint16_t* pui16Value);
        static void _SwReset();
    private:
        static uint32_t InstanceCounter;

    };
} // namespace mbed


#endif /* YOTTA_MODULES_MBED_DRIVERS_MBED_DRIVERS_CORE_H_ */
