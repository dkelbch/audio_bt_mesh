#include <stdint.h>  
#include <stdbool.h> 

#include "esp_log.h"

#include "ptt_vendor_model.h"

static const char *TAG="PTT_VENDOR";
static bool s_is_lead=false;

void ptt_vendor_model_init(bool is_lead){s_is_lead=is_lead;}
void ptt_send_ptt_down(uint8_t g){ }
void ptt_send_ptt_up(uint8_t g){ }
