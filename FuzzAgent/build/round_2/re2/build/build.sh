#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Set up compiler flags - append to existing flags
export CXXFLAGS="$CXXFLAGS -O2"
export CFLAGS="$CFLAGS -O2"

# Install system Abseil development packages if needed
echo "Checking for Abseil development packages..."
if ! dpkg -l | grep -q libabsl-dev; then
    echo "Installing libabsl-dev..."
    apt-get update && apt-get install -y libabsl-dev
fi

# Update PKG_CONFIG_PATH to find Abseil
export PKG_CONFIG_PATH="/usr/lib/x86_64-linux-gnu/pkgconfig:/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH"

# Build RE2 - following oss-fuzz approach
echo "Building RE2..."
cd "$SRC"

# Add additional warning suppression for sanitizer builds
EXTRA_CXXFLAGS=""
if [[ $CXXFLAGS = *sanitize* ]]; then
    EXTRA_CXXFLAGS="-Wno-error=conversion -Wno-error=sign-conversion -Wno-error=shorten-64-to-32 -Wno-error=unused-parameter -Wno-error=missing-field-initializers"
fi

# Build the static library with Abseil support
echo "Building RE2 static library with Abseil support..."

# Get Abseil flags from pkg-config
ABSL_DEPS="absl_absl_check absl_absl_log absl_base absl_core_headers absl_fixed_array absl_flags absl_flat_hash_map absl_flat_hash_set absl_hash absl_inlined_vector absl_optional absl_span absl_str_format absl_strings absl_synchronization"

# Try to get Abseil flags, but don't fail if pkg-config doesn't work
ABSL_CXXFLAGS=$(pkg-config $ABSL_DEPS --cflags 2>/dev/null || echo "")
ABSL_LDFLAGS=$(pkg-config $ABSL_DEPS --libs 2>/dev/null || echo "")

echo "Building RE2 with compiler: $CXX"
echo "Using CXXFLAGS: $CXXFLAGS $EXTRA_CXXFLAGS $ABSL_CXXFLAGS"

# Build RE2 with Abseil support
make -j$(nproc) CXX="$CXX" CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS $ABSL_CXXFLAGS" obj/libre2.a

# Install common files (headers, pkg-config) to $WORK
echo "Installing RE2 headers and pkg-config to $WORK..."
make common-install prefix="$WORK"

# Create lib directory
mkdir -p "$WORK/lib"

# Copy static library
cp obj/libre2.a "$WORK/lib/"

# Copy Abseil static libraries to 3rdparty directory
echo "Copying Abseil libraries to 3rdparty directory..."
mkdir -p "$WORK/3rdparty/lib"
find /usr/lib/x86_64-linux-gnu -name "libabsl*.a" -exec cp {} "$WORK/3rdparty/lib/" \; 2>/dev/null || true
find /usr/local/lib -name "libabsl*.a" -exec cp {} "$WORK/3rdparty/lib/" \; 2>/dev/null || true

# Create a simple README explaining how to link RE2 with Abseil
mkdir -p "$WORK"
cat > "$WORK/README.md" << 'EOF'
# RE2 Library with Abseil Dependencies

This build of RE2 requires Abseil libraries for linking. To use RE2 in your project:

## For static linking:
```
clang++ -I$WORK/include your_program.cpp \\
  $WORK/lib/libre2.a \\
  $(pkg-config --libs absl_base absl_strings absl_synchronization absl_str_format absl_optional absl_span absl_hash absl_flat_hash_map absl_flat_hash_set absl_inlined_vector absl_fixed_array absl_core_headers absl_absl_check absl_absl_log absl_flags 2>/dev/null || echo "-labsl_base -labsl_strings -labsl_synchronization") \\
  -pthread -o your_program
```

## Abseil libraries available:
All required Abseil static libraries have been copied to $WORK/3rdparty/lib/
EOF

# Verify installation
if [ -f "$WORK/lib/libre2.a" ]; then
    echo "✓ Build completed successfully"
    echo "  Library installed to: $WORK/lib/libre2.a"
    echo "  Size: $(stat -c%s "$WORK/lib/libre2.a") bytes"
    
    if [ -f "$WORK/include/re2/re2.h" ]; then
        echo "  Headers installed to: $WORK/include/re2/"
    else
        echo "⚠ Warning: Headers may not be installed correctly"
    fi
    
    # Count Abseil libraries copied
    ABSEIL_COUNT=$(find "$WORK/3rdparty/lib" -name "libabsl*.a" 2>/dev/null | wc -l)
    echo "  Abseil libraries copied: $ABSEIL_COUNT"
    
else
    echo "❌ Error: Library not found at $WORK/lib/libre2.a"
    exit 1
fi

echo "RE2 build complete with Abseil support"
