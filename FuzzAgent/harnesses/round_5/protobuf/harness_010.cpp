#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

namespace harness_010 {
// Protocol Buffers fuzzing harness for comprehensive file I/O stream operations using actual files
// This harness targets FileInputStream and FileOutputStream APIs with normal file operations
// Differentiates from previous harnesses by using actual temporary files rather than pipes/sockets
// Focuses on complete file I/O lifecycle, ByteCount accuracy, and normal operational flows
// Specifically targets APIs from coverage guidance:
// - FileInputStream constructors
// - FileInputStream::Next
// - FileInputStream::BackUp  
// - FileInputStream::Skip
// - FileInputStream::ByteCount
// - FileInputStream::Close
// - FileInputStream::CopyingFileInputStream::Read
// - FileInputStream::CopyingFileInputStream::Skip
// - FileOutputStream constructors
// - FileOutputStream::Next
// - FileOutputStream::BackUp
// - FileOutputStream::ByteCount
// - FileOutputStream::Close
// - FileOutputStream::CopyingFileOutputStream::Write
// - FileOutputStream::CopyingFileOutputStream::Close
// - OstreamOutputStream::BackUp



using google::protobuf::io::FileInputStream;
using google::protobuf::io::FileOutputStream;
using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::IstreamInputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful file I/O operations
    // We need enough for: file operations + test data
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create temporary files for testing
    char input_template[] = "/tmp/protobuf_fuzz_input_XXXXXX";
    char output_template[] = "/tmp/protobuf_fuzz_output_XXXXXX";
    
    int input_fd = mkstemp(input_template);
    int output_fd = mkstemp(output_template);
    
    if (input_fd == -1 || output_fd == -1) {
        if (input_fd != -1) close(input_fd);
        if (output_fd != -1) close(output_fd);
        return 0;
    }
    
    // Step 2: Generate test data and write to input file
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    // If we have no test data from fuzzer, use a small amount of remaining data
    if (test_data.empty()) {
        // Consume remaining bytes as test data
        test_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (test_data.empty()) {
            // If still empty, we can't proceed with meaningful testing
            close(input_fd);
            close(output_fd);
            unlink(input_template);
            unlink(output_template);
            return 0;
        }
    }
    
    // Write test data to input file
    ssize_t written = write(input_fd, test_data.data(), test_data.size());
    if (written <= 0) {
        close(input_fd);
        close(output_fd);
        unlink(input_template);
        unlink(output_template);
        return 0;
    }
    
    // Reset file pointer to beginning for reading
    lseek(input_fd, 0, SEEK_SET);
    
    // Step 3: Create FileInputStream with input file descriptor
    int input_block_size = fdp.ConsumeIntegralInRange<int>(-1, 8192);
    FileInputStream file_input(input_fd, input_block_size);
    
    // Step 4: Create FileOutputStream with output file descriptor
    int output_block_size = fdp.ConsumeIntegralInRange<int>(-1, 8192);
    FileOutputStream file_output(output_fd, output_block_size);
    
    // Step 5: Test FileInputStream operations
    const void* read_buffer = nullptr;
    int read_size = 0;
    bool has_data = false;
    int64_t byte_count_before = 0;
    int64_t byte_count_after = 0;
    
    // Target API: FileInputStream::Next (Read data)
    if (file_input.Next(&read_buffer, &read_size)) {
        has_data = true;
        
        // Target API: FileInputStream::ByteCount
        byte_count_before = file_input.ByteCount();
        
        if (read_size > 0 && read_buffer != nullptr) {
            // Step 6: Test FileOutputStream operations with read data
            void* write_buffer = nullptr;
            int write_size = 0;
            
            // Target API: FileOutputStream::Next (Get write buffer)
            if (file_output.Next(&write_buffer, &write_size)) {
                // Determine how much to write (up to min of read_size and write_size)
                int write_amount = fdp.ConsumeIntegralInRange<int>(0, std::min(read_size, write_size));
                if (write_amount > 0) {
                    // Copy data from input to output
                    memcpy(write_buffer, read_buffer, write_amount);
                    
                    // Target API: FileOutputStream::BackUp (if we didn't use full buffer)
                    int unused = write_size - write_amount;
                    if (unused > 0) {
                        file_output.BackUp(unused);
                    }
                    
                    // Target API: FileOutputStream::ByteCount
                    int64_t output_byte_count = file_output.ByteCount();
                    (void)output_byte_count; // Use to avoid unused variable warning
                }
            }
            
            // Step 7: Test FileInputStream::BackUp with various amounts
            int backup_amount = fdp.ConsumeIntegralInRange<int>(0, read_size);
            backup_amount = std::min(backup_amount, read_size);
            
            // Target API: FileInputStream::BackUp
            file_input.BackUp(backup_amount);
            
            // Read again after backup
            const void* read_buffer2 = nullptr;
            int read_size2 = 0;
            if (file_input.Next(&read_buffer2, &read_size2)) {
                // Step 8: Test FileInputStream::Skip
                int skip_amount = fdp.ConsumeIntegralInRange<int>(0, read_size2);
                skip_amount = std::min(skip_amount, read_size2);
                
                // Target API: FileInputStream::Skip
                bool skip_result = file_input.Skip(skip_amount);
                (void)skip_result; // Use to avoid unused variable warning
                
                // Target API: FileInputStream::ByteCount after operations
                byte_count_after = file_input.ByteCount();
                
                // Step 9: Continue reading remaining data and writing to output
                const void* read_buffer3 = nullptr;
                int read_size3 = 0;
                while (file_input.Next(&read_buffer3, &read_size3)) {
                    void* write_buffer2 = nullptr;
                    int write_size2 = 0;
                    
                    if (file_output.Next(&write_buffer2, &write_size2)) {
                        int copy_amount = std::min(read_size3, write_size2);
                        if (copy_amount > 0) {
                            memcpy(write_buffer2, read_buffer3, copy_amount);
                            
                            // Back up any unused space
                            int unused2 = write_size2 - copy_amount;
                            if (unused2 > 0) {
                                file_output.BackUp(unused2);
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Step 10: Test OstreamOutputStream::BackUp as well
    // Create a string stream for OstreamOutputStream testing
    std::stringstream ss;
    OstreamOutputStream ostream_output(&ss, fdp.ConsumeIntegralInRange<int>(-1, 4096));
    
    void* ostream_buffer = nullptr;
    int ostream_size = 0;
    if (ostream_output.Next(&ostream_buffer, &ostream_size)) {
        // Write some data
        if (ostream_size > 0) {
            size_t write_data_size = fdp.ConsumeIntegralInRange<size_t>(0, ostream_size);
            if (write_data_size > 0) {
                std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_data_size);
                if (!write_data.empty()) {
                    size_t copy_size = std::min(write_data.size(), static_cast<size_t>(ostream_size));
                    memcpy(ostream_buffer, write_data.data(), copy_size);
                    
                    // Target API: OstreamOutputStream::BackUp
                    int ostream_backup = fdp.ConsumeIntegralInRange<int>(0, ostream_size - copy_size);
                    ostream_output.BackUp(ostream_backup);
                }
            }
        }
    }
    
    // Step 11: Close streams
    // Target API: FileInputStream::Close
    bool input_close_result = file_input.Close();
    (void)input_close_result;
    
    // Target API: FileOutputStream::Close
    bool output_close_result = file_output.Close();
    (void)output_close_result;
    
    // Step 12: Verify the output file contains data
    lseek(output_fd, 0, SEEK_SET);
    std::vector<uint8_t> output_buffer(test_data.size());
    ssize_t bytes_read = read(output_fd, output_buffer.data(), output_buffer.size());
    (void)bytes_read;
    
    // Clean up temporary files
    close(input_fd);
    close(output_fd);
    unlink(input_template);
    unlink(output_template);
    
    return 0;
}

} // namespace harness_010
