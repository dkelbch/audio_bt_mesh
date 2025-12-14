idf_component_register(
    SRCS "ptt_client_ctrl.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        main
        bt
        ptt_proto
)