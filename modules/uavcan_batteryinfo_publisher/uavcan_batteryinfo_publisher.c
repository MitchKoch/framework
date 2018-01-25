#include "uavcan_batteryinfo_publisher.h"

#include <modules/uavcan/uavcan.h>
#include <common/ctor.h>
#include <modules/worker_thread/worker_thread.h>

#ifndef UAVCAN_BATTERYINFO_PUBLISHER_WORKER_THREAD
#error Please define UAVCAN_BATTERYINFO_PUBLISHER_WORKER_THREAD in framework_conf.h.
#endif

#define WT UAVCAN_BATTERYINFO_PUBLISHER_WORKER_THREAD
WORKER_THREAD_DECLARE_EXTERN(WT)

static struct uavcan_equipment_power_BatteryInfo_s battery_info;
static struct worker_thread_timer_task_s battery_info_publisher_task;

static void battery_info_publisher_task_func(struct worker_thread_timer_task_s* task);

struct uavcan_equipment_power_BatteryInfo_s* uavcan_publisher_battery_info_message(void) {
    return &battery_info;
}

RUN_AFTER(UAVCAN_INIT) {
    battery_info.temperature              = 0;
    battery_info.voltage                  = 0;
    battery_info.current                  = 0;
    battery_info.average_power_10sec      = 0;
    battery_info.remaining_capacity_wh    = 0;
    battery_info.full_charge_capacity_wh  = 0;
    battery_info.hours_to_full_charge     = 0;
    battery_info.status_flags             = 0;
    battery_info.state_of_health_pct      = 0;
    battery_info.state_of_charge_pct      = 0;
    battery_info.state_of_charge_pct_stdev = 50;
    battery_info.battery_id               = 0;
    battery_info.model_instance_id        = 0;
    battery_info.model_name_len           = 0;

    worker_thread_add_timer_task(&WT, &battery_info_publisher_task, battery_info_publisher_task_func, NULL, S2ST(10), true);
}

static void battery_info_publisher_task_func(struct worker_thread_timer_task_s* task) {
    (void)task;

    uavcan_broadcast(0, &uavcan_equipment_power_BatteryInfo_descriptor, CANARD_TRANSFER_PRIORITY_LOW, &battery_info);
}
