#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
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
