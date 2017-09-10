#pragma once

#include <AP_HAL/AP_HAL.h>
#include <AP_Param/AP_Param.h>
#include <Filter/Filter.h>
#include <Filter/DerivativeFilter.h>

// maximum number of genset instances
#define GENSET_MAX_INSTANCES 1

class AP_GenSet_Backend;

class AP_GenSet
{
    friend class AP_GenSet_Backend;

public:
    // constructor
    AP_GenSet();

    // initialise the genset object, loading backend drivers
    bool init();

    // update the genset object, asking backends to push data to the frontend
    void update(void);

    struct GenSet_Status {
        bool fault_level_1;
        bool fault_level_2;
        uint8_t mode;
        uint8_t update_counter;
        uint16_t bus_voltage_mv;
        uint16_t ic_rpm;
        uint64_t last_timestamp;
    };

    // status
    const GenSet_Status & get_status(void) const { return get_status(primary_instance); }
    const GenSet_Status & get_status(uint8_t instance) const { return status[instance]; }

    // get last time sample was taken (in ms)
    uint64_t get_last_update(void) const { return get_last_update(primary_instance); }
    uint64_t get_last_update(uint8_t instance) const { return status[instance].last_timestamp; }

    // settable parameters
    static const struct AP_Param::GroupInfo var_info[];

    // return number of registered gensets
    uint8_t get_num_instances(void) const { return num_instances; }


private:
    // how many drivers do we have?
    GenSet_Status status[GENSET_MAX_INSTANCES];	//this is the status message
    AP_GenSet_Backend *drivers[GENSET_MAX_INSTANCES];

    /// primary GPS instance
    uint8_t primary_instance;

    /// number of GENSET instances present
    uint8_t num_instances;

};
