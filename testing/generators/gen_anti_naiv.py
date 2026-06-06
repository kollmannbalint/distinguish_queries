
import random
import os

random.seed(342343)

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
TRACE_DIR = os.path.join(BASE_DIR, "data", "generated", "anti", "traces")
QUERY_DIR = os.path.join(BASE_DIR, "data", "generated", "anti", "queries")

FILENAMES = ["anti_naive1", "anti_naive2", "anti_naive3", "anti_naive4"]
FILE_PATHS_TRACES = [os.path.join(TRACE_DIR, fname + ".txt") for fname in FILENAMES]
FILE_PATHS_QUERIES = [os.path.join(QUERY_DIR, fname + ".txt") for fname in FILENAMES]

aSufLens = [1000, 1000000, 50000, 100000]
randWordCnts = [15,0, 0, 100]
randWordLens = [30, 30, 30, 200]
maxShortPrefLens = [100, 20, 20, 12]
queryCnts = [100000, 1000, 50000, 100000]

def anti_naiv_traces(aSufLen, randWordCnt, randWordLen, maxShortPrefLen, queryCnt):
    used = set()

    def unique_word():
        while True:
            w = ''.join(random.choice("ab") for _ in range(randWordLen))
            w = w[0] + "b" + w[2:]
            if w not in used:
                used.add(w)
                return w

    P = [unique_word() for _ in range(randWordCnt)]
    N = [unique_word() for _ in range(randWordCnt)]
    

    wordA = "a" + "a" * aSufLen
    wordB = "b" + "a" * aSufLen

    P += [wordA, wordB]

    return P,N


def anti_naiv_queries(aSufLen, randWordCnt, randWordLen, maxShortPrefLen, queryCnt):
    queries = []
    
    for _ in range(queryCnt):
        lenA = random.choice(range(maxShortPrefLen))
        lenB = random.choice(range(maxShortPrefLen))

        wordA = "a" + "a" * lenA
        wordB = "b" + "a" * lenB

        if(random.choices([0,1])) == 1:
            queries.append((wordA, wordB))
        else:
            queries.append((wordB, wordA))

    return queries
    

    #return "\n".join(parts)

for i in range (len(FILENAMES)):
    aSufLen = aSufLens[i]
    randWordCnt = randWordCnts[i]
    randWordLen = randWordLens[i]
    maxShortPrefLen = maxShortPrefLens[i]
    queryCnt = queryCnts[i]

    P,N = anti_naiv_traces(aSufLen, randWordCnt, randWordLen, maxShortPrefLen, queryCnt)
    queries = anti_naiv_queries(aSufLen, randWordCnt, randWordLen, maxShortPrefLen, queryCnt)

    parts_traces = [str(len(P))]
    parts_traces += P
    parts_traces += [str(len(N))]
    parts_traces += N

    ftraces = FILE_PATHS_TRACES[i]
    with open(ftraces, 'w') as f:
        f.write("\n".join(parts_traces) + "\n")

    fqueries = FILE_PATHS_QUERIES[i]
    parts_queries = [str(len(queries))]
    parts_queries += [f"{a} {b}" for a, b in queries]

    with open(fqueries, 'w') as f:
        f.write("\n".join(parts_queries) + "\n")