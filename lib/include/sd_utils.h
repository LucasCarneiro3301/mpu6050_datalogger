#include "ff.h"
#include "diskio.h"
#include "f_util.h"
#include "hw_config.h"
#include "my_debug.h"
#include "rtc.h"
#include "sd_card.h"

sd_card_t *sd_get_by_name(const char *const name);
void run_setrtc();
void run_format();
int run_mount();
int run_unmount();
void run_getfree();
void run_ls();
void run_cat();