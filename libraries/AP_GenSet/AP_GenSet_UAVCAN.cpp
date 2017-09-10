#include <AP_HAL/AP_HAL.h>

#if HAL_WITH_UAVCAN

#include "AP_GenSet_UAVCAN.h"
#include <AP_BoardConfig/AP_BoardConfig.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

extern const AP_HAL::HAL& hal;

#define debug_genset_uavcan(level, fmt, args...) do { if ((level) <= AP_BoardConfig::get_can_debug()) { hal.console->printf(fmt, ##args); }} while (0)

// There is limitation to use only one UAVCAN genset now.

/*
  constructor - registers instance at top GenSet driver
 */
AP_GenSet_UAVCAN::AP_GenSet_UAVCAN(AP_GenSet &_genset, AP_GenSet::GenSet_Status &_status) :
    AP_GenSet_Backend(_genset, _status)
{
    _new_data = false;
    _sem_genset = hal.util->new_semaphore();
}

// For each instance we need to deregister from AP_UAVCAN class
AP_GenSet_UAVCAN::~AP_GenSet_UAVCAN()
{
    if (hal.can_mgr != nullptr) {
        AP_UAVCAN *ap_uavcan = hal.can_mgr->get_UAVCAN();
        if (ap_uavcan != nullptr) {
            ap_uavcan->remove_genset_listener(this);

            debug_genset_uavcan(2, "AP_GenSet_UAVCAN destructed\n\r");
        }
    }
}

// Consume new data and mark it received
bool AP_GenSet_UAVCAN::read(void)
{
    if (_sem_genset->take(0)) {
        if (_new_data) {
            _new_data = false;

            status = _interm_status;
            _sem_genset->give();

            return true;
        }

        _sem_genset->give();
    }
    return false;
}

void AP_GenSet_UAVCAN::handle_genset_msg(const AP_GenSet::GenSet_Status &msg)
{
	if (_sem_genset->take(0)) {
		_interm_status = msg;
		_new_data = true;
        _sem_genset->give();
    }
}

#endif // HAL_WITH_UAVCAN

