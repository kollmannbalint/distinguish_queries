import random

prefLen = 10
sufLen = 3
prefCnt = 500
sufCnt = 8
queryCnt = 100

def get_random_word(length):
    w = ''.join(random.choice("abcdefghi") for _ in range(length))
    return w

def anti_precomp_traces():
    used = set()

    def unique_word():
        while True:
            w = ''.join(random.choice("ab") for _ in range(random.randint(3, 6)))
            if w not in used:
                used.add(w)
                return w

    Pprefs = [unique_word() for _ in range(prefCnt)]
    Nprefs = [unique_word() for _ in range(prefCnt)]
    sufs = [unique_word() for _ in range(sufCnt)]

    P = []
    N = []
    
    for pref in Pprefs:
        for suf in sufs:
            P.append(pref + suf)
    
    for pref in Nprefs:
        for suf in sufs:
            N.append(pref + suf)

    return P,N,Pprefs,Nprefs,sufs



def anti_precomp_queries(Pprefs, Nprefs, sufs):
    queries = []

    for _ in range(queries):
        Ppref = random.choice(Pprefs)
        Npref = random.choice(Nprefs)
        suf = random.choice(sufs)

        queries.append((Ppref + suf, Npref + suf))

    return queries


def anti_precomp_tests():
    P,N,Pprefs,Nprefs,sufs = anti_precomp_traces()
    queries = anti_precomp_queries(Pprefs, Nprefs, sufs)

    parts = []
    parts.append(str(len(P)))
    parts.extend(P)
    parts.append(str(len(N)))
    parts.extend(N)
    parts.append(str(len(queries)))
    parts.extend(parts.extend(f"{a} {b}" for a, b in queries))

    return "\n".join(parts)