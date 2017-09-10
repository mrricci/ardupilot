#pragma once

#include <AP_HAL/AP_HAL.h>
#include <AP_Param/AP_Param.h>
#include <Filter/Filter.h>
#include <Filter/DerivativeFilter.h>

// maximum number of genset instances
#define GENSET_MAX_INSTANCES 1

// maximum number of drivers. Note that a single driver can provide
// multiple genset instances
#define GENSET_MAX_DRIVERS 1

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
    const GenSet_Status & get_status(void) const { return get_status(_primary); }
    const GenSet_Status & get_status(uint8_t instance) const { return status[instance]; }

    // get last time sample was taken (in ms)
    uint64_t get_last_update(void) const { return get_last_update(_primary); }
    uint64_t get_last_update(uint8_t instance) const { return status[instance].last_timestamp; }

    // settable parameters
    static const struct AP_Param::GroupInfo var_info[];

    // Set the primary genset
    void set_primary_genset(uint8_t primary) { _primary_genset.set_and_save(primary); };

    // register a new genset, claiming a genset slot. If we are out of slots it
    // will panic
    uint8_t register_genset(void);

    // return number of registered gensets
    uint8_t num_instances(void) const { return _num_gensets; }



private:
    // how many drivers do we have?
    uint8_t _num_drivers;
    AP_GenSet_Backend *drivers[GENSET_MAX_DRIVERS];

    // how many gensets do we have?
    uint8_t _num_gensets;

    // what is the primary genset at the moment?
    uint8_t _primary;

    GenSet_Status status[GENSET_MAX_INSTANCES];	//this is the status message

    AP_Int8                             _primary_genset; // primary chosen by user

    //bool _add_backend(AP_GenSet_Backend *backend);
};
