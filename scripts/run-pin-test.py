import os
import subprocess

PIN = "/qsym/third_party/pin-2.14-71313-gcc.4.4.7-linux/pin.sh"
PINTOOL_DIR = "/qsym/qsym/pintool"
INPUT = "/qsym/output/not_kitty.jpg"

SO = {
    "intel64": os.path.join(PINTOOL_DIR, "obj-intel64/libqsym.so"),
    "ia32": os.path.join(PINTOOL_DIR, "obj-ia32/libqsym.so")
}

def gen_cmd(timeout):
    cmd = []
    if timeout:
        cmd += ["timeout", "-k", str(5), str(timeout)]
    cmd += [PIN]

    # Hack for 16.04
    cmd += ["-ifeellucky"]

    # Check if target is 32bit ELF
    cmd += ["-t", SO["intel64"]]

    # Add log file
    cmd += ["-logfile", "/tmp/pin.log"]
    cmd += ["-i", INPUT] + ["-f", "1"]
    cmd += ["-o", "/tmp"]
    cmd += ["-l", "1"]

    cmd += ["-b", "/qsym/output/bitmap"]
    cmd += ["-z", "/qsym/output/jpeg-9d/new_cfg.dot"]
    cmd += ["-d", "1"]
    return cmd + ["--"] + ["/qsym/output/jpeg-9d/build/djpeg-normal", INPUT]

def main():
    cmd = gen_cmd(90)
    print(cmd)
    proc = subprocess.call(cmd)
    

if __name__ == '__main__':
    main()