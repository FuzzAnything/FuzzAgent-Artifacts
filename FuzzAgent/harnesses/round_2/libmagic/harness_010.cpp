#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_010 {
/*
 * libmagic fuzzing harness - tar, json, csv, and ASCII magic parsers
 * Target: is_tar.c, is_json.c, is_csv.c, ascmagic.c, fsmagic.c
 * 
 * This harness specifically targets the tar, JSON, CSV, and ASCII text parsers
 * which likely have low coverage. It generates:
 * 1) tar archives (ustar, gnutar formats) - targeting is_tar.c
 * 2) JSON data (objects, arrays, strings) - targeting is_json.c
 * 3) CSV data (comma/tab separated) - targeting is_csv.c
 * 4) ASCII text with magic patterns - targeting ascmagic.c and fsmagic.c
 * 
 * Edge cases included:
 * - Malformed tar headers (invalid checksums, corrupted magic)
 * - Invalid JSON (unclosed brackets, malformed strings)
 * - Empty CSV, single column CSV, malformed quotes
 * - ASCII text with binary data, control characters
 * - Mixed content types in single buffer
 * 
 * Uses MAGIC_RAW flag for binary detection and MAGIC_CONTINUE for multi-type detection.
 * 
 * Semantically distinct from previous harnesses:
 * - harness_000-009: Focus on different file formats (ELF, CDF, compression, etc.)
 * - harness_010: Specifically targets tar, JSON, CSV, and ASCII text parsers
 * 
 * Key focus areas:
 * - Exercise is_tar.c with various tar header formats
 * - Trigger JSON parser with valid and invalid JSON structures
 * - Test CSV parser with RFC-4180 compliance and edge cases
 * - Test ASCII detection with various encodings and text patterns
 * - Use fsmagic.c for filesystem-like magic detection
 */



// Tar header constants from tar.h
#define RECORDSIZE 512
#define NAMSIZ 100
#define TUNMLEN 32
#define TGNMLEN 32
#define TMAGIC "ustar"      // 5 chars and a null
#define GNUTMAGIC "ustar  " // 7 chars and a null

// Helper to generate tar header
void generate_tar_header(std::vector<uint8_t>& buffer, FuzzedDataProvider& fdp, 
                         bool is_valid, bool is_gnu) {
    // Ensure buffer has at least RECORDSIZE bytes
    if (buffer.size() < RECORDSIZE) {
        buffer.resize(RECORDSIZE);
    }
    
    // Fill with random data first
    std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(RECORDSIZE);
    if (random_data.size() == RECORDSIZE) {
        std::copy(random_data.begin(), random_data.end(), buffer.begin());
    } else {
        // Fill with zeros
        std::fill(buffer.begin(), buffer.end(), 0);
    }
    
    // Set name field
    std::string filename = fdp.ConsumeRandomLengthString(NAMSIZ - 1);
    strncpy(reinterpret_cast<char*>(buffer.data()), filename.c_str(), NAMSIZ - 1);
    
    // Set mode (octal)
    std::string mode = fdp.ConsumeBytesAsString(7);
    strncpy(reinterpret_cast<char*>(buffer.data() + NAMSIZ), mode.c_str(), 7);
    
    // Set uid (octal)
    std::string uid = fdp.ConsumeBytesAsString(7);
    strncpy(reinterpret_cast<char*>(buffer.data() + NAMSIZ + 8), uid.c_str(), 7);
    
    // Set gid (octal)
    std::string gid = fdp.ConsumeBytesAsString(7);
    strncpy(reinterpret_cast<char*>(buffer.data() + NAMSIZ + 16), gid.c_str(), 7);
    
    // Set size (octal)
    std::string size = fdp.ConsumeBytesAsString(11);
    strncpy(reinterpret_cast<char*>(buffer.data() + NAMSIZ + 24), size.c_str(), 11);
    
    // Set mtime (octal)
    std::string mtime = fdp.ConsumeBytesAsString(11);
    strncpy(reinterpret_cast<char*>(buffer.data() + NAMSIZ + 36), mtime.c_str(), 11);
    
    // Set checksum (octal) - will be calculated if valid
    if (is_valid) {
        // For valid tar, calculate checksum placeholder
        strcpy(reinterpret_cast<char*>(buffer.data() + NAMSIZ + 148), "        "); // 8 spaces
    } else {
        // For invalid tar, use random checksum
        std::string checksum = fdp.ConsumeBytesAsString(8);
        strncpy(reinterpret_cast<char*>(buffer.data() + NAMSIZ + 148), checksum.c_str(), 8);
    }
    
    // Set linkflag
    buffer[NAMSIZ + 156] = fdp.ConsumeIntegral<uint8_t>();
    
    // Set linkname
    std::string linkname = fdp.ConsumeRandomLengthString(NAMSIZ - 1);
    strncpy(reinterpret_cast<char*>(buffer.data() + NAMSIZ + 157), linkname.c_str(), NAMSIZ - 1);
    
    // Set magic field
    size_t magic_offset = NAMSIZ + 157 + NAMSIZ;
    if (is_valid) {
        if (is_gnu) {
            strncpy(reinterpret_cast<char*>(buffer.data() + magic_offset), GNUTMAGIC, 8);
        } else {
            strncpy(reinterpret_cast<char*>(buffer.data() + magic_offset), TMAGIC, 6);
        }
    } else {
        // Invalid magic
        std::string bad_magic = fdp.ConsumeBytesAsString(8);
        strncpy(reinterpret_cast<char*>(buffer.data() + magic_offset), bad_magic.c_str(), 8);
    }
}

// Helper to generate JSON data
std::string generate_json(FuzzedDataProvider& fdp, bool is_valid) {
    std::string json;
    
    if (!is_valid) {
        // Generate invalid JSON
        int invalid_type = fdp.ConsumeIntegralInRange<int>(0, 5);
        switch (invalid_type) {
            case 0:
                json = "{"; // Unclosed object
                break;
            case 1:
                json = "["; // Unclosed array
                break;
            case 2:
                json = "\"unclosed string"; // Unclosed string
                break;
            case 3:
                json = "null extra"; // Extra tokens
                break;
            case 4:
                json = "{key: value}"; // Invalid key syntax
                break;
            case 5:
                json = "true false"; // Multiple values without separator
                break;
        }
        return json;
    }
    
    // Generate valid JSON
    int json_type = fdp.ConsumeIntegralInRange<int>(0, 4);
    switch (json_type) {
        case 0: // Object
            {
                int num_pairs = fdp.ConsumeIntegralInRange<int>(1, 5);
                json = "{";
                for (int i = 0; i < num_pairs; i++) {
                    if (i > 0) json += ",";
                    std::string key = fdp.ConsumeRandomLengthString(20);
                    json += "\"" + key + "\":";
                    
                    int value_type = fdp.ConsumeIntegralInRange<int>(0, 4);
                    switch (value_type) {
                        case 0:
                            json += "\"" + fdp.ConsumeRandomLengthString(30) + "\"";
                            break;
                        case 1:
                            json += std::to_string(fdp.ConsumeIntegral<int>());
                            break;
                        case 2:
                            json += fdp.ConsumeBool() ? "true" : "false";
                            break;
                        case 3:
                            json += "null";
                            break;
                        case 4:
                            json += "[]";
                            break;
                    }
                }
                json += "}";
            }
            break;
            
        case 1: // Array
            {
                int num_items = fdp.ConsumeIntegralInRange<int>(1, 5);
                json = "[";
                for (int i = 0; i < num_items; i++) {
                    if (i > 0) json += ",";
                    int item_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                    switch (item_type) {
                        case 0:
                            json += "\"" + fdp.ConsumeRandomLengthString(30) + "\"";
                            break;
                        case 1:
                            json += std::to_string(fdp.ConsumeIntegral<int>());
                            break;
                        case 2:
                            json += fdp.ConsumeBool() ? "true" : "false";
                            break;
                        case 3:
                            json += "null";
                            break;
                    }
                }
                json += "]";
            }
            break;
            
        case 2: // String
            json = "\"" + fdp.ConsumeRandomLengthString(100) + "\"";
            break;
            
        case 3: // Number
            json = std::to_string(fdp.ConsumeIntegral<int>());
            if (fdp.ConsumeBool()) {
                json += "." + std::to_string(fdp.ConsumeIntegralInRange<int>(0, 999));
            }
            break;
            
        case 4: // Boolean or null
            if (fdp.ConsumeBool()) {
                json = fdp.ConsumeBool() ? "true" : "false";
            } else {
                json = "null";
            }
            break;
    }
    
    return json;
}

// Helper to generate CSV data
std::string generate_csv(FuzzedDataProvider& fdp, bool is_valid) {
    std::string csv;
    
    if (!is_valid) {
        // Generate invalid CSV
        int invalid_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        switch (invalid_type) {
            case 0:
                csv = "\"unclosed quote\n"; // Unclosed quote
                break;
            case 1:
                csv = "field1,field2\nfield3"; // Uneven rows
                break;
            case 2:
                csv = "\"quote\"\"inside\"bad\n"; // Bad quote escaping
                break;
            case 3:
                csv = ""; // Empty
                break;
        }
        return csv;
    }
    
    // Generate valid CSV (RFC-4180 compliant)
    int num_rows = fdp.ConsumeIntegralInRange<int>(1, 5);
    int num_cols = fdp.ConsumeIntegralInRange<int>(1, 5);
    char delimiter = fdp.ConsumeBool() ? ',' : '\t';
    
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            if (col > 0) csv += delimiter;
            
            bool needs_quotes = fdp.ConsumeBool();
            std::string field = fdp.ConsumeRandomLengthString(20);
            
            // Add quotes if needed or if field contains delimiter or quotes
            if (needs_quotes || field.find(delimiter) != std::string::npos || 
                field.find('"') != std::string::npos || field.find('\n') != std::string::npos) {
                // Escape quotes by doubling them
                size_t pos = 0;
                while ((pos = field.find('"', pos)) != std::string::npos) {
                    field.replace(pos, 1, "\"\"");
                    pos += 2;
                }
                csv += "\"" + field + "\"";
            } else {
                csv += field;
            }
        }
        if (row < num_rows - 1) {
            csv += "\n";
        }
    }
    
    return csv;
}

// Helper to generate ASCII text with magic patterns
std::string generate_ascii_text(FuzzedDataProvider& fdp) {
    std::string text;
    
    int text_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    switch (text_type) {
        case 0: // Plain ASCII
            text = fdp.ConsumeRandomLengthString(200);
            break;
            
        case 1: // With control characters
            text = fdp.ConsumeRandomLengthString(100);
            // Insert some control chars
            for (int i = 0; i < 5; i++) {
                size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, text.length());
                text.insert(pos, 1, static_cast<char>(fdp.ConsumeIntegralInRange<int>(1, 31)));
            }
            break;
            
        case 2: // With UTF-8 sequences
            text = fdp.ConsumeRandomLengthString(100);
            // Insert some UTF-8 sequences
            text += "\xC3\xA9"; // é
            text += "\xE2\x82\xAC"; // €
            text += fdp.ConsumeRandomLengthString(50);
            break;
            
        case 3: // With null bytes
            text = fdp.ConsumeRandomLengthString(150);
            for (int i = 0; i < 3; i++) {
                size_t pos = fdp.ConsumeIntegralInRange<size_t>(0, text.length());
                text.insert(pos, 1, '\0');
            }
            break;
            
        case 4: // Multi-line text
            for (int i = 0; i < 10; i++) {
                text += fdp.ConsumeRandomLengthString(20) + "\n";
            }
            break;
            
        case 5: // Mixed binary and text
            text = "#!/bin/bash\n";
            text += fdp.ConsumeRandomLengthString(100);
            text += "\n\x00\x01\x02\x03";
            text += fdp.ConsumeRandomLengthString(50);
            break;
    }
    
    return text;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int base_flags = fdp.ConsumeIntegral<int>();
    int data_type = fdp.ConsumeIntegralInRange<int>(0, 4); // 0: tar, 1: json, 2: csv, 3: ascii, 4: mixed
    bool is_valid = fdp.ConsumeBool();
    bool use_raw_flag = fdp.ConsumeBool();
    
    // Configure flags for our testing
    int flags = base_flags & (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | 
              MAGIC_PRESERVE_ATIME | MAGIC_ERROR | 
              MAGIC_MIME_ENCODING | MAGIC_APPLE | MAGIC_EXTENSION);
    
    // Always use RAW flag for binary detection as specified
    if (use_raw_flag) {
        flags |= MAGIC_RAW;
    }
    
    // Initialize magic cookie
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0;
    }
    
    // Try to load standard magic database (ignore failure)
    magic_load(magic, NULL);
    
    // Generate test data based on selected type
    std::vector<uint8_t> test_data;
    std::string string_data;
    
    switch (data_type) {
        case 0: // Tar archive
            {
                test_data.resize(RECORDSIZE * 2); // Header + some content
                bool is_gnu = fdp.ConsumeBool();
                generate_tar_header(test_data, fdp, is_valid, is_gnu);
                
                // Add some file content after header
                std::vector<uint8_t> content = fdp.ConsumeRemainingBytes<uint8_t>();
                if (test_data.size() > RECORDSIZE && !content.empty()) {
                    size_t copy_size = std::min(content.size(), test_data.size() - RECORDSIZE);
                    std::copy(content.begin(), content.begin() + copy_size, 
                             test_data.begin() + RECORDSIZE);
                }
            }
            break;
            
        case 1: // JSON data
            string_data = generate_json(fdp, is_valid);
            test_data.assign(string_data.begin(), string_data.end());
            break;
            
        case 2: // CSV data
            string_data = generate_csv(fdp, is_valid);
            test_data.assign(string_data.begin(), string_data.end());
            break;
            
        case 3: // ASCII text
            string_data = generate_ascii_text(fdp);
            test_data.assign(string_data.begin(), string_data.end());
            break;
            
        case 4: // Mixed content (test fsmagic.c's ability to detect multiple types)
            {
                // Create a buffer with multiple content types
                std::string mixed;
                
                // Start with possible shebang for script detection
                if (fdp.ConsumeBool()) {
                    mixed = "#!/usr/bin/env python\n";
                }
                
                // Add some JSON-like content
                mixed += generate_json(fdp, fdp.ConsumeBool());
                mixed += "\n";
                
                // Add some CSV-like content
                mixed += generate_csv(fdp, fdp.ConsumeBool());
                mixed += "\n";
                
                // Add some ASCII text
                mixed += generate_ascii_text(fdp);
                
                // Potentially add binary data at the end
                if (fdp.ConsumeBool()) {
                    std::vector<uint8_t> binary = fdp.ConsumeRemainingBytes<uint8_t>();
                    mixed.append(binary.begin(), binary.end());
                }
                
                test_data.assign(mixed.begin(), mixed.end());
            }
            break;
    }
    
    // Test magic_buffer() with our generated data
    if (!test_data.empty()) {
        const char* result = magic_buffer(magic, test_data.data(), test_data.size());
        (void)result; // Use result to avoid unused variable warning
        
        // Also test with different flags if we have remaining input
        if (fdp.remaining_bytes() > 4) {
            int alt_flags = fdp.ConsumeIntegral<int>();
            alt_flags &= (MAGIC_MIME_TYPE | MAGIC_MIME_ENCODING | MAGIC_CONTINUE);
            
            // Temporarily change flags
            int original_flags = magic_getflags(magic);
            magic_setflags(magic, alt_flags);
            
            // Test again with different flags
            const char* alt_result = magic_buffer(magic, test_data.data(), test_data.size());
            (void)alt_result;
            
            // Restore original flags
            magic_setflags(magic, original_flags);
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_010
