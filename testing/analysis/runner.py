import subprocess
import os
import csv

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
TRACE_DIR = os.path.join(BASE_DIR, "data", "generated", "abbadingo", "traces")
QUERY_DIR = os.path.join(BASE_DIR, "data", "generated", "abbadingo", "queries")
RESULT_DIR = os.path.join(BASE_DIR, "data", "results", "abbadingo")

#FILENAMES = ["trainA", "trainB", "trainC", "trainD", "train.1", "train.2", "train.3", "trainR", "train.4", "train.5", "train.6", "trainS", "train.7", "train.8", "train.9", "trainT"]
#FILENAMES = ["small_test"]
FILENAMES = ["trainA"]

#FILENAMES = ["anti_naive1", "anti_naive2", "anti_naive3", "anti_naive4"]

#FILENAMES = ["trainA", "trainB", "train.1", "train.2",  "train.4", "train.5", "train.6", "train.7", "train.8", "train.9", "trainT"]

FILE_PATHS_TRACES = [os.path.join(TRACE_DIR, fname + ".txt") for fname in FILENAMES]
FILE_PATHS_QUERIES = [os.path.join(QUERY_DIR, fname + ".txt") for fname in FILENAMES]
FILE_PATH_CSV = os.path.join(RESULT_DIR, "abbadingo_results_all.csv")
EXE = os.path.join(BASE_DIR, "build", "Release", "main_app.exe")

#IMPLS = ["naive", "precomp", "sqrt"]
IMPLS = ["db"]
REPEAT = 4

def run_cpp(impl, inp):
    try:
        result = subprocess.run(
            [EXE, impl],
            input=inp,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=100
        )

        return result.stdout.strip().split(",")

    except subprocess.TimeoutExpired:
        return None

#time is measured in microseconds (us)
#memory is measured in bytes

def run_tests(dataset_id, inp_traces, inp_queries, outp):
    traces = ""
    queries = ""
    inp = ""

    with open(inp_traces, "r") as f:
        traces = f.read().strip()

    with open(inp_queries, "r") as f:
        queries = f.read().strip()
    
    inp = traces + "\n" + queries + "\n"


    with open(outp, "a", newline="") as f:
        writer = csv.writer(f)

        for _ in range(REPEAT):
            for impl in IMPLS:
                row = run_cpp(impl, inp)
                if row is None:
                    writer.writerow([dataset_id, "TIMEOUT"] + [""] * 8)
                else:
                    writer.writerow([dataset_id, "OK"] + row)


with open(FILE_PATH_CSV, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow([
        "dataset",
        "status",
        "impl",
        "positive_set_size",
        "negative_set_size",
        "number_of_queries",
        "build_time_us",
        "query_time_us",
        "memory_bytes",
        "total_output_len"
    ])

for i in range(len(FILENAMES)):
    dataset = FILENAMES[i]
    ftraces = FILE_PATHS_TRACES[i]
    fqueries = FILE_PATHS_QUERIES[i]
    foutp = FILE_PATH_CSV
    run_tests(dataset, ftraces, fqueries, foutp)