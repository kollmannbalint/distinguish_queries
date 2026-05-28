import subprocess
import os

FILE_PATHS_ABBADINGO = ['abbadingo']
FILE_PATHS_QUERIES = ['query_file']
EXE = 'filename' #give a file name for this

def conv_abbadingo(path):
    P = []
    N = []

    with open(path, 'r') as f:
        
        first_line = f.readline().strip()
        N, alphabetSize = map(int, first_line.split())

        for _ in range(N):
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

#feed it into gen_query_double_trie
#run gen_query_double_trie as a subprocess to generate queries
#read the query test instances
#generate files


#tests here: https://abbadingo.cs.nuim.ie/data-sets.html