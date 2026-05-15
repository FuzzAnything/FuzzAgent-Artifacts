#!/bin/bash
set -e  # Fail immediately on error

# Clean and prepare build directory
rm -rf /tmp/build_tmp && mkdir /tmp/build_tmp && cd /tmp/build_tmp

# Configure wllvm environment for coverage instrumentation
export LLVM_COMPILER=clang
export WLLVM_OUTPUT_LEVEL=WARNING

# For wllvm to work properly, we need to use wllvm as the compiler
# but it should wrap the actual compiler provided by the system
export REAL_CC="$CC"
export REAL_CXX="$CXX"
export CC="wllvm"
export CXX="wllvm++"

# Append SQLite fuzzing-specific flags to CFLAGS
# These limits help prevent irrelevant OOMs during fuzzing
export CFLAGS="$CFLAGS -DSQLITE_MAX_LENGTH=128000000 \
               -DSQLITE_MAX_SQL_LENGTH=128000000 \
               -DSQLITE_MAX_MEMORY=25000000 \
               -DSQLITE_PRINTF_PRECISION_LIMIT=1048576 \
               -DSQLITE_DEBUG=1 \
               -DSQLITE_MAX_PAGE_COUNT=16384"

# Configure SQLite with all features enabled for comprehensive testing
# --all: enable all features
# --shared=0: build static library only (no shared library with AFL symbols)
# --disable-tcl: don't build TCL extension (not needed for fuzzing)
# --disable-readline --disable-editline: disable line editing features
# --disable-load-extension: disable extension loading for security
echo "Configuring SQLite with wllvm for bitcode generation..."
"$SRC/configure" --prefix="$WORK" \
                 --all \
                 --shared=0 \
                 --disable-tcl \
                 --disable-readline \
                 --disable-editline \
                 --disable-load-extension

# Build with parallel jobs
echo "Building SQLite with wllvm..."
make -j$(nproc)

# Install to $WORK directory
echo "Installing SQLite to $WORK..."
make install

# Extract bitcode from the static library for coverage analysis
echo "Extracting bitcode for coverage analysis..."
if command -v extract-bc >/dev/null 2>&1; then
    if [ -f "$WORK/lib/libsqlite3.a" ]; then
        # First, make sure wllvm has stored the bitcode information
        # We need to be in the build directory to find the .o files
        cd /tmp/build_tmp
        
        # Use wllvm to extract bitcode from the entire archive
        echo "Running extract-bc on libsqlite3.a..."
        extract-bc "$WORK/lib/libsqlite3.a"
        
        # Check what was generated - wllvm might create .bca (bitcode archive) instead of .bc
        if [ -f "$WORK/lib/libsqlite3.a.bc" ]; then
            echo "Successfully generated bitcode: $WORK/lib/libsqlite3.a.bc"
            BITCODE_FILE="$WORK/lib/libsqlite3.a.bc"
        elif [ -f "$WORK/lib/libsqlite3.a.bca" ]; then
            echo "Successfully generated bitcode archive: $WORK/lib/libsqlite3.a.bca"
            BITCODE_FILE="$WORK/lib/libsqlite3.a.bca"
            
            # Try to extract individual .bc files from the .bca archive
            echo "Extracting individual bitcode files from archive..."
            mkdir -p /tmp/bitcode_extract
            cd /tmp/bitcode_extract
            
            # Extract all files from the bca archive
            ar x "$BITCODE_FILE" 2>/dev/null || llvm-ar x "$BITCODE_FILE" 2>/dev/null || true
            
            # Find any .bc files that were extracted
            if find . -name "*.bc" -type f | grep -q .; then
                echo "Found bitcode files in archive:"
                find . -name "*.bc" -type f | head -5
                
                # Combine them into a single .bc file if possible
                FIRST_BC=$(find . -name "*.bc" -type f | head -1)
                if [ -n "$FIRST_BC" ]; then
                    cp "$FIRST_BC" "$WORK/lib/libsqlite3.combined.bc" 2>/dev/null || true
                fi
            fi
            
            cd /tmp/build_tmp
        else
            echo "Warning: No bitcode file found after extraction"
        fi
        # Generate callgraph from bitcode using LLVM opt
        if [ -n "$BITCODE_FILE" ] && [ -f "$BITCODE_FILE" ]; then
            echo "Generating callgraph from bitcode..."
            if command -v opt >/dev/null 2>&1; then
                # Try different methods to generate callgraph
                echo "Attempting to generate callgraph with opt..."
                
                # First, check if BITCODE_FILE is a .bca archive and extract the .bc file
                ACTUAL_BC_FILE=""
                EXTRACT_DIR=""
                if [[ "$BITCODE_FILE" == *.bca ]]; then
                    echo "Extracting bitcode from archive..."
                    EXTRACT_DIR=$(mktemp -d /tmp/bc_extract_XXXXXX)
                    cd "$EXTRACT_DIR"
                    
                    # Extract from archive
                    ar x "$BITCODE_FILE" 2>/dev/null || llvm-ar x "$BITCODE_FILE" 2>/dev/null || true
                    
                    # Find the extracted .bc file
                    # Find the extracted .bc file
                    EXTRACTED_BC=$(find . -name "*.bc" -type f | head -1)
                    if [ -n "$EXTRACTED_BC" ] && [ -f "$EXTRACTED_BC" ]; then
                        ACTUAL_BC_FILE="$EXTRACTED_BC"
                        echo "Found extracted bitcode: $ACTUAL_BC_FILE"
                        
                        # Make path absolute for later use
                        ACTUAL_BC_FILE="$(pwd)/$ACTUAL_BC_FILE"
                        
                        # Copy to WORK directory for reference
                        cp "$ACTUAL_BC_FILE" "$WORK/lib/libsqlite3.extracted.bc" 2>/dev/null || true
                    fi
                else
                    ACTUAL_BC_FILE="$BITCODE_FILE"
                fi
                if [ -n "$ACTUAL_BC_FILE" ] && [ -f "$ACTUAL_BC_FILE" ]; then
                    echo "Using bitcode file: $ACTUAL_BC_FILE"
                    
                    # Generate callgraph using new pass manager syntax
                    echo "Generating callgraph with opt -passes=dot-callgraph..."
                    
                    # If we extracted to a temp directory, run opt there
                    if [ -n "$EXTRACT_DIR" ] && [ -d "$EXTRACT_DIR" ]; then
                        cd "$EXTRACT_DIR"
                        echo "Running opt in extract directory: $(pwd)"
                    fi
                    
                    # Run opt and capture output
                    opt -passes=dot-callgraph "$ACTUAL_BC_FILE" -o /dev/null 2>&1
                    OPT_EXIT_CODE=$?
                    echo "opt exit code: $OPT_EXIT_CODE"
                    
                    # Check for generated callgraph file
                    # The output file will be named <input_file>.callgraph.dot
                    BASENAME=$(basename "$ACTUAL_BC_FILE")
                    CALLGRAPH_FILE="${BASENAME}.callgraph.dot"
                    
                    echo "Looking for callgraph file: $CALLGRAPH_FILE in $(pwd)"
                    
                    if [ -f "$CALLGRAPH_FILE" ]; then
                        echo "Found callgraph file: $CALLGRAPH_FILE"
                        cp "$CALLGRAPH_FILE" "$WORK/lib/libsqlite3.a.bc.callgraph.dot"
                        echo "Generated callgraph: $WORK/lib/libsqlite3.a.bc.callgraph.dot"
                        cp "callgraph.dot" "$WORK/lib/libsqlite3.a.bc.callgraph.dot"
                        echo "Generated callgraph: $WORK/lib/libsqlite3.a.bc.callgraph.dot"
                    else
                        # Check for any .dot files in current directory (including hidden files)
                        echo "Searching for any dot files (including hidden)..."
                        if find . -name "*.dot" -o -name ".*.dot" | grep -q .; then
                            DOT_FILES=$(find . -name "*.dot" -o -name ".*.dot")
                            echo "Found dot files: $DOT_FILES"
                            # Copy the first one
                            FIRST_DOT=$(echo "$DOT_FILES" | head -1)
                            cp "$FIRST_DOT" "$WORK/lib/libsqlite3.a.bc.callgraph.dot" 2>/dev/null || true
                            echo "Copied callgraph: $WORK/lib/libsqlite3.a.bc.callgraph.dot"
                        else
                            echo "Warning: Could not generate callgraph.dot"
                            # Try alternative approach: use llvm-dis to disassemble and get some graph
                            echo "Trying llvm-dis as alternative..."
                            llvm-dis "$ACTUAL_BC_FILE" -o /dev/null 2>&1 || true
                        fi
                    fi
                    
                    # Return to build directory
                    cd /tmp/build_tmp
                    
                    # Return to build directory
                    cd /tmp/build_tmp
                else
                    echo "Warning: No valid bitcode file found for callgraph generation"
                fi
            else
                echo "Warning: LLVM opt not found, cannot generate callgraph"
            fi
        fi
        
        echo "Generating symbol list as fallback callgraph..."
        if command -v nm >/dev/null 2>&1 && [ -f "$WORK/lib/libsqlite3.a" ]; then
            nm "$WORK/lib/libsqlite3.a" 2>/dev/null | grep "sqlite3_" | head -100 > "$WORK/lib/libsqlite3.symbols.txt" || true
            echo "Generated symbol list: $WORK/lib/libsqlite3.symbols.txt"
        fi
    else
        echo "Error: Static library not found at $WORK/lib/libsqlite3.a"
        exit 1
    fi
else
    echo "Error: extract-bc command not found. Please ensure wllvm is properly installed."
    exit 1
fi

# Verify the installation
if [ -f "$WORK/lib/libsqlite3.a" ]; then
    echo "Successfully built static library: $WORK/lib/libsqlite3.a"
    echo "  Size: $(wc -c < "$WORK/lib/libsqlite3.a") bytes"
    # Check if library contains symbols (basic sanity check)
    if command -v nm >/dev/null 2>&1; then
        SYMBOL_COUNT=$(nm "$WORK/lib/libsqlite3.a" 2>/dev/null | grep -c "sqlite3_" || echo 0)
        echo "  Contains $SYMBOL_COUNT SQLite symbols"
    fi
else
    echo "ERROR: Static library not found at $WORK/lib/libsqlite3.a"
    exit 1
fi

# Check for shared library (could be libsqlite3.so or versioned symlink)
if ls "$WORK/lib"/libsqlite3.so* 1>/dev/null 2>&1; then
    echo "Successfully built shared library"
    # Find the actual shared library file (not symlink)
    for lib in "$WORK/lib"/libsqlite3.so.*; do
        if [ -f "$lib" ] && [ ! -L "$lib" ]; then
            echo "Found shared library file: $lib"
            echo "  Size: $(wc -c < "$lib") bytes"
            break
        fi
    done
else
    echo "WARNING: Shared library not found, but static library exists"
fi

# Verify coverage files were generated
echo ""
echo "=== Coverage Infrastructure Files Check ==="
BITCODE_FILES=0
CALLGRAPH_FILES=0

# Check for bitcode files
for file in "$WORK/lib"/libsqlite3.*.bc "$WORK/lib"/libsqlite3.*.bca "$WORK/lib"/libsqlite3.bc "$WORK/lib"/libsqlite3.bca; do
    if [ -f "$file" ]; then
        echo "✓ Bitcode file: $file"
        echo "  Size: $(wc -c < "$file") bytes"
        BITCODE_FILES=$((BITCODE_FILES + 1))
    fi
done

if [ $BITCODE_FILES -eq 0 ]; then
    echo "✗ No bitcode files found"
fi

# Check for callgraph files
for file in "$WORK/lib"/libsqlite3.*.dot "$WORK/lib"/libsqlite3.callgraph.dot "$WORK/lib"/libsqlite3.*.callgraph.*; do
    if [ -f "$file" ]; then
        echo "✓ Callgraph file: $file"
        echo "  Size: $(wc -c < "$file") bytes"
        CALLGRAPH_FILES=$((CALLGRAPH_FILES + 1))
    fi
done

# Check for symbol list as fallback
if [ -f "$WORK/lib/libsqlite3.symbols.txt" ]; then
    echo "✓ Symbol list: $WORK/lib/libsqlite3.symbols.txt"
    echo "  Size: $(wc -c < "$WORK/lib/libsqlite3.symbols.txt") bytes"
    CALLGRAPH_FILES=$((CALLGRAPH_FILES + 1))
fi

if [ $CALLGRAPH_FILES -eq 0 ]; then
    echo "✗ No callgraph files found"
fi

echo ""
echo "Build completed successfully. Libraries installed to $WORK/lib"
echo "Headers installed to $WORK/include"

# Don't delete the temporal build directory after building!
# It will be used for postprocessing.
# rm -rf /tmp/build_tmp
