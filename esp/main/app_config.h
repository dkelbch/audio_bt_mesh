#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

#if defined(DEV_ROLE_LEAD)
#define IS_LEAD_ROLE 1
#define IS_TEAM_ROLE 0
#elif defined(DEV_ROLE_TEAM)
#define IS_LEAD_ROLE 0
#define IS_TEAM_ROLE 1
#else
#define IS_LEAD_ROLE 0
#define IS_TEAM_ROLE 1
#endif

#endif /* _APP_CONFIG_H */
