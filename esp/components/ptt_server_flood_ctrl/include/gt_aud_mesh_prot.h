/*
 * gt_aud_mesh_prot.h
 *
 * Vendor model identifiers + opcodes for GT Audio Mesh PTT.
 * NOTE: Payload types are defined in ptt_proto.h.
 */
#ifndef GT_AUD_MESH_PROT_H
#define GT_AUD_MESH_PROT_H

#include "esp_ble_mesh_defs.h"
#include "ptt_proto.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Bluetooth SIG company ID used for the vendor model (Espressif here) */
#define GT_AUD_MESH_COMPANY_ID                0x02E5

/* Vendor model IDs for client and server roles */
#define GT_AUD_MESH_MODEL_ID_CLIENT           0x0000
#define GT_AUD_MESH_MODEL_ID_SERVER           0x0001

/* 3-byte vendor opcodes (1 byte opcode + 2 bytes company ID, little endian) */
#define GT_AUD_MESH_OP_PTT_SEND               ESP_BLE_MESH_MODEL_OP_3(0x00, GT_AUD_MESH_COMPANY_ID)
#define GT_AUD_MESH_OP_PTT_STATUS             ESP_BLE_MESH_MODEL_OP_3(0x01, GT_AUD_MESH_COMPANY_ID)

/* Minimum expected message lengths (bytes) */
#define GT_AUD_MESH_LEN_PTT_SEND              2
#define GT_AUD_MESH_LEN_PTT_STATUS            1

#ifdef __cplusplus
}
#endif

#endif /* GT_AUD_MESH_PROT_H */
