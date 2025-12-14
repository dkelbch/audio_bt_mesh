#!/bin/sh
# Usage:
#   ./b.sh LEAD
#   ./b.sh TEAM

ROLE="$1"
ACTION="$2"
if [ -n "$2" ]; then
    ACTION="$2"
else
    ACTION="build"
fi


if [ "$ROLE" = "lead" ]; then
    echo ">>> Building LEAD firmware..."
    idf.py -B build_lead \
        -DSDKCONFIG=sdkconfig.lead \
        -DDEV_ROLE=LEAD \
        set-target esp32s3 \
        $ACTION
elif [ "$ROLE" = "team" ]; then
    echo ">>> Building TEAM firmware..."
    idf.py -B build_team \
        -DSDKCONFIG=sdkconfig.team \
        -DDEV_ROLE=TEAM \
        set-target esp32s3 \
        $ACTION
else
    echo "Usage: ./b.sh {LEAD|TEAM} {build|fullclean}"
    exit 1
fi

