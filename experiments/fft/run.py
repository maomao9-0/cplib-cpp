#!/usr/bin/env python3
"""Build reproducible scalar FFT experiments; run each suite sequentially."""

import argparse
import csv
import os
from pathlib import Path
import platform
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
DEST = Path(__file__).resolve().parent
SOURCES = {
    "transform": "benchmark",
    "convolution": "convolution",
    "split": "split",
    "accuracy": "accuracy",
    "validate": "validate",
    "backends": "backends",
}
VARIANTS = {
    "transform": [
        "baseline", "r2-tree", "r2-trig", "r2-double-trig", "r2-fma",
        "r4-tree", "r4-fma", "split", "split-fma", "long-double", "production",
    ],
    "convolution": ["baseline", "r2", "r4", "split", "r4-fma", "production"],
    "split": ["production"],
    "accuracy": ["reference"],
    "validate": ["reference"],
    "backends": ["comparison"],
}
HEADERS = {
    "backends": [
        "coefficient_type", "log2_input_n", "baseline_ms", "production_ms", "speedup",
    ],
    "transform": [
        "kind", "variant", "log2_n", "first_ms", "median_ms", "relative_l2", "max_abs",
    ],
    "convolution": [
        "kernel", "method", "log2_input_n", "pattern", "median_ms", "max_abs", "rms",
        "bad_sampled_coefficients",
    ],
    "split": [
        "kernel", "log2_input_n", "modulus", "pattern", "mode", "max_lane_error",
        "bad_sampled_lanes", "total_ms",
    ],
    "accuracy": ["variant", "log2_n", "pattern", "relative_l2", "max_abs"],
}

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--suite", choices=SOURCES, required=True)
parser.add_argument(
    "--cpu", type=int, default=0,
    help="Pin the experiment to one logical CPU (default: 0)",
)
parser.add_argument(
    "--native", action="store_true",
    help="Use -O3 -march=native instead of portable -O2",
)
parser.add_argument(
    "--variants", nargs="+", help="Override variants for transform/convolution/split",
)
args = parser.parse_args()
os.sched_setaffinity(0, {args.cpu})
flags = [
    "-std=c++20", "-O3" if args.native else "-O2",
    "-fno-tree-vectorize", "-fno-tree-slp-vectorize", "-ffp-contract=off",
    "-I" + str(ROOT),
]
if args.native:
    flags.append("-march=native")
source = SOURCES[args.suite]
variants = args.variants or VARIANTS[args.suite]
suffix = "native" if args.native else "generic"
output = DEST / (
    "validation.txt" if args.suite == "validate" else f"{args.suite}-{suffix}.csv"
)
compiler = subprocess.check_output(["g++", "--version"], text=True).splitlines()[0]
cpu = next(
    line for line in Path("/proc/cpuinfo").read_text().splitlines()
    if line.startswith("model name")
)
metadata = (
    f"Compiler: {compiler}\nFlags: {' '.join(flags)}\n"
    f"Platform: {platform.platform()}\nCPU affinity: {args.cpu}\n{cpu}\n"
)
(DEST / f"{args.suite}-{suffix}.env.txt").write_text(metadata)

with tempfile.TemporaryDirectory(prefix="cplib-fft-") as tmp:
    binary = str(Path(tmp) / source)
    subprocess.run(
        ["g++", *flags, str(DEST / (source + ".cpp")), "-o", binary], check=True,
    )
    with output.open("w", newline="") as out:
        writer = csv.writer(out, lineterminator="\n")
        if args.suite in HEADERS:
            writer.writerow(HEADERS[args.suite])
        for variant in variants:
            if args.suite == "transform":
                # A new process at every size measures truly cold initialization.
                commands = [
                    [binary, variant, str(lg), str(lg)] for lg in range(8, 22)
                ]
            elif args.suite in ["convolution", "split"]:
                commands = [[binary, variant]]
            else:
                commands = [[binary]]
            for command in commands:
                result = subprocess.run(
                    command, text=True, capture_output=True, check=True,
                )
                if args.suite == "validate":
                    out.write(result.stdout)
                else:
                    writer.writerows(csv.reader(result.stdout.splitlines()))
                out.flush()
            print(f"{args.suite}: {variant} finished", flush=True)
print(output)
