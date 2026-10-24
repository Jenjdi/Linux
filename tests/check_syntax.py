#!/usr/bin/env python3
"""编译所有源码并独立检查头文件；不把语法通过等同于算法正确。"""
from pathlib import Path
import concurrent.futures
import os
import shlex
import subprocess
import sys
root = Path(__file__).resolve().parents[1]
flags = shlex.split(os.environ.get('CPPFLAGS', ''))
files = [path for path in root.rglob('*') if '.git' not in path.parts and path.suffix in {'.c', '.cc', '.cpp', '.h', '.hpp'}]
def check(path):
    is_c = path.suffix == '.c'
    compiler = os.environ.get('CC' if is_c else 'CXX', 'gcc' if is_c else 'g++')
    command = [compiler, '-fsyntax-only', '-fmax-errors=3', '-std=c11' if is_c else '-std=c++17', '-pthread'] + flags
    if path.suffix in {'.h', '.hpp'}:
        command += ['-x', 'c++', '-']
        source = '#include "' + str(path) + '"\n'
    else:
        command.append(str(path)); source = None
    result = subprocess.run(command, input=source, text=True, errors='replace', capture_output=True)
    return path, result
with concurrent.futures.ThreadPoolExecutor(max_workers=4) as executor:
    results = list(executor.map(check, files))
for path, result in results:
    if result.returncode:
        print(str(path.relative_to(root)) + '\n' + result.stderr, file=sys.stderr)
failed = sum(result.returncode != 0 for _, result in results)
print(f'检查 {len(results)} 个源文件/头文件，失败 {failed}')
sys.exit(bool(failed))
