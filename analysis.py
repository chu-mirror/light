import sys
import re
import fileinput
from functools import reduce
from pathlib import Path

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
    ]
]

light_home = Path(__file__).parent

def definition_files_of(topic):
    # rs: relations, deffs: definition files
    rs = filter(lambda t: t[0] == topic, definition_is_in)
    deffs = map(lambda t: [t[1]], rs)
    return reduce(lambda t1, t2: t1 + t2, deffs, [])

def extract_includes(*files):
    #p: pattern, hs: headers 
    p = re.compile(r'#[ \t]*include[ \t]*([<"])(?P<header>\w+\.h)[">]')
    hs = []
    with fileinput.input(files=files, encoding="utf-8") as f:
        hs = list(map(lambda m: m.group("header"),
                      filter(lambda r: r is not None, map(p.match, f))))
    return hs

def is_of_light(header_file):
    return header_file.relative_to(light_home)

