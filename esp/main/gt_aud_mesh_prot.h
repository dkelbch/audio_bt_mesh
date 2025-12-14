/*
 * gt_aud_mesh_common.h
 *
 * Common definitions for the GT Audio Mesh PTT vendor model.
 * Shared between client and server implementations.
 */
#ifndef     GT_AUD_MESH_PROT_H
#define        GT_AUD_MESH_PROT_H

#include "esp_ble_mesh_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  Company and model identifiers                                             */
/* -------------------------------------------------------------------------- */

/* Bluetooth SIG company ID used for the vendor model (here: Espressif) */
#define GT_AUD_MESH_COMPANY_ID                0x02E5

/* Vendor model IDs for client and server roles */
#define GT_AUD_MESH_MODEL_ID_CLIENT           0x0000
#define GT_AUD_MESH_MODEL_ID_SERVER           0x0001

/* -------------------------------------------------------------------------- */
/*  Opcodes                                                                   */
/* -------------------------------------------------------------------------- */
/*
 * All opcodes are 3-byte vendor opcodes:
 *   - 1 byte: opcode value (0x00..0x7F)
 *   - 2 bytes: company ID (little endian)
 *
 * For now we map the generic SEND/STATUS to the PTT use case:
 *   - GT_AUD_MESH_OP_PTT_SEND   : client -> server (request / release)
 *   - GT_AUD_MESH_OP_PTT_STATUS : server -> client (grant / reject / busy)
 */

#define GT_AUD_MESH_OP_PTT_SEND               \
    ESP_BLE_MESH_MODEL_OP_3(0x00, GT_AUD_MESH_COMPANY_ID)

#define GT_AUD_MESH_OP_PTT_STATUS             \
    ESP_BLE_MESH_MODEL_OP_3(0x01, GT_AUD_MESH_COMPANY_ID)

/* Minimum expected message lengths (in bytes) */
#define GT_AUD_MESH_LEN_PTT_SEND              2
#define GT_AUD_MESH_LEN_PTT_STATUS            1

/* -------------------------------------------------------------------------- */
/*  PTT payload definitions                                                   */
/* -------------------------------------------------------------------------- */

/* PTT command types carried in GT_AUD_MESH_OP_PTT_SEND */
typedef enum {
    GT_AUD_MESH_PTT_CMD_REQUEST = 0x00,  /* Request to become the talker    */
    GT_AUD_MESH_PTT_CMD_RELEASE = 0x01,  /* Release the floor (stop talking)*/
    GT_AUD_MESH_PTT_CMD_UNDEF   = 0xFF
} gt_aud_mesh_ptt_cmd_t;

/* Result codes returned in GT_AUD_MESH_OP_PTT_STATUS */
typedef enum {
    GT_AUD_MESH_PTT_RESULT_REJECT  = 0x00, /* Floor not granted / busy      */
    GT_AUD_MESH_PTT_RESULT_GRANTED = 0x01, /* Floor granted                 */
    GT_AUD_MESH_PTT_RESULT_BUSY    = 0x02, /* Floor held by another node   */
} gt_aud_mesh_ptt_result_t;

/* Payload for GT_AUD_MESH_OP_PTT_SEND */
typedef struct __attribute__((packed)) {
    uint8_t cmd;      /* gt_aud_mesh_ptt_cmd_t value                */
    uint8_t flags;    /* reserved for future use (prio, role, etc.) */
} gt_aud_mesh_ptt_send_t;

/* Payload for GT_AUD_MESH_OP_PTT_STATUS */
typedef struct __attribute__((packed)) {
    uint8_t result;   /* gt_aud_mesh_ptt_result_t value             */
} gt_aud_mesh_ptt_status_t;

/* -------------------------------------------------------------------------- */
/*  Helper macros                                                             */
/* -------------------------------------------------------------------------- */

/* Simple helpers to check results */
#define GT_AUD_MESH_PTT_IS_GRANTED(_res) \
    ((_res) == GT_AUD_MESH_PTT_RESULT_GRANTED)

#define GT_AUD_MESH_PTT_IS_BUSY(_res) \
    ((_res) == GT_AUD_MESH_PTT_RESULT_BUSY)

#ifdef __cplusplus
}
#endif
#endif     //GT_AUD_MESH_PROT_H
