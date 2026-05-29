
import random

aSufLen = 1000
randWordCnt = 15
randWordLen = 30
maxShortPrefLen = 100
queryCnt = 100

def anti_naiv_traces():
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

    P.extend([wordA, wordB])

    return P,N


def anti_naiv_queries():
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

def anti_naiv_tests():
    P,N = anti_naiv_traces()
    queries = anti_naiv_queries()

    parts = []
    parts.append(str(len(P)))
    parts.extend(P)
    parts.append(str(len(N)))
    parts.extend(N)
    parts.append(str(len(queries)))
    parts.extend(parts.extend(f"{a} {b}" for a, b in queries))

    return "\n".join(parts)