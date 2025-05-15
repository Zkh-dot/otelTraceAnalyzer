#!/bin/bash

# build so
python3 ./setup.py build_ext --inplace && mv ./otelanalyzer.so ./otelanalyzer

docker run --rm -v $(pwd):/io quay.io/pypa/manylinux2014_x86_64 /io/build-wheels.sh
twine upload dist/*

sudo rm dist/*