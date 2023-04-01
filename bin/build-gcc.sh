#!/bin/sh

set -euxo pipefail

SRCDIR="$(pwd)/sparsematrix"
BUILDDIR="$(pwd)/build"
GCOV="${GCOV:=gcov}"
MIN_COVERAGE=90

echo "SRCDIR=$SRCDIR"
echo "BUILDDIR=$BUILDDIR"
echo "CC=$CC"
echo "CXX=$CXX"
echo "GCOV=$GCOV"

# Build
rm -rf $BUILDDIR
cmake --preset default-configure-coverage
cmake --build --preset default-build-coverage
echo "*\n!.gitignore\n" > $BUILDDIR/.gitignore

# Prepare for test
cd $BUILDDIR
lcov --zerocounters --directory .

# Run tests
ctest

# Process coverage data
lcov --capture --directory testbin/ --include "$SRCDIR/sparsematrix.h" --gcov-tool $GCOV --output-file coverage.info
lcov --list coverage.info
coverage=$(lcov --list coverage.info | grep -e '^sparsematrix.h' | awk -F'[[:space:]]+' '{gsub(/[^[:digit:]\.]/, "", $2); print $2}')
if (( $(echo "$coverage > $MIN_COVERAGE" | bc -l) ))
then
    echo "Coverage OK"
else
    echo "Coverage NOT OK ($coverage)"
fi

# Generate HTML report
genhtml --output-directory coverage_html coverage.info
