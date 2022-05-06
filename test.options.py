#!./envi -a 1 -b 2
import sys
print("test.py: sys.argv =", sys.argv)
import os
print("test.py: A =", os.environ.get("A"))
print("test.py: B =", os.environ.get("B"))

