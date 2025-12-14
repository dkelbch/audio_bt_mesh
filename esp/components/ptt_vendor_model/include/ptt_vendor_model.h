#ifndef        _PTT_VENDOR_MODEL_H
#define         _PTT_VENDOR_MODEL_H

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