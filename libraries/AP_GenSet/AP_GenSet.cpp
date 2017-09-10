/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *       APM_GenSet.cpp - genset driver
 *
 */
#include "AP_GenSet.h"

#include <utility>

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include <AP_Math/AP_Math.h>
#include <AP_BoardConfig/AP_BoardConfig.h>
#include <AP_Vehicle/AP_Vehicle_Type.h>

#if HAL_WITH_UAVCAN
#include "AP_GenSet_UAVCAN.h"
#endif


extern const AP_HAL::HAL& hal;

// table of user settable parameters
const AP_Param::GroupInfo AP_GenSet::var_info[] = {
    // @Param: PRIMARY
    // @DisplayName: Primary genset
    // @Description: This selects which genset will be the primary if multiple gensets are found
    // @Values: 0:FirstGenSet,1:2ndGenSet
    // @User: Advanced
    AP_GROUPINFO("PRIMARY", 2, AP_GenSet, _primary_genset, 0),

    AP_GROUPEND
};

/*
  AP_GenSet constructor
 */
AP_GenSet::AP_GenSet()
{
    AP_Param::setup_object_defaults(this, var_info);
}
/*
bool AP_GenSet::_add_backend(AP_GenSet_Backend *backend)
{
    if (!backend) {
        return false;
    }
    if (_num_drivers >= GENSET_MAX_DRIVERS) {
        AP_HAL::panic("Too many genset drivers");
    }
    drivers[_num_drivers++] = backend;
    return true;
}
*/
/*
  macro to add a backend with check for too many gensets
 We don't try to start more than the maximum allowed
 */
/*
#define ADD_BACKEND(backend) \
    do { _add_backend(backend);     \
       if (_num_drivers == GENSET_MAX_DRIVERS || \
          _num_gensets == GENSET_MAX_INSTANCES) { \
          return; \
       } \
    } while (0)
*/
/*
  initialise the genset object, loading backend drivers
 */
bool AP_GenSet::init()
{
#if HAL_WITH_UAVCAN
    // If there is place left - allocate one UAVCAN based genset
    if ((AP_BoardConfig::get_can_enable() != 0) && (hal.can_mgr != nullptr))
    {
        if(_num_drivers < GENSET_MAX_DRIVERS && _num_gensets < GENSET_MAX_INSTANCES)
        {
            printf("Creating AP_GenSet_UAVCAN\n\r");
            drivers[_num_drivers] = new AP_GenSet_UAVCAN(*this, status[0]);

            //TODO where do i call register_genset()?? - this is done in AP_GenSet_UAVCAN
            /*
			if (hal.can_mgr != nullptr) {
				AP_UAVCAN *ap_uavcan = hal.can_mgr->get_UAVCAN();
				if (ap_uavcan != nullptr) {
					ap_uavcan->register_genset_listener(drivers[_num_drivers], 0);

					if (AP_BoardConfig::get_can_debug() >= 2) {
						hal.console->printf("AP_GENSET_UAVCAN registered\n\r");
					}
				}
			}*/

            _num_drivers++;
        }
    }
#endif


    if (_num_drivers == 0 || _num_gensets == 0 || drivers[0] == nullptr) {
        AP_BoardConfig::sensor_config_error("Genset: unable to initialize driver");
        printf("No GenSet backends available\n");

        return false;
    }

    return true;
}

/*
  call update on all drivers
 */
void AP_GenSet::update(void)
{
    if (_num_drivers == 0 || _num_gensets == 0 || drivers[0] == nullptr) {
        printf("No GenSet backends available\n");
    }

    // copy from backend
    for (uint8_t i=0; i<_num_drivers; i++) {
        drivers[i]->read();
    }

    // choose primary genset
    if (_primary_genset >= 0 && _primary_genset < _num_gensets) {
        _primary = _primary_genset;
    } else {
        _primary = 0;
    }
}

/* register a new genset, claiming a genset slot. If we are out of
   slots it will panic
*/
uint8_t AP_GenSet::register_genset(void)
{
    printf("    AP_GenSet::register_genset(void) _num_gensets:%d+1\n\r", _num_gensets);
    if (_num_gensets >= GENSET_MAX_INSTANCES) {
        AP_HAL::panic("Too many gensets");
        printf("    ERROR!!!!!\n\r");
    }
    return _num_gensets++;
}

