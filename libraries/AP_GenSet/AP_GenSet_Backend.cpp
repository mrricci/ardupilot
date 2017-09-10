#include "AP_GenSet.h"
#include "AP_GenSet_Backend.h"

extern const AP_HAL::HAL& hal;

// constructor
AP_GenSet_Backend::AP_GenSet_Backend(AP_GenSet &_genset, AP_GenSet::GenSet_Status &_status) :
    genset(_genset),
    status(_status)
{
    //_sem = hal.util->new_semaphore();
	status.update_counter=0;
}
