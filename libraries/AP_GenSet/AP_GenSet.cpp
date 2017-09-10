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
#include <AP_BoardConfig/AP_BoardConfig.h>
#include <AP_Vehicle/AP_Vehicle_Type.h>

#if HAL_WITH_UAVCAN
#include "AP_GenSet_UAVCAN.h"
#endif


extern const AP_HAL::HAL& hal;

/*
  AP_GenSet constructor
 */
AP_GenSet::AP_GenSet()
{
    primary_instance = 0;
    num_instances = 0;

    init();
}

/*
  initialise the genset object, loading backend drivers
 */
bool AP_GenSet::init()
{
    AP_GenSet_Backend *new_genset = nullptr;

#if HAL_WITH_UAVCAN
    // If there is place left - allocate one UAVCAN based genset
    if ((AP_BoardConfig::get_can_enable() != 0) && (hal.can_mgr != nullptr))
    {
		printf("Creating AP_GenSet_UAVCAN\n\r");
		new_genset = new AP_GenSet_UAVCAN(*this, status[0]);

		// register new listener at first empty node
		if (hal.can_mgr != nullptr) {
			AP_UAVCAN *ap_uavcan = hal.can_mgr->get_UAVCAN();
			if (ap_uavcan != nullptr) {
				ap_uavcan->register_genset_listener(new_genset, 0);

				if (AP_BoardConfig::get_can_debug() >= 2) {
					hal.console->printf("AP_GENSET_UAVCAN registered\n\r");
				}
			}
		}
    }
#endif

    // check to make sure genset was created
    if (new_genset != nullptr) {
    	num_instances=1;
        drivers[primary_instance] = new_genset;

        return true;
    }
    else
    {
        AP_BoardConfig::sensor_config_error("AP_Genset::init - unable to initialize driver");
        printf("AP_Genset::init - unable to initialize driver\n");

        return false;
    }
}

/*
  call update on all drivers
 */
void AP_GenSet::update(void)
{
	uint8_t instance = primary_instance;
    if (drivers[instance] == nullptr) {
        printf("No GenSet backends available\n");
        return;
    }

    // we have an active driver for this instance
    bool result = drivers[instance]->read();

    if (result != true) {
            printf("AP_GenSet::update() - read() failed\n");
            return;
    }
}

