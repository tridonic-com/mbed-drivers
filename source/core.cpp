/*
 * core.cpp
 *
 *  Created on: 29. Juni 2016
 *      Author: martin.tomasini2
 */

extern "C"
{
    #include "core_api.h"
}
//
#include "mbed-drivers/core.h"


namespace mbed {

    core::core(void)
    {
        core::InstanceCounter++;
    }

    void core::_SwitchEndian32(uint32_t* pui32Value)
    {
        SwitchEndian32(pui32Value);

    }

    void core::_SwitchEndian16(uint16_t* pui16Value)
    {
        SwitchEndian16(pui16Value);
    }

    void core::_SwReset()
    {
        SwReset();
    }

    core::~core()
    {
        core::InstanceCounter--;
    }

    uint32_t core::InstanceCounter = 0;


} // namespace mbed


