#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "zlib.h"

namespace harness_001 {
/*
 * Fuzzing harness for zlib gzip file I/O subsystem APIs
 * Target: gzopen, gzread, gzwrite, gzgets, gzgetc, gzungetc, gzputc, gzputs, gzclose, gzerror
 * Harness ID: 001
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create a temporary file for gzip operations
    char temp_filename[] = "/tmp/gzip_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        return 0;
    }
    close(fd);

    // Determine gzip mode from fuzzer input
    std::string mode = "wb";
    uint8_t mode_selector = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (mode_selector) {
        case 0: mode = "rb"; break;  // Read mode
        case 1: mode = "wb"; break;  // Write mode
        case 2: mode = "ab"; break;  // Append mode
        case 3: mode = "wb6"; break; // Write with compression level 6
    }

    // Open gzip file
    gzFile gzfile = gzopen(temp_filename, mode.c_str());
    if (gzfile == NULL) {
        // Try with a different mode if opening failed
        mode = "wb";
        gzfile = gzopen(temp_filename, mode.c_str());
        if (gzfile == NULL) {
            remove(temp_filename);
            return 0;
        }
    }

    // Determine number of operations to perform
    uint8_t num_operations = fdp.ConsumeIntegral<uint8_t>() % 16 + 1;

    for (uint8_t i = 0; i < num_operations && fdp.remaining_bytes() > 0; i++) {
        // Choose operation type
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 10;

        switch (operation) {
            case 0: {  // gzputc - write a single character
                if (mode[0] == 'w' || mode[0] == 'a') {
                    char c = fdp.ConsumeIntegral<uint8_t>();
                    gzputc(gzfile, c);
                }
                break;
            }
            case 1: {  // gzputs - write a string
                if (mode[0] == 'w' || mode[0] == 'a') {
                    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    std::string str = fdp.ConsumeRandomLengthString(str_len);
                    gzputs(gzfile, str.c_str());
                }
                break;
            }
            case 2: {  // gzwrite - write binary data
                if (mode[0] == 'w' || mode[0] == 'a') {
                    size_t write_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                    if (fdp.remaining_bytes() < write_len) {
                        break;
                    }
                    std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(write_len);
                    gzwrite(gzfile, buffer.data(), buffer.size());
                }
                break;
            }
            case 3: {  // gzread - read data
                if (mode[0] == 'r') {
                    size_t read_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                    std::vector<uint8_t> buffer(read_len);
                    gzread(gzfile, buffer.data(), read_len);
                }
                break;
            }
            case 4: {  // gzgetc - read a single character
                if (mode[0] == 'r') {
                    gzgetc(gzfile);
                }
                break;
            }
            case 5: {  // gzungetc - push back a character
                if (mode[0] == 'r') {
                    char c = fdp.ConsumeIntegral<uint8_t>();
                    gzungetc(c, gzfile);
                }
                break;
            }
            case 6: {  // gzgets - read a string
                if (mode[0] == 'r') {
                    size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                    std::vector<char> buffer(buf_size);
                    gzgets(gzfile, buffer.data(), buf_size);
                }
                break;
            }
            case 7: {  // gzflush - flush output
                if (mode[0] == 'w' || mode[0] == 'a') {
                    int flush_mode = fdp.ConsumeIntegral<uint8_t>() % 6;
                    gzflush(gzfile, flush_mode);
                }
                break;
            }
            case 8: {  // gztell - get current position
                gztell(gzfile);
                break;
            }
            case 9: {  // gzerror - check for errors
                int errnum;
                const char* errmsg = gzerror(gzfile, &errnum);
                // errmsg can be used for debugging but we don't need to store it
                (void)errmsg;
                break;
            }
        }
    }

    // Close the gzip file
    gzclose(gzfile);

    // Now open the file in read mode to test reading what was written
    if (mode[0] == 'w' || mode[0] == 'a') {
        gzFile read_file = gzopen(temp_filename, "rb");
        if (read_file != NULL) {
            // Try reading some data
            size_t read_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            if (fdp.remaining_bytes() >= read_len) {
                std::vector<uint8_t> buffer(read_len);
                gzread(read_file, buffer.data(), read_len);
                
                // Try gzgets on what we just read (seek back)
                gzrewind(read_file);
                std::vector<char> str_buf(256);
                gzgets(read_file, str_buf.data(), 256);
                
                // Check error status
                int errnum;
                gzerror(read_file, &errnum);
            }
            gzclose(read_file);
        }
    }

    // Clean up temporary file
    remove(temp_filename);

    return 0;
}

} // namespace harness_001
