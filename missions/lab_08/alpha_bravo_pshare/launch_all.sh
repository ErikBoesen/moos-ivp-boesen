#!/bin/bash

echo "Launching all..."
pAntler alpha.moos &
pAntler bravo.moos &
pAntler shoreside.moos &

wait
