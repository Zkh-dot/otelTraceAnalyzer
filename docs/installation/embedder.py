import base64
import sys

def main():
    if len(sys.argv) != 3:
        print("Usage: python embedder.py path_to_lib.so desired_path_to_analyze.py")
        return

    input_filename = sys.argv[1]
    output_script = sys.argv[2]

    with open(input_filename, 'rb') as f:
        data = f.read()
    encoded = base64.b64encode(data).decode('ascii')

    script_content = f'''\
import ctypes
import os
import base64

SO_NAME = "otelanalyzer.so"

SO_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)),  SO_NAME)

ENCODED_LIB = "{encoded}"

def create_and_launch():
    data = base64.b64decode(ENCODED_LIB.encode("ascii"))
    
    with open(SO_PATH, 'wb') as f:
        f.write(data)

create_and_launch()

lib = ctypes.CDLL(SO_PATH)

from .otelanalyzer import *
'''

    with open(output_script, 'w') as f:
        f.write(script_content)
    print(f"Script created: {output_script}")

if __name__ == "__main__":
    main()