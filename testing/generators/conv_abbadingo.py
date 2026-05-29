import subprocess
import os

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
RAW_DIR = os.path.join(BASE_DIR, "data", "raw", "abbadingo")
TRACE_DIR = os.path.join(BASE_DIR, "data", "generated", "abbadingo", "traces")
QUERY_DIR = os.path.join(BASE_DIR, "data", "generated", "abbadingo", "queries")

FILENAMES = ["small_test"]#["trainA", "trainB", "trainC", "trainD", "train.1", "train.2", "train.3", "trainR", "train.4", "train.5", "train.6", "trainS", "train.7", "train.8", "train.9", "trainT"]

FILE_PATHS_RAW_ABBADINGO = [os.path.join(RAW_DIR, fname) for fname in FILENAMES]
FILE_PATHS_TRACES = [os.path.join(TRACE_DIR, fname + ".txt") for fname in FILENAMES]
FILE_PATHS_QUERIES = [os.path.join(QUERY_DIR, fname + ".txt") for fname in FILENAMES]
EXE = os.path.join(BASE_DIR, "cpp", "build", "Debug", "gen_query.exe")

RSEED = 8324932
MAX_TOT_WORD_LENGTH = 1000
MAX_QUERIES = 300

# print(FILE_PATHS_RAW_ABBADINGO)
# print(FILE_PATHS_TRACES)
# print(FILE_PATHS_QUERIES)

def conv_abbadingo(path):
    P = []
    N = []

    with open(path, 'r') as f:
        
        first_line = f.readline().strip()
        n, alphabetSize = map(int, first_line.split())

        for _ in range(n):
            line = f.readline().strip()
            parts = list(map(int, line.split()))

            label = parts[0]
            k = parts[1]
            trace = parts[2:]

            trace_str = ''.join(chr(ord('a') + x) for x in trace)

            if label == 1:
                P.append(trace_str)
            else:
                N.append(trace_str)
        
    return P, N

def trace_sets_to_string(P, N):
    parts = []
    parts.append(str(len(P)))
    parts.extend(P)
    parts.append(str(len(N)))
    parts.extend(N)

    return "\n".join(parts)

def run_cpp(inp):
    return subprocess.run(
        [EXE],
        input=inp,
        text=True,
        capture_output=True,
        timeout=2
    ).stdout.strip()


def conv_abbadingo_dataset():
    for i in range(len(FILENAMES)):
        finp = FILE_PATHS_RAW_ABBADINGO[i]
        foutp = FILE_PATHS_TRACES[i]

        P,N = conv_abbadingo(finp)
        outp = trace_sets_to_string(P,N)

        with open(foutp, 'w') as f:    
            f.write(outp)

#conv_abbadingo_dataset()

def gen_queries(rseed, max_queries, max_tot_word_length):
    for i in range(len(FILENAMES)):
        ftraces = FILE_PATHS_TRACES[i]
        fqueries = FILE_PATHS_QUERIES[i]
        P = []
        N = []

        with open(ftraces, 'r') as f:
            n = int(f.readline().strip())
            for _ in range(n):
                trace = f.readline().strip()
                P.append(trace)
            m = int(f.readline().strip())
            for _ in range(m):
                trace = f.readline().strip()
                N.append(trace)

        query_gen_inp_list = [str(rseed), str(max_queries), str(max_tot_word_length)]
        query_gen_inp_list.append(str(len(P)))
        query_gen_inp_list += P
        query_gen_inp_list.append(str(len(N)))
        query_gen_inp_list += N

        query_gen_inp = "\n".join(query_gen_inp_list) + "\n"

        queries = run_cpp(query_gen_inp)
        q = len(queries.strip().split('\n'))

        with open(fqueries, 'w') as f:
            f.write(str(q) + "\n")
            f.write(queries)


gen_queries(RSEED, MAX_QUERIES, MAX_TOT_WORD_LENGTH)

#tests here: https://abbadingo.cs.nuim.ie/data-sets.html