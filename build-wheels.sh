#!/bin/bash
set -e -x

# Install system dependencies if any
# yum install -y <your-dependencies>

# Compile wheels for each Python version
i=0
for PYBIN in /opt/python/*/bin; do
    echo "${PYBIN}/pip"
    "${PYBIN}/pip" install --upgrade pip setuptools wheel
    "${PYBIN}/pip" wheel /io/ -w /io/dist/
    i=$((i+1))
    if [ $i -eq 3 ]; then
        break
    fi
done

# Bundle external shared libraries into the wheels
for whl in /io/dist/*.whl; do
    auditwheel repair "$whl" -w /io/dist/
done

# Remove the original wheels
rm /io/dist/*-linux_*.whl


