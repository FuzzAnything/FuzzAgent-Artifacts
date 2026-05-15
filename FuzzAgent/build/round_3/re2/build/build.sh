#!/bin/bash -eu
# Copyright 2016 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################

CXXFLAGS="$CXXFLAGS -O2"

# First, build and install Abseil.
# https://github.com/abseil/abseil-cpp/issues/1524#issuecomment-1739364093
# explains why Abseil must be built for fuzzing, not depended on normally.
# N.B., this is pasted almost verbatim from what libphonenumber does here.
ABSEIL_DIR="$SRC/../abseil-cpp"
if [ ! -d "$ABSEIL_DIR" ]; then
    git clone --depth=1 https://github.com/abseil/abseil-cpp.git "$ABSEIL_DIR"
fi
cd "$ABSEIL_DIR"
mkdir -p build && cd build
cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DCMAKE_C_COMPILER="$CC" \
      -DCMAKE_CXX_COMPILER="$CXX" \
      -DCMAKE_C_FLAGS="$CFLAGS" \
      -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
      -DBUILD_SHARED_LIBS=OFF \
      ..
make -j$(nproc)
make install
ldconfig

# Second, build and install RE2.
# N.B., we don't follow the standard incantation for building RE2
# (i.e., `make && make test && make install && make testinstall`),
# because some of the targets doesn't use $CXXFLAGS properly, which
# causes compilation to fail. The obj/libre2.a target is all we
# really need for our fuzzer, so that's all we build. Hopefully
# this won't cause the fuzzer to fail erroneously due to not running
# upstream's tests first to be sure things compiled correctly.
# However, we do want the "common" files installed so that we can
# interrogate pkg-config about the Abseil dependencies instead of
# maintaining yet another enumeration of them here.
cd "$SRC"

# Add additional warning suppression for sanitizer builds
EXTRA_CXXFLAGS=""
if [[ $CXXFLAGS = *sanitize* ]]; then
    EXTRA_CXXFLAGS="-Wno-error=conversion -Wno-error=sign-conversion -Wno-error=shorten-64-to-32 -Wno-error=unused-parameter -Wno-error=missing-field-initializers"
fi

make -j$(nproc) CXX="$CXX" CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS" obj/libre2.a
make common-install prefix="$WORK"

# Create lib directory if it doesn't exist
mkdir -p "$WORK/lib"

# Copy the static library to $WORK/lib
cp obj/libre2.a "$WORK/lib/"

# Also copy Abseil libraries and headers to $WORK/3rdparty for completeness
mkdir -p "$WORK/3rdparty/lib" "$WORK/3rdparty/include"
cp -r /usr/local/lib/libabsl*.a "$WORK/3rdparty/lib/" 2>/dev/null || true
cp -r /usr/local/include/absl "$WORK/3rdparty/include/" 2>/dev/null || true
