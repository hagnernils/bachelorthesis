#!/usr/bin/env bash

COMMAND='../cmake-build-release/bachelorthesis {maxLeafPrimitives} {numRays} 10 {medianSplit} {modelName}'

# as warmup
hyperfine -w 3 -r 2 -L maxLeafPrimitives 1 -L numRays 1000 -L medianSplit 0 -L modelName spheres_highres "${COMMAND}"
# actual runs

# benchmark for different split methods for different sceenes and number of minimum leaves
hyperfine -r 5 -L maxLeafPrimitives 1,2,3,4,5,10,50 -L numRays 10000 -L medianSplit 0,1 -L modelName spheres_lowres,spheres_midres,spheres_highres,ERNST2019 "${COMMAND}" --export-json analysis/BVH_leaf_timings.json
