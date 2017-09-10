#pragma once

#include "AP_GenSet.h"

class AP_GenSet_Backend
{
public:
    AP_GenSet_Backend(AP_GenSet &_genset, AP_GenSet::GenSet_Status &_status);
    virtual ~AP_GenSet_Backend(void) {};

    // The read() method is the only one needed in each driver. It
    // should return true when the backend has successfully received a
    // valid packet from the GPS.
    virtual bool read() = 0;

    // callback for UAVCAN messages
    virtual void handle_genset_msg(const AP_GenSet::GenSet_Status &msg) { return ; }

protected:
    AP_GenSet &genset;                   ///< access to frontend (for parameters)
    AP_GenSet::GenSet_Status &status;    ///< public state for this instance

    // semaphore for access to shared frontend data
    //AP_HAL::Semaphore *_sem;
};
