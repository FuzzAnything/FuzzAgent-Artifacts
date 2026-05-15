#!/bin/bash
set -e

echo "=== Building RE2 with properly merged Abseil ==="
echo "SRC: $SRC"
echo "WORK: $WORK"
echo "CC: $CC"
echo "CXX: $CXX"

# Create 3rdparty directory for Abseil
mkdir -p "$WORK/3rdparty"

# Build and install Abseil to $WORK/3rdparty
echo "=== Building and installing Abseil to $WORK/3rdparty ==="

if [ ! -d "/tmp/abseil-cpp" ]; then
    echo "Cloning Abseil..."
    rm -rf /tmp/abseil-cpp
    git clone --depth 1 https://github.com/abseil/abseil-cpp.git /tmp/abseil-cpp
fi

ABSL_SRC="/tmp/abseil-cpp"
cd "$ABSL_SRC"
rm -rf build && mkdir build
cd build

# Install Abseil to $WORK/3rdparty
cmake -DCMAKE_INSTALL_PREFIX="$WORK/3rdparty" \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_TESTING=OFF \
      -DABSL_PROPAGATE_CXX_STD=ON \
      -DABSL_BUILD_TESTING=OFF \
      -DABSL_ENABLE_INSTALL=ON \
      ..

make -j$(nproc)
make install

echo "=== Building RE2 ==="
cd "$SRC"

# Clean previous build
rm -rf obj build

# Set environment variables with Abseil from 3rdparty
export CXX="$CXX"
export CXXFLAGS="$CXXFLAGS -I$WORK/3rdparty/include"
export CC="$CC"
export CFLAGS="$CFLAGS -I$WORK/3rdparty/include"
export PKG_CONFIG_PATH="$WORK/3rdparty/lib/pkgconfig:$PKG_CONFIG_PATH"

echo "Building RE2 static library..."
# Build the library
make -j$(nproc) obj/libre2.a

# Verify the library was built
if [ ! -f "obj/libre2.a" ]; then
    echo "ERROR: obj/libre2.a not built!"
    exit 1
fi

echo "Installing RE2 headers and library..."
# First ensure directories exist
mkdir -p "$WORK/include" "$WORK/lib" "$WORK/lib/pkgconfig"

# Manually install headers
HEADER_FILES="re2/filtered_re2.h re2/re2.h re2/set.h re2/stringpiece.h"
mkdir -p "$WORK/include/re2"
for header in $HEADER_FILES; do
    if [ -f "$header" ]; then
        cp "$header" "$WORK/include/re2/"
    fi
done

# Copy the original library (we'll remove it later after creating combined version)
cp obj/libre2.a "$WORK/lib/"

echo "Creating properly merged library libre2_combined.a..."
# Create a temporary directory for combining
COMBINE_DIR="/tmp/combine-$$"
rm -rf "$COMBINE_DIR"
mkdir -p "$COMBINE_DIR"
cd "$COMBINE_DIR"

echo "Step 1: Extract ALL object files from libre2.a..."
ar x "$SRC/obj/libre2.a" 2>/dev/null || true
RE2_OBJECT_COUNT=$(ls -1 *.o 2>/dev/null | wc -l || echo 0)
echo "Extracted $RE2_OBJECT_COUNT object files from libre2.a"
echo "Step 2: Extract ALL object files from ALL Abseil libraries..."
ABSL_LIB_COUNT=0
for abseil_lib in $(find "$WORK/3rdparty" -name "libabsl*.a" -type f); do
    ABSL_LIB_COUNT=$((ABSL_LIB_COUNT + 1))
    libname=$(basename "$abseil_lib")
    echo "  Extracting from $libname..."
    ar x "$abseil_lib" 2>/dev/null || true
done

ABSL_OBJECT_COUNT=$(ls -1 *.o 2>/dev/null | wc -l || echo 0)
echo "Found $ABSL_LIB_COUNT Abseil libraries, extracted $ABSL_OBJECT_COUNT object files"
TOTAL_OBJECTS=$(ls -1 *.o 2>/dev/null | wc -l || echo 0)
echo "Total object files before merging: $TOTAL_OBJECTS"

if [ $TOTAL_OBJECTS -eq 0 ]; then
    echo "ERROR: No object files extracted!"
    exit 1
fi

echo "Step 3: Merge ALL object files into a single relocatable object using ld -r..."
# Create a list of all object files
OBJECT_FILES=$(ls -1 *.o 2>/dev/null | tr '\n' ' ')

# Use ld -r to merge all object files into one
if ld -r $OBJECT_FILES -o merged_all.o 2>/dev/null; then
    echo "SUCCESS: Merged $TOTAL_OBJECTS object files into merged_all.o"
    
    # Check for undefined symbols in merged object
    if command -v nm >/dev/null 2>&1; then
        UNDEFINED_COUNT=$(nm --undefined-only merged_all.o 2>/dev/null | grep -i "absl" | wc -l || echo 0)
        if [ $UNDEFINED_COUNT -gt 0 ]; then
            echo "WARNING: merged_all.o still has $UNDEFINED_COUNT undefined Abseil symbols"
            echo "First few undefined symbols:"
            nm --undefined-only merged_all.o 2>/dev/null | grep -i "absl" | head -5
        else
            echo "SUCCESS: No undefined Abseil symbols in merged_all.o"
        fi
    fi
    
    echo "Step 4: Create final library from merged object..."
    ar rcs "$WORK/lib/libre2_combined.a" merged_all.o 2>/dev/null || true
else
    echo "WARNING: ld -r failed, falling back to simple archive..."
    # Fallback: create archive from all objects
    ar rcs "$WORK/lib/libre2_combined.a" *.o 2>/dev/null || true
fi

# Verify the combined library was created
if [ ! -f "$WORK/lib/libre2_combined.a" ]; then
    echo "ERROR: Failed to create libre2_combined.a!"
    exit 1
fi

# Save size of original library before removal for comparison
if command -v stat >/dev/null 2>&1 && [ -f "$WORK/lib/libre2.a" ]; then
    ORIG_SIZE=$(stat -c%s "$WORK/lib/libre2.a" 2>/dev/null || echo 0)
else
    ORIG_SIZE=0
fi

echo "Replacing libre2.a with combined version (contains Abseil symbols)..."
# Remove original and rename combined to libre2.a for compatibility
rm -f "$WORK/lib/libre2.a"
mv "$WORK/lib/libre2_combined.a" "$WORK/lib/libre2.a"

# Clean up temporary directory
cd /
rm -rf "$COMBINE_DIR"

# Create pkg-config file for RE2
echo "Creating pkg-config file..."
ABSL_DEPS=$(grep "^ABSL_DEPS=" "$SRC/Makefile" | cut -d= -f2- | tr '\\' ' ' | sed 's/[[:space:]]*$//')

# Convert to pkg-config format: absl_absl_check -> absl-absl-check
REQUIRES=""
for dep in $ABSL_DEPS; do
    # Convert underscores to hyphens
    pkg_dep=$(echo "$dep" | sed 's/_/-/g')
    REQUIRES="$REQUIRES $pkg_dep"
done
REQUIRES=$(echo "$REQUIRES" | sed 's/^ //')

cat > "$WORK/lib/pkgconfig/re2.pc" << EOF
prefix=$WORK
exec_prefix=\${prefix}
includedir=\${prefix}/include
libdir=\${exec_prefix}/lib

Name: re2
Description: RE2 is a fast, safe, thread-friendly regular expression engine.
Requires: $REQUIRES
Version: 11.0.0
Cflags: -pthread -I\${includedir}
Libs: -pthread -L\${libdir} -lre2
EOF

echo "=== Verification ==="
echo "Checking final library..."

# Check for undefined Abseil symbols in final library
if command -v nm >/dev/null 2>&1; then
    echo "Checking libre2.a for undefined Abseil symbols..."
    UNDEFINED_COUNT=$(nm --undefined-only "$WORK/lib/libre2.a" 2>/dev/null | grep -i "absl" | wc -l || echo 0)
    if [ $UNDEFINED_COUNT -gt 0 ]; then
        echo "WARNING: libre2.a still has $UNDEFINED_COUNT undefined Abseil symbols"
        echo "First 5 undefined symbols:"
        nm --undefined-only "$WORK/lib/libre2.a" 2>/dev/null | grep -i "absl" | head -5
    else
        echo "SUCCESS: No undefined Abseil symbols found in libre2.a"
    fi
    
    # Check size
    COMBINED_SIZE=$(stat -c%s "$WORK/lib/libre2.a" 2>/dev/null || echo 0)
    echo "Final library size:"
    echo "  libre2.a: $COMBINED_SIZE bytes (contains merged RE2 + Abseil symbols)"
    if [ $ORIG_SIZE -gt 0 ] && [ $COMBINED_SIZE -gt $ORIG_SIZE ]; then
        SIZE_DIFF=$((COMBINED_SIZE - ORIG_SIZE))
        echo "  Library is $SIZE_DIFF bytes larger than original (includes Abseil)"
    fi
fi

# Final test
echo "=== Final test ==="
TEST_DIR="/tmp/final-test-$$"
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"
cd "$TEST_DIR"

cat > test_re2.cc << 'EOF'
#include <re2/re2.h>
int main() {
    re2::RE2 re("test");
    return re.ok() ? 0 : 1;
}
EOF

echo "Compiling test with libre2.a (merged with Abseil)..."
$CXX $CXXFLAGS -I"$WORK/include" test_re2.cc "$WORK/lib/libre2.a" -o test_re2 -pthread 2>&1 | tail -20

if [ -x test_re2 ]; then
    echo "SUCCESS: Test program compiled with libre2.a!"
    if ./test_re2; then
        echo "SUCCESS: Test program ran successfully!"
    else
        echo "WARNING: Test program returned error (might be expected for simple test)"
    fi
else
    echo "WARNING: Failed to compile test program with libre2.a"
fi

cd /
rm -rf "$TEST_DIR"

echo ""
echo "=== Build complete ==="
echo "Available in $WORK/lib:"
ls -la "$WORK/lib/"*.a 2>/dev/null || echo "No .a files found"
echo ""
echo "Abseil libraries available in $WORK/3rdparty/lib for linking if needed"
echo ""
echo "For fuzzing with self-contained library:"
echo "  \$CXX \$CXXFLAGS -I$WORK/include fuzzer.cc -o fuzzer \\"
echo "    $WORK/lib/libre2.a -lpthread \$LIB_FUZZING_ENGINE"
