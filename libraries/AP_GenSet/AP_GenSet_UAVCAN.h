//
//  UAVCAN GenSet driver
//

#pragma once

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>

#include "AP_GenSet.h"
#include "AP_GenSet_Backend.h"

class AP_GenSet_UAVCAN : public AP_GenSet_Backend {
public:
    AP_GenSet_UAVCAN(AP_GenSet &_genset, AP_GenSet::GenSet_Status &_status);
    ~AP_GenSet_UAVCAN() override;

    bool read() override;

    // This method is called from UAVCAN thread
    void handle_genset_msg(const AP_GenSet::GenSet_Status &msg) override;

private:
    bool _new_data;

    AP_GenSet::GenSet_Status _interm_status;
    AP_HAL::Semaphore *_sem_genset;
};
