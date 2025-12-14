if(NOT DEFINED DEV_ROLE)
    set(DEV_ROLE "TEAM")
endif()

idf_component_register(
    SRCS "ptt_mesh_iface.c" "ptt_mesh_iface_${DEV_ROLE}.c"
    INCLUDE_DIRS "include"
    REQUIRES ptt_vendor_model ptt_client_ctrl
)
