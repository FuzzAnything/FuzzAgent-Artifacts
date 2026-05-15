#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_007 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size: need enough data for operation selection and at least some buffer data
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    int block_size = fdp.ConsumeIntegralInRange<int>(-1, 8192);
    int read_size = fdp.ConsumeIntegralInRange<int>(1, 4096);
    int skip_amount = fdp.ConsumeIntegralInRange<int>(0, 1024);
    int backup_amount = fdp.ConsumeIntegralInRange<int>(0, 512);
    
    // Consume variable-size data for file content
    size_t file_content_size = fdp.ConsumeIntegralInRange<size_t>(1, 8192);
    std::vector<uint8_t> file_content = fdp.ConsumeBytes<uint8_t>(file_content_size);
    
    // Create a temporary file for testing
    char temp_filename[] = "/tmp/protobuf_fuzz_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd == -1) {
        return 0;
    }
    
    // Write initial content to the temporary file
    if (write(temp_fd, file_content.data(), file_content.size()) != static_cast<ssize_t>(file_content.size())) {
        close(temp_fd);
        unlink(temp_filename);
        return 0;
    }
    
    // Close the file descriptor to allow reopening with FileInputStream
    close(temp_fd);
    
    // Reopen the file for reading
    int read_fd = open(temp_filename, O_RDONLY);
    if (read_fd == -1) {
        unlink(temp_filename);
        return 0;
    }
    
    // Test different operations based on fuzzer input
    switch (operation) {
        case 0: {
            // Test FileInputStream operations: Read, Skip, BackUp
            google::protobuf::io::FileInputStream file_input(read_fd, block_size);
            
            // Create buffer for reading
            std::vector<uint8_t> read_buffer(read_size);
            
            // Get the underlying CopyingFileInputStream for direct Read calls
            // Note: The CopyingFileInputStream is private, so we can't access it directly.
            // Instead, we'll use the public FileInputStream methods.
            
            // Try to read using the FileInputStream's Next method
            const void* buffer_ptr = nullptr;
            int buffer_size = 0;
            if (file_input.Next(&buffer_ptr, &buffer_size)) {
                // Successfully got a buffer
                // Back up some amount (if valid)
                if (backup_amount > 0 && backup_amount <= buffer_size) {
                    file_input.BackUp(backup_amount);
                }
            }
            
            // Try to skip bytes
            if (skip_amount > 0) {
                file_input.Skip(skip_amount);
            }
            
            // Close the input stream
            file_input.Close();
            break;
        }
        
        case 1: {
            // Test FileOutputStream operations: Write
            // Create a temporary output file
            char output_filename[] = "/tmp/protobuf_fuzz_out_XXXXXX";
            int output_fd = mkstemp(output_filename);
            if (output_fd == -1) {
                close(read_fd);
                unlink(temp_filename);
                return 0;
            }
            
            google::protobuf::io::FileOutputStream file_output(output_fd, block_size);
            
            // Write some data using the FileOutputStream
            // Get buffer from Next and write to it
            void* output_buffer = nullptr;
            int output_buffer_size = 0;
            if (file_output.Next(&output_buffer, &output_buffer_size)) {
                // Write some data from our file content to the buffer
                size_t write_size = std::min(file_content.size(), static_cast<size_t>(output_buffer_size));
                if (write_size > 0) {
                    memcpy(output_buffer, file_content.data(), write_size);
                    
                    // If we didn't use the full buffer, back up
                    if (write_size < static_cast<size_t>(output_buffer_size)) {
                        file_output.BackUp(output_buffer_size - write_size);
                    }
                } else {
                    // Back up entire buffer if no data written
                    file_output.BackUp(output_buffer_size);
                }
            }
            
            // Close the output stream
            file_output.Close();
            
            // Clean up output file
            close(output_fd);
            unlink(output_filename);
            
            // Close input file
            close(read_fd);
            unlink(temp_filename);
            break;
        }
        
        case 2: {
            // Test both input and output streams together (copy operation)
            // Create a temporary output file
            char output_filename[] = "/tmp/protobuf_fuzz_out_XXXXXX";
            int output_fd = mkstemp(output_filename);
            if (output_fd == -1) {
                close(read_fd);
                unlink(temp_filename);
                return 0;
            }
            
            google::protobuf::io::FileInputStream file_input(read_fd, block_size);
            google::protobuf::io::FileOutputStream file_output(output_fd, block_size);
            
            // Simple copy: read from input, write to output
            const void* read_buffer = nullptr;
            int read_buffer_size = 0;
            void* write_buffer = nullptr;
            int write_buffer_size = 0;
            
            // Try to get buffers and copy data
            if (file_input.Next(&read_buffer, &read_buffer_size) && 
                file_output.Next(&write_buffer, &write_buffer_size)) {
                
                size_t copy_size = std::min(static_cast<size_t>(read_buffer_size), 
                                          static_cast<size_t>(write_buffer_size));
                if (copy_size > 0) {
                    memcpy(write_buffer, read_buffer, copy_size);
                    
                    // Handle potential size mismatches
                    if (copy_size < static_cast<size_t>(read_buffer_size)) {
                        file_input.BackUp(read_buffer_size - copy_size);
                    }
                    if (copy_size < static_cast<size_t>(write_buffer_size)) {
                        file_output.BackUp(write_buffer_size - copy_size);
                    }
                }
            }
            
            // Test Skip on input
            if (skip_amount > 0) {
                file_input.Skip(skip_amount);
            }
            
            // Test BackUp on input
            if (file_input.Next(&read_buffer, &read_buffer_size)) {
                if (backup_amount > 0 && backup_amount <= read_buffer_size) {
                    file_input.BackUp(backup_amount);
                }
            }
            
            // Close both streams
            file_input.Close();
            file_output.Close();
            
            // Clean up output file
            close(output_fd);
            unlink(output_filename);
            
            // Close input file descriptor (already closed by stream)
            unlink(temp_filename);
            break;
        }
        
        case 3: {
            // Test error handling and edge cases
            google::protobuf::io::FileInputStream file_input(read_fd, block_size);
            
            // Test with very small block size
            if (block_size < 0) {
                // Test default block size behavior
                const void* buffer = nullptr;
                int buffer_size = 0;
                file_input.Next(&buffer, &buffer_size);
            }
            
            // Test ByteCount
            int64_t byte_count = file_input.ByteCount();
            
            // Test multiple Next/BackUp sequences
            for (int i = 0; i < 3; i++) {
                const void* buffer = nullptr;
                int buffer_size = 0;
                if (file_input.Next(&buffer, &buffer_size)) {
                    int back_up = fdp.ConsumeIntegralInRange<int>(0, buffer_size);
                    if (back_up > 0) {
                        file_input.BackUp(back_up);
                    }
                }
            }
            
            // Close the stream
            file_input.Close();
            
            // Close the file descriptor
            close(read_fd);
            unlink(temp_filename);
            break;
        }
    }
    
    // Clean up (some cases already cleaned up, but ensure cleanup)
    if (read_fd != -1) {
        close(read_fd);
    }
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_007
