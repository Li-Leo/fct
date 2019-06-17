#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import subprocess
import re
import shutil
from os import environ, rename
from pathlib import Path
import sys

def get_name():
    name = ''
    with open(Path.cwd() / "Source/version.h", 'r') as f:
        name = (re.findall('\".+?\"', str(f.readlines())))[0]
    return name.strip('"')

def get_version():
    v = []
    with open(Path.cwd() / "Source/version.h", 'r') as f:
        for line in f:
            line = line.strip('\n')
            if 'MAJOR_VERSION' in line:
                v.append(re.findall("\d+", line))
            elif 'MINOR_VERSION' in line:
                v.append(re.findall("\d+", line))
            elif 'REVISION_VERSION' in line:
                v.append(re.findall("\d+", line))
    return [j[0] for j in v]

def make_release(version):
    file_name = "FCT1_8"
    my_env = environ.copy()
    my_env["PATH"] = "C:\Program Files\IAR Systems\Embedded Workbench 7.0\common\\bin;" \
                     + my_env["PATH"]
    cmd = "iarbuild" + " IAR_project\\" + "Spump18_master.ewp" + " -make Release"

    print(cmd)

    p = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, env = my_env, bufsize=1)
    ret = ''
    while True:
        out = p.stdout.read(1)
        if not out and p.poll() != None: #out == '' 
            break
        if out:
            ret += out.decode('utf-8')
            print(out.decode('utf-8'), end='')

    errors = re.findall("\d+", ret.split('\r\n')[-3])

    if errors[0] == '0':
        print('Build success!')
        s = get_name() + 'V' + version
        p = Path.cwd() / s
        if p.exists():
            for file in p.glob('*'):
                file.unlink()
        else:
            p.mkdir()
        in_file = Path.cwd() / ('IAR_project/Release/exe/' + 'Spump18_master.txt')
        shutil.copy(in_file, p)
        rename(p / 'Spump18_master.txt', p / (s+'.txt'))
    else:
        print('Build fail!')

if __name__ == "__main__":
    # print(get_name())
    v = '.'.join(c for c in get_version())
    print('building version %s...' % v)
    make_release(v)
