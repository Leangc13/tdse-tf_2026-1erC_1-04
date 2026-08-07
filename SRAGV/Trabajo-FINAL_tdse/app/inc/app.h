#ifndef APP_H_
#define APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void app_init(void);
void app_update(void);

extern volatile uint32_t g_app_tick_cnt;

#ifdef __cplusplus
}
#endif

#endif /* APP_H_ */
