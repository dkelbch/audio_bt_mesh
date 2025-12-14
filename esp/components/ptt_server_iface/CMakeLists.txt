idf_component_register(
    SRCS "ptt_server_iface.c"
    INCLUDE_DIRS "include"
    REQUIRES ptt_vendor_model ptt_server_flood_ctrl
)