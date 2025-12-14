/*
 * gt_aud_mesh_prot.h
 * Common definitions for the GT Audio Mesh PTT vendor model.
 */
#ifndef GT_AUD_MESH_PROT_H
#define GT_AUD_MESH_PROT_H

#include <stdint.h>
#include "esp_ble_mesh_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GT_AUD_MESH_COMPANY_ID                0x02E5
#define GT_AUD_MESH_MODEL_ID_CLIENT           0x0000
#define GT_AUD_MESH_MODEL_ID_SERVER           0x0001

#define GT_AUD_MESH_OP_PTT_SEND               ESP_BLE_MESH_MODEL_OP_3(0x00, GT_AUD_MESH_COMPANY_ID)
#define GT_AUD_MESH_OP_PTT_STATUS             ESP_BLE_MESH_MODEL_OP_3(0x01, GT_AUD_MESH_COMPANY_ID)

#define GT_AUD_MESH_LEN_PTT_SEND              2
#define GT_AUD_MESH_LEN_PTT_STATUS            1

typedef enum {
    GT_AUD_MESH_PTT_CMD_REQUEST = 0x00,
    GT_AUD_MESH_PTT_CMD_RELEASE = 0x01,
    GT_AUD_MESH_PTT_CMD_UNDEF   = 0xFF
} gt_aud_mesh_ptt_cmd_t;

typedef enum {
    GT_AUD_MESH_PTT_RESULT_REJECT  = 0x00,
    GT_AUD_MESH_PTT_RESULT_GRANTED = 0x01,
    GT_AUD_MESH_PTT_RESULT_BUSY    = 0x02,
} gt_aud_mesh_ptt_result_t;

typedef struct __attribute__((packed)) {
    uint8_t cmd;
    uint8_t flags;
} gt_aud_mesh_ptt_send_t;

typedef struct __attribute__((packed)) {
    uint8_t result;
} gt_aud_mesh_ptt_status_t;

#define GT_AUD_MESH_PTT_IS_GRANTED(_res) ((_res) == GT_AUD_MESH_PTT_RESULT_GRANTED)
#define GT_AUD_MESH_PTT_IS_BUSY(_res)    ((_res) == GT_AUD_MESH_PTT_RESULT_BUSY)

#ifdef __cplusplus
}
#endif
#endif /* GT_AUD_MESH_PROT_H */
