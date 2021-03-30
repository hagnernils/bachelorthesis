#!/usr/bin/env bash

COMMAND='../cmake-build-release/bachelorthesis {maxLeafPrimitives} {numRays} 10 {medianSplit} {uniformSampling} {modelName} {file}.txt'

# for ernst
#for numrays in {1..14}
#do
#    for run in {1..10}
#    do
#      echo "$((2 ** numrays))" "$run"
#      ../cmake-build-release/bachelorthesis 1 "$((2 ** numrays))" 10 0 0 ERNST_truescale analysis/ERNST_truescale.txt > /dev/null 2>&1
#    done
#done

# for differential to square
#for numrays in {16..18}
#do
#  for run in {1..1000}
#  do
#      echo "$((2 ** numrays))" "$run"
#    ../cmake-build-release/bachelorthesis 5 "$((2 ** numrays))" 10 0 1 diffSquare analysis/diffSquareU.txt > /dev/null 2>&1
#    ../cmake-build-release/bachelorthesis 5 "$((2 ** numrays))" 10 0 0 diffSquare analysis/diffSquareC.txt > /dev/null 2>&1
#  done
#done


# for sphere convergence
#for numrays in {1..10}
#do
#  for run in {1..100}
#  do
#      echo "$((2 ** numrays))" "$run"
#    ../cmake-build-release/bachelorthesis 1 "$((numrays))" 10 0 1 spheres_highres analysis/spheresU.txt > /dev/null 2>&1
#    ../cmake-build-release/bachelorthesis 1 "$((numrays))" 10 0 0 spheres_highres analysis/spheresC.txt > /dev/null 2>&1
#  done
#done


# benchmark for different split methods for ERNST and number of minimum leaves
#hyperfine -r 5 -L maxLeafPrimitives 1 -L numRays 100,200,300,400,500 -L medianSplit 2 -L uniformSampling 0 -L modelName ERNST_truescale -L file out "${COMMAND}" --export-json analysis/BVH_leaf_timings_exhaustive.json
# exhaustive intersection benchmark
hyperfine -r 5 -L maxLeafPrimitives 1 -L numRays 1000 -L medianSplit 2 -L uniformSampling 0 -L modelName ERNST_truescale -L file out "${COMMAND}" --export-json analysis/BVH_leaf_timings_exhaustive.json