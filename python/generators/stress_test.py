import subprocess
import random
import os

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
EXE = os.path.join(BASE_DIR, "cpp", "build", "test_program.exe")


def generate():
    n = 5
    m = 5
    q = 50

    used = set()

    def unique_word():
        while True:
            w = ''.join(random.choice("ab") for _ in range(random.randint(3, 6)))
            if w not in used:
                used.add(w)
                return w

    P = [unique_word() for _ in range(n)]
    N = [unique_word() for _ in range(m)]

    all_words = P + N

    queries = []
    for _ in range(q):
        w1 = random.choice(all_words)
        w2 = random.choice(all_words)

        i = random.randint(1, len(w1))
        j = random.randint(1, len(w2))

        queries.append((w1[:i], w2[:j]))

    inp = []
    inp.append(str(n))
    inp += P
    inp.append(str(m))
    inp += N
    inp.append(str(q))
    inp += [f"{a} {b}" for a, b in queries]

    return "\n".join(inp) + "\n", P, N, queries


def run_cpp(inp):
    return subprocess.run(
        [EXE],
        input=inp,
        text=True,
        capture_output=True,
        timeout=2
    ).stdout.strip()


def valid_cross(pset, nset, a, b):
    return (a in pset and b in nset) or (a in nset and b in pset)


def exists_valid_suffix(pset, nset, a, b):
    full = pset | nset

    for w in full:
        if w.startswith(a):
            suf = w[len(a):]
            x = b + suf
            if valid_cross(pset, nset, w, x):
                return True

        if w.startswith(b):
            suf = w[len(b):]
            x = a + suf
            if valid_cross(pset, nset, x, w):
                return True

    return False


def check(P, N, queries, answers):
    pset = set(P)
    nset = set(N)

    for (a, b), suf in zip(queries, answers):

        if suf == "$":
            suf = ""

        if suf != "#":
            a2 = a + suf
            b2 = b + suf

            if not valid_cross(pset, nset, a2, b2):
                print("invalid suffix", a, b, suf)
                return False

        else:
            if exists_valid_suffix(pset, nset, a, b):
                print("missed existing suffix", a, b)
                return False

    return True


for _ in range(1000):
    inp, P, N, queries = generate()

    cpp_out = run_cpp(inp)
    answers = cpp_out.strip().split()

    if not check(P, N, queries, answers):
        print("MISMATCH FOUND")
        print(inp)
        print(cpp_out)
        break

print("OK")

"""
5                                                                 
abbbb
abbbab
aaabab
bbab
bbabb
5
abbbaa
aaaba
abbba
aaaa
bbbaa
1
bbbaa
bbabb
"""