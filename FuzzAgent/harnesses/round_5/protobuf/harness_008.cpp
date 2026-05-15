#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

namespace harness_008 {
// Protocol Buffers fuzzing harness for comprehensive file and stream I/O operations
// This harness targets file-based I/O APIs identified as having 200+ undiscovered branches
// Specifically targets: OstreamOutputStream::BackUp, FileInputStream::BackUp, FileOutputStream::Write,
// FileInputStream::Skip, FileInputStream::Read, FileOutputStream::Close, CopyingFileOutputStream::Close,
// IstreamInputStream::Skip, FileInputStream::Skip, CopyingFileInputStream::Close, FileInputStream::Close



using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ArrayOutputStream;
using google::protobuf::io::StringOutputStream;
using google::protobuf::io::IstreamInputStream;
using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::FileInputStream;
using google::protobuf::io::FileOutputStream;
using google::protobuf::io::CopyingInputStreamAdaptor;
using google::protobuf::io::CopyingInputStream;
using google::protobuf::io::CopyingOutputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful file I/O operations
    if (size < 64) return 0;  // Need enough for multiple parameters and stream data
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create test data for streams
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(32, 4096);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    if (test_data.empty()) return 0;
    
    // Step 2: Create pipe for file descriptor testing (simulates file I/O)
    int pipe_fds[2];
    if (pipe(pipe_fds) == -1) {
        // If pipe fails, use alternative approach with socket pair
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, pipe_fds) == -1) {
            return 0;  // Cannot create file descriptor, skip test
        }
    }
    
    // Step 3: Write test data to write end of pipe
    ssize_t written = write(pipe_fds[1], test_data.data(), test_data.size());
    if (written <= 0) {
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return 0;
    }
    
    // Step 4: Create FileInputStream with read end of pipe
    int block_size_input = fdp.ConsumeIntegralInRange<int>(-1, 65536);
    FileInputStream file_input(pipe_fds[0], block_size_input);
    
    // Step 5: Create another pipe for FileOutputStream testing
    int pipe_out_fds[2];
    if (pipe(pipe_out_fds) == -1) {
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, pipe_out_fds) == -1) {
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            return 0;
        }
    }
    
    int block_size_output = fdp.ConsumeIntegralInRange<int>(-1, 65536);
    FileOutputStream file_output(pipe_out_fds[1], block_size_output);
    
    // Step 6: Test FileInputStream operations (primary target APIs)
    
    // 6.1 Test FileInputStream::Read (via Next method)
    const void* read_buffer;
    int read_size;
    bool has_read = false;
    
    if (file_input.Next(&read_buffer, &read_size) && read_size > 0) {
        has_read = true;
        
        // 6.2 Test FileInputStream::BackUp (primary target)
        int backup_amount = fdp.ConsumeIntegralInRange<int>(0, read_size);
        backup_amount = std::min(backup_amount, read_size);
        backup_amount = std::max(backup_amount, 0);
        file_input.BackUp(backup_amount);  // Target API: FileInputStream::BackUp
        
        // Read again after backup
        if (file_input.Next(&read_buffer, &read_size) && read_size > 0) {
            // 6.3 Test FileInputStream::Skip (primary target - called twice for different implementations)
            int skip_amount = fdp.ConsumeIntegralInRange<int>(0, read_size);
            skip_amount = std::min(skip_amount, read_size);
            skip_amount = std::max(skip_amount, 0);
            file_input.Skip(skip_amount);  // Target API: FileInputStream::Skip
            
            // Check error status
            (void)file_input.GetErrno();
        }
    }
    
    // Step 7: Test FileOutputStream operations
    
    // 7.1 Test FileOutputStream::Write (via Next and BackUp)
    void* write_buffer;
    int write_size;
    
    if (file_output.Next(&write_buffer, &write_size) && write_size > 0) {
        // Write some data from fuzzer input
        size_t write_data_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min(test_data.size(), (size_t)write_size));
        if (write_data_size > 0 && test_data.size() >= write_data_size) {
            memcpy(write_buffer, test_data.data(), write_data_size);
            
            // 7.2 Test FileOutputStream BackUp (handled by parent class)
            int out_backup_amount = fdp.ConsumeIntegralInRange<int>(0, write_data_size);
            out_backup_amount = std::min(out_backup_amount, (int)write_data_size);
            out_backup_amount = std::max(out_backup_amount, 0);
            file_output.BackUp(out_backup_amount);
        }
        
        // Check error status
        (void)file_output.GetErrno();
    }
    
    // Step 8: Test IstreamInputStream and OstreamOutputStream operations
    // (to cover IstreamInputStream::Skip and OstreamOutputStream::BackUp)
    
    // 8.1 Create IstreamInputStream from std::istringstream
    std::string test_string(reinterpret_cast<const char*>(test_data.data()), test_data.size());
    std::istringstream iss(test_string);
    int istream_block_size = fdp.ConsumeIntegralInRange<int>(-1, 65536);
    IstreamInputStream istream_input(&iss, istream_block_size);
    
    // Test IstreamInputStream::Skip (primary target)
    if (istream_input.Next(&read_buffer, &read_size) && read_size > 0) {
        int istream_skip = fdp.ConsumeIntegralInRange<int>(0, read_size);
        istream_skip = std::min(istream_skip, read_size);
        istream_skip = std::max(istream_skip, 0);
        istream_input.Skip(istream_skip);  // Target API: IstreamInputStream::Skip
    }
    
    // 8.2 Create OstreamOutputStream to std::ostringstream
    std::ostringstream oss;
    int ostream_block_size = fdp.ConsumeIntegralInRange<int>(-1, 65536);
    OstreamOutputStream ostream_output(&oss, ostream_block_size);
    
    // Test OstreamOutputStream::BackUp (primary target)
    if (ostream_output.Next(&write_buffer, &write_size) && write_size > 0) {
        size_t ostream_write_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min(test_data.size(), (size_t)write_size));
        if (ostream_write_size > 0 && test_data.size() >= ostream_write_size) {
            memcpy(write_buffer, test_data.data(), ostream_write_size);
            
            int ostream_backup = fdp.ConsumeIntegralInRange<int>(0, ostream_write_size);
            ostream_backup = std::min(ostream_backup, (int)ostream_write_size);
            ostream_backup = std::max(ostream_backup, 0);
            ostream_output.BackUp(ostream_backup);  // Target API: OstreamOutputStream::BackUp
        }
    }
    
    // Step 9: Test CodedInputStream/CodedOutputStream wrappers for structured I/O
    if (has_read) {
        // Reset pipe for coded stream testing
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        
        int new_pipe_fds[2];
        if (pipe(new_pipe_fds) == -1) {
            if (socketpair(AF_UNIX, SOCK_STREAM, 0, new_pipe_fds) == -1) {
                close(pipe_out_fds[0]);
                close(pipe_out_fds[1]);
                return 0;
            }
        }
        
        // Write test data again
        write(new_pipe_fds[1], test_data.data(), test_data.size());
        
        FileInputStream coded_file_input(new_pipe_fds[0], 1024);
        CodedInputStream coded_input(&coded_file_input);
        
        // Test various CodedInputStream read operations
        while (!coded_input.ExpectAtEnd()) {
            uint32_t tag = coded_input.ReadTag();
            if (tag == 0) break;
            
            // Try reading different types based on tag
            switch (tag & 0x7) {
                case 0: {  // varint
                    uint64_t value;
                    coded_input.ReadVarint64(&value);
                    break;
                }
                case 1: {  // fixed64
                    uint64_t value;
                    coded_input.ReadLittleEndian64(&value);
                    break;
                }
                case 2: {  // length delimited
                    std::string str;
                    coded_input.ReadString(&str, fdp.ConsumeIntegralInRange<int>(0, 256));
                    break;
                }
                case 5: {  // fixed32
                    uint32_t value;
                    coded_input.ReadLittleEndian32(&value);
                    break;
                }
            }
            
            // Test Skip with random amount
            int coded_skip = fdp.ConsumeIntegralInRange<int>(0, 128);
            coded_input.Skip(coded_skip);
        }
        
        close(new_pipe_fds[0]);
        close(new_pipe_fds[1]);
    }
    
    // Step 10: Test Close operations (primary target APIs)
    // Note: Closing streams with file descriptors they don't own can cause issues
    // We'll test GetErrno() before and after operations
    
    // 10.1 Check error status before closing
    (void)file_input.GetErrno();
    (void)file_output.GetErrno();
    
    // 10.2 For file descriptors we don't own, we shouldn't call Close()
    // as it would close the underlying file descriptors.
    // Instead, we test error paths by checking GetErrno()
    
    // Create a FileInputStream with invalid file descriptor to test error paths
    FileInputStream error_input(-1);
    (void)error_input.GetErrno();  // Should return EBADF or similar
    
    // Try operations on invalid stream
    const void* error_buffer;
    int error_size;
    (void)error_input.Next(&error_buffer, &error_size);
    error_input.BackUp(0);  // Should be safe even on error
    (void)error_input.Skip(0);
    
    // 10.3 Test FileOutputStream::Write directly
    // Create a new pipe for Write testing
    int write_test_fds[2];
    if (pipe(write_test_fds) != -1) {
        FileOutputStream write_test_output(write_test_fds[1], 1024);
        
        // Get buffer via Next then use it (Write happens internally through CopyingOutputStreamAdaptor)
        void* test_write_buffer;
        int test_write_size;
        if (write_test_output.Next(&test_write_buffer, &test_write_size) && test_write_size > 0) {
            // Fill with some test data
            size_t fill_size = std::min(test_data.size(), (size_t)test_write_size);
            if (fill_size > 0) {
                memcpy(test_write_buffer, test_data.data(), fill_size);
                
                // BackUp some of it to test partial write
                int test_backup = fdp.ConsumeIntegralInRange<int>(0, fill_size);
                write_test_output.BackUp(test_backup);
            }
        }
        // Don't close the pipe - it's owned by the stream with default settings
    }
    
    // 10.4 Test Close operations on dedicated file descriptors
    // Create pipes specifically for Close testing
    int close_test_fds_in[2];
    int close_test_fds_out[2];
    
    if (pipe(close_test_fds_in) != -1 && pipe(close_test_fds_out) != -1) {
        // Write some data to input pipe
        write(close_test_fds_in[1], test_data.data(), std::min(test_data.size(), (size_t)128));
        
        // Create streams with close_on_delete set to true
        FileInputStream close_input(close_test_fds_in[0], 1024);
        FileOutputStream close_output(close_test_fds_out[1], 1024);
        
        // Set close on delete so streams own the file descriptors
        close_input.SetCloseOnDelete(true);
        close_output.SetCloseOnDelete(true);
        
        // Test GetErrno before closing
        (void)close_input.GetErrno();
        (void)close_output.GetErrno();
        
        // Perform some operations
        const void* close_buf;
        int close_size;
        if (close_input.Next(&close_buf, &close_size)) {
            // Read some data
            if (close_size > 0) {
                int skip_amt = fdp.ConsumeIntegralInRange<int>(0, close_size);
                close_input.Skip(skip_amt);
            }
        }
        
        // Now test Close() methods (primary targets)
        // Target API: FileInputStream::Close (which calls CopyingFileInputStream::Close)
        bool input_close_result = close_input.Close();
        (void)input_close_result;
        
        // Target API: FileOutputStream::Close (which calls CopyingFileOutputStream::Close)  
        bool output_close_result = close_output.Close();
        (void)output_close_result;
        
        // Check error status after closing
        (void)close_input.GetErrno();
        (void)close_output.GetErrno();
        
        // Note: File descriptors are now closed by the Close() calls
        // Don't call close() on them manually
    } else {
        // Clean up if pipe creation failed
        if (close_test_fds_in[0] > 0) close(close_test_fds_in[0]);
        if (close_test_fds_in[1] > 0) close(close_test_fds_in[1]);
        if (close_test_fds_out[0] > 0) close(close_test_fds_out[0]);
        if (close_test_fds_out[1] > 0) close(close_test_fds_out[1]);
    }
    
    // Step 11: Cleanup (close file descriptors we created)
    close(pipe_fds[0]);
    close(pipe_fds[1]);
    close(pipe_out_fds[0]);
    close(pipe_out_fds[1]);
    
    return 0;
}

} // namespace harness_008
