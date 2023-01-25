#!/bin/bash
. ./entry
. ./dashboard
. ./result

export arguments=${#}
export first_argument=${1}

entry

dashboard

result
