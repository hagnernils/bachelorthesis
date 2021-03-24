#!/usr/bin/env python

"""This program shows `hyperfine` benchmark results as a histogram."""

import argparse
import json
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import LogLocator, LogFormatter

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("file", help="JSON file with benchmark results")
parser.add_argument("--title", help="Plot title")
parser.add_argument(
    "--labels", help="Comma-separated list of entries for the plot legend"
)
parser.add_argument("--bins", help="Number of bins (default: auto)")
parser.add_argument(
    "--type", help="Type of histogram (*bar*, barstacked, step, stepfilled)"
)
parser.add_argument(
    "-o", "--output", help="Save image to the given filename."
)

args = parser.parse_args()

with open(args.file) as f:
    results = json.load(f)["results"]

if args.labels:
    labels = args.labels.split(",")
else:
    labels = [b["command"] for b in results]
all_times = [b["times"] for b in results]
all_params = [b["parameters"] for b in results]

# group results by scene
scenes = set([p["modelName"] for p in all_params])
# group results by split method
split_methods = set([p["medianSplit"] for p in all_params])
num_max_leaf_values = set([int(p["maxLeafPrimitives"]) for p in all_params])

# the leaf values are on the x axis
param_selector = lambda b: b["parameters"]


def max_bvh_leaf_selector(results):
    return [p["maxLeafPrimitives"] for p in map(param_selector, results)]

mean_execution_time = lambda results: [p["mean"] for p in results]
min_max_execution_time = lambda results: [(p["min"], p["max"]) for p in results]


fig, ax = plt.subplots()
x_centers = np.arange(len(num_max_leaf_values))
width = 0.75
width_per_scene = width/len(scenes)
width_per_method = width_per_scene / len(split_methods)

method_labels = ["midpoint split", "median split"]

for scene_index, scene in enumerate(scenes):
    for method_index, method in enumerate(split_methods):
        matching_results_method = [b for b in results if (b["parameters"]["modelName"] == scene and b["parameters"]["medianSplit"] == method)]
        xpos_scene_offset = width_per_scene * scene_index
        xpos_method_offset = width_per_method * method_index
        label = "Scene {}, {}".format(scene,  method_labels[method_index])
        ax.bar(x_centers + xpos_scene_offset + xpos_method_offset - width_per_scene * len(scenes) / 2, mean_execution_time(matching_results_method), width_per_method, bottom=0.001, align='edge', label=label)

plt.legend(prop={"family": ["Source Code Pro", "Fira Mono", "Courier New"]})

plt.ylabel("Execution time in seconds")
plt.xlabel("Maximum number of primitives per BVH leaf")
ax.set_yscale('log')

ax.yaxis.set_major_formatter(LogFormatter(2))
#ax.yaxis.set_minor_formatter(LogFormatter())
#ax.set_yticks()
ax.set_ylim(bottom=0.1, auto=True)
ax.yaxis.set_major_locator(LogLocator())
ax.yaxis.set_minor_locator(LogLocator())

ax.set_xticks(x_centers)
ax.set_xticklabels(num_max_leaf_values)

if args.title:
    plt.title(args.title)

if args.output:
    plt.savefig(args.output)
else:
    plt.show()
