#include "uavcan_batterystatus_publisher.h"

#include <modules/uavcan/uavcan.h>
#include <common/ctor.h>
#include <modules/worker_thread/worker_thread.h>

#ifndef UAVCAN_BATTERYSTATUS_PUBLISHER_WORKER_THREAD
#error Please define UAVCAN_BATTERYSTATUS_PUBLISHER_WORKER_THREAD in framework_conf.h.
#endif

#define WT UAVCAN_BATTERYSTATUS_PUBLISHER_WORKER_THREAD
WORKER_THREAD_DECLARE_EXTERN(WT)

static struct com_matternet_equipment_power_BatteryStatus_s battery_status;
static struct worker_thread_timer_task_s battery_status_publisher_task;

static void battery_status_publisher_task_func(struct worker_thread_timer_task_s* task);

struct com_matternet_equipment_power_BatteryStatus_s* com_matternet_publisher_battery_status_message(void) {
    return &battery_status;
}

RUN_AFTER(UAVCAN_INIT) {
//    battery_status.temperature_probe_degC[0] = 0;
//    battery_status.cell_V[0]               = 0;

    worker_thread_add_timer_task(&WT, &battery_status_publisher_task, battery_status_publisher_task_func, NULL, S2ST(10), true);
}

static void battery_status_publisher_task_func(struct worker_thread_timer_task_s* task) {
    (void)task;

    uavcan_broadcast(0, &com_matternet_equipment_power_BatteryStatus_descriptor, CANARD_TRANSFER_PRIORITY_LOW, &battery_status);
}
