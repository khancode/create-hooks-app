#!/bin/bash

HOOKS=$(jq -c '.HOOKS[]' config.json)

for hook in $HOOKS; do
  HOOK_C_FILENAME=$(echo $hook | jq -r '.HOOK_C_FILENAME')
  make build-one-hook HOOK_C_FILENAME=$HOOK_C_FILENAME
done
