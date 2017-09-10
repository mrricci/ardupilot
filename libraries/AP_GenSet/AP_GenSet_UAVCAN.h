#pragma once

#include "AP_GenSet.h"
#include "AP_GenSet_Backend.h"
#include <AP_UAVCAN/AP_UAVCAN.h>

class AP_GenSet_UAVCAN : public AP_GenSet_Backend {
public:
    AP_GenSet_UAVCAN(AP_GenSet &, AP_GenSet::GenSet_Status &);
    ~AP_GenSet_UAVCAN() override;

    bool read() override;

    // This method is called from UAVCAN thread
    virtual void handle_genset_msg(const AP_GenSet::GenSet_Status &msg) override;

    const char *name() const override { return "UAVCAN"; }

private:
    bool _new_data;

    AP_GenSet::GenSet_Status _interm_status;
    AP_HAL::Semaphore *_sem_genset;
};
