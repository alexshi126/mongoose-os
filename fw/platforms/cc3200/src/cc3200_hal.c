/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#include "driverlib/prcm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include "common/platform.h"
#include "common/cs_dbg.h"

#include "simplelink.h"
#include "device.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "mgos_hal.h"

void mgos_dev_system_restart(void) {
  sl_DeviceDisable(); /* Turn off NWP */
  /* Turns out to be not that easy. In particular, using *Reset functions is
   * not a good idea.
   * https://e2e.ti.com/support/wireless_connectivity/f/968/p/424736/1516404
   * Instead, the recommended way is to enter hibernation with immediate wakeup.
   */
  MAP_PRCMHibernateIntervalSet(328 /* 32KHz ticks, 100 ms */);
  MAP_PRCMHibernateWakeupSourceEnable(PRCM_HIB_SLOW_CLK_CTR);
  MAP_PRCMHibernateEnter();
}

/* There is no true random source on CC3200. */
int mg_ssl_if_mbed_random(void *ctx, unsigned char *buf, size_t len) {
  for (size_t i = 0; i < len; i++) {
    buf[i] = rand();
  }
  return 0;
}

#ifndef MGOS_HAVE_WIFI
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *e) {
  (void) e;
}
void SimpleLinkWlanEventHandler(SlWlanEvent_t *e) {
  (void) e;
}
#endif
