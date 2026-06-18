import subprocess
import os
import csv
import pandas as pd

#DATASET = "abbadingo"
DATASET = "stamina"
#DATASET = "anti"

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
TRACE_DIR = os.path.join(BASE_DIR, "data", "generated", DATASET, "traces")
QUERY_DIR = os.path.join(BASE_DIR, "data", "generated", DATASET, "queries")
RESULT_DIR = os.path.join(BASE_DIR, "data", "results", DATASET)

#FILENAMES = ["trainA", "trainB", "trainC", "trainD", "train.1", "train.2", "train.3", "trainR", "train.4", "train.5", "train.6", "trainS", "train.7", "train.8", "train.9", "trainT"]
FILENAMES = [ (str(i) + "_training")  for i in range(1, 100)]
#FILENAMES = ["anti_naive1", "anti_naive2", "anti_naive3", "anti_naive4"]


FILE_PATHS_TRACES = [os.path.join(TRACE_DIR, fname + ".txt") for fname in FILENAMES]
FILE_PATHS_QUERIES = [os.path.join(QUERY_DIR, fname + ".txt") for fname in FILENAMES]
FILE_PATH_TEST_CSV = os.path.join(RESULT_DIR, "anti_naive_results.csv")
FILE_PATH_STATS_CSV = os.path.join(RESULT_DIR, "stamina_stats.csv")
EXE_TEST = os.path.join(BASE_DIR, "build", "Release", "main_app.exe")
EXE_STATS = os.path.join(BASE_DIR, "build", "Release", "ds_stats.exe")

IMPLS = ["naive", "precomp", "sqrt"]
REPEAT = 5

def run_cpp_test(impl, inp):
    try:
        result = subprocess.run(
            [EXE_TEST, impl],
            input=inp,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=100
        )

        return result.stdout.strip().split(",")

    except subprocess.TimeoutExpired:
        return None
    
def run_cpp_stats(inp):
    result = subprocess.run(
        [EXE_STATS],
            input=inp,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=1000
    )

    return result.stdout.strip().split(",")

#time is measured in microseconds (us)
#memory is measured in bytes

def run_test(dataset_id, inp_traces, inp_queries, outp):
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
                row = run_cpp_test(impl, inp)
                if row is None:
                    writer.writerow([dataset_id, "TIMEOUT"] + [""] * 8)
                else:
                    writer.writerow([dataset_id, "OK"] + row)

def run_stats(dataset_id, inp_traces, outp):
    traces = ""
    inp = ""

    with open(inp_traces, "r") as f:
        traces = f.read().strip()
    
    inp = traces


    with open(outp, "a", newline="") as f:
        writer = csv.writer(f)

        row = run_cpp_stats(inp)
        writer.writerow([dataset_id] + row)


def run_all_tests():
    with open(FILE_PATH_TEST_CSV, "w", newline="") as f:
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
        foutp = FILE_PATH_TEST_CSV
        run_test(dataset, ftraces, fqueries, foutp)

def run_all_statistics():
    with open(FILE_PATH_STATS_CSV, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([
            "dataset",
            "alphabet_size",
            "positive_set_size",
            "negative_set_size",
            "avg_word_len",
            "cnt_symbols",
            "cnt_nodes_trie",
            "cnt_nodes_double_trie",
            "cnt_links",
            "cnt_distinguishable_prefix_pairs",
            "avg_overlap"
        ])

    for i in range(len(FILENAMES)):
        dataset = FILENAMES[i]
        ftraces = FILE_PATHS_TRACES[i]
        foutp = FILE_PATH_STATS_CSV
        run_stats(dataset, ftraces, foutp)

def conv_to_latex(path):
    df = pd.read_csv(path)

    df_words = df[["dataset", "alphabet_size", "positive_set_size", "negative_set_size", "avg_word_len", "cnt_symbols"]]

    df_words.columns = [col.replace("_", r"\_") for col in df_words.columns]
    df_words = df_words.map(lambda x: x.replace("_", r"\_") if isinstance(x, str) else x)
    print(df_words.to_latex(
        index=False,
        escape=False,
        float_format="%.2f"
    ))

    df_trie = df[["dataset", "cnt_nodes_trie", "cnt_nodes_double_trie", "cnt_links", "cnt_distinguishable_prefix_pairs", "avg_overlap"]]

    df_trie.columns = [col.replace("_", r"\_") for col in df_trie.columns]
    df_trie = df_trie.map(lambda x: x.replace("_", r"\_") if isinstance(x, str) else x)
    print(df_trie.to_latex(
        index=False,
        escape=False,
        float_format="%.2f"
    ))

#run_all_tests()
#run_all_statistics()

conv_to_latex(FILE_PATH_STATS_CSV)