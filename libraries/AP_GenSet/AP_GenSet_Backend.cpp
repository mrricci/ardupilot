#include "AP_GenSet.h"
#include "AP_GenSet_Backend.h"

#define GENSET_BACKEND_DEBUGGING 1

#if GENSET_BACKEND_DEBUGGING
 # define Debug(fmt, args ...)  do {hal.console->printf("%s:%d: " fmt "\n", __FUNCTION__, __LINE__, ## args); hal.scheduler->delay(1); } while(0)
#else
 # define Debug(fmt, args ...)
#endif

extern const AP_HAL::HAL& hal;

// constructor
AP_GenSet_Backend::AP_GenSet_Backend(AP_GenSet &_genset, AP_GenSet::GenSet_Status &_status) :
    genset(_genset),
    status(_status)
{
	status.update_counter=0;
}
