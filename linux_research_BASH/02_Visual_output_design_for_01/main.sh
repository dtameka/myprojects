#!/bin/bash
. ./entry
. ./color
. ./dashboard
. ./result

export argument0=${0}
export argument1=${1}
export argument2=${2}
export argument3=${3}
export argument4=${4}
export arguments=${#}

entry

match_arguments ${argument1}

match_arguments ${argument3}

match_arguments ${argument2}

match_arguments ${argument4}

color_match ${argument1} ${argument2} ${argument3} ${argument4} ${argument0}

color

dashboard

result
