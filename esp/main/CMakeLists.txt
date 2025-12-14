idf_component_register(
 SRCS "main.c" "app_common.c" "app_lead.c" "app_team.c" "ptt_app.c"
 INCLUDE_DIRS "."
 REQUIRES nvs_flash bt ptt_vendor_model bsp ptt_client_ctrl ptt_mesh_iface freertos ptt_server_iface
)

if(DEFINED DEV_ROLE AND DEV_ROLE STREQUAL "LEAD")
 target_compile_definitions(${COMPONENT_LIB} PUBLIC DEV_ROLE_LEAD=1)
elseif(DEFINED DEV_ROLE AND DEV_ROLE STREQUAL "TEAM")
 target_compile_definitions(${COMPONENT_LIB} PUBLIC DEV_ROLE_TEAM=1)
else()
 target_compile_definitions(${COMPONENT_LIB} PUBLIC DEV_ROLE_TEAM=1)
endif()

