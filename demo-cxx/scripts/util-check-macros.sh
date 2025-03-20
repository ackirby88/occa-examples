#!/bin/bash -e
ls .occa/*/*/* | xargs -n1 ./.CheckVarSelfAssign
echo "Check Self Assignment Utility complete."
