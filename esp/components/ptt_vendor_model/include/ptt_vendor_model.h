#ifndef        _PTT_VENDOR_MODEL_H
#define         _PTT_VENDOR_MODEL_H

#include <stdint.h>
#include <stdbool.h>

/* Fixed LEAD unicast address for initial bring-up */
#ifndef PTT_LEAD_UNICAST_ADDR
#define PTT_LEAD_UNICAST_ADDR 0x0001
#endif


#ifdef __cplusplus
extern "C" {
#endif         // __cplusplus

void ptt_vendor_model_init(bool is_lead);
void ptt_send_ptt_down(uint8_t group_idx);
void ptt_send_ptt_up(uint8_t group_idx);

#ifdef __cplusplus
 }
#endif         // __cplusplus
#endif         //_PTT_VENDOR_MODEL_H
