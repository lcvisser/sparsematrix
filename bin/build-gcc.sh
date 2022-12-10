#!/bin/sh

set -euxo pipefail

SRCDIR="$(pwd)/sparsematrix"
GCOV="${GCOV:=gcov}"

echo "SRCDIR=$SRCDIR"
echo "CC=$CC"
echo "CXX=$CXX"
echo "GCOV=$GCOV"

# Build
rm -rf build
cmake --preset default-configure-coverage
cmake --build --preset default-build-coverage

# Prepare for test
cd build
lcov --zerocounters --directory .

# Run tests
ctest

# Process coverage data
lcov --capture --directory testbin/ --include "$SRCDIR/sparsematrix.h" --gcov-tool $GCOV --output-file coverage.info
lcov --list coverage.info


# Generate HTML report
genhtml --output-directory coverage_html coverage.info
