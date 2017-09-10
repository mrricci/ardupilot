#include "AP_GenSet_Backend.h"

extern const AP_HAL::HAL& hal;

// constructor
AP_GenSet_Backend::AP_GenSet_Backend(AP_GenSet &_genset, AP_GenSet::GenSet_Status &_status) :
    genset(_genset),
    status(_status)
{
    //_sem = hal.util->new_semaphore();
}

/*
  copy latest data to the frontend from a backend
 */
/*
void AP_GenSet_Backend::_copy_to_frontend(uint8_t instance, uint8_t mode, uint64_t _last_timestamp)
{
    if (instance >= _frontend._num_gensets) {
        return;
    }
    _frontend.gensets[instance].mode = mode;
    _frontend.gensets[instance].last_update_ms = _last_timestamp/1000;
}
*/
