#!/usr/bin/python

import sys
import re
import os
import fileinput

from pathlib import Path
from functools import reduce

definition_is_in = [
    (f"{t}.h", f"{t}.c") for t in [
        "hash",
        "memgr",
        "closure",
        "array",
        "list",
        "assoc_table",
        "hash_table",
        "atom",
        "deque",
        "str",
    ]
]

light_home = Path(__file__).parent

pattern_include = re.compile(r'#[ \t]*include[ \t]*([<"])(?P<header>\w+\.h)[">]')

def definition_files_of(topic):
    # rs: relations, deffs: definition files
    rs = filter(lambda t: t[0] == topic, definition_is_in)
    deffs = map(lambda t: [t[1]], rs)
    return reduce(lambda t1, t2: t1 + t2, deffs, [])

def extract_includes(*files):
    # hs: headers
    hs = []
    with fileinput.input(files=files, encoding="utf-8") as f:
        hs = list(map(lambda m: m.group("header"),
                      filter(lambda r: r is not None, map(pattern_include.match, f))))
    return hs

def is_of_light(header_file):
    return header_file.relative_to(light_home)


include_dirs = [light_home / "include"]

def find_header(header):
    for d in include_dirs:
        # hp: path of header
        hp = d / header
        if hp.exists():
            return hp.absolute()
    return None

def definition_files_related_to(file):
    # q: queue, hds: headers
    q = {file}
    hds = set()

    while q != set():
        f = q.pop()
        for hd in extract_includes(f):
            p = find_header(hd)
            if p is None:
                continue
            if hd not in hds:
                q.add(p)
            if is_of_light(p):
                hds.add(hd)
    return reduce(lambda l1, l2: l1+l2,
                  map(lambda hd: definition_files_of(hd), hds),
                  [])

if __name__ == "__main__":
    include_dirs += list(filter(lambda p: p.is_dir(), map(Path, sys.argv[2:])))
    file_to_analyze = sys.argv[1]
    # incs: includes, deffs: definition files
    deffs = definition_files_related_to(file_to_analyze)

    print(" ".join(
        map(
            lambda f: os.path.normpath(os.path.join(os.path.dirname(__file__), "src", f)),
            deffs
        )
    ))
