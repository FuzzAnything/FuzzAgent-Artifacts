#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <ares.h>
}
extern "C" {
#include <ares_dns.h>
}
extern "C" {
#include <ares_nameser.h>
}
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"ares_library_cleanup":[[141,7,141,24,0,0,4,0]]},"library_calls":["ares_library_initialized","ares_init","ares_destroy","ares_set_local_dev","ares_parse_soa_reply","ares_dns_rec_type_tostr","ares_queue_wait_empty","ares_free_data","ares_fds","ares_process_fd","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_initialized","ares_init","ares_set_local_dev","ares_parse_soa_reply","ares_dns_rec_type_tostr","ares_queue_wait_empty","ares_free_data","ares_fds","ares_process_fd","ares_destroy","ares_library_cleanup"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 1) {
        return 0;
    }

    // 1. Check if library is initialized
    int lib_init_status = ares_library_initialized();
    (void)lib_init_status; // Use result to avoid unused variable warning

    // 2. Initialize channel
    ares_channel_t *channel = nullptr;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS || channel == nullptr) {
        // Channel initialization failed, cleanup if needed
        if (channel != nullptr) {
            ares_destroy(channel);
        }
        return 0;
    }

    // 3. Set local device (use first byte of data as dummy device name length)
    // Create a dummy device name to satisfy the API call requirement
    char dummy_dev[2] = "0";
    if (size > 1) {
        dummy_dev[0] = (data[0] % 26) + 'a'; // Generate a letter from a-z
    }
    ares_set_local_dev(channel, fuzz_str_1);

    // 4. Parse SOA reply from input data
    struct ares_soa_reply *soa_out = nullptr;
    // Convert size to int safely (cap at INT_MAX)
    int alen = (size > INT_MAX) ? INT_MAX : static_cast<int>(size);
    int parse_result = ares_parse_soa_reply(data, alen, &soa_out);

    // 5. Convert DNS record type to string (using SOA type)
    // We'll use the SOA record type (value 6) since we're parsing SOA reply
    const char *type_str = ares_dns_rec_type_tostr(static_cast<ares_dns_rec_type_t>(6));
    (void)type_str; // Use result to avoid unused variable warning

    // 6. Wait for queue to be empty with timeout
    // Use a small timeout since we're not performing actual network operations
    int timeout_ms = 0; // Immediate return
    ares_status_t wait_status = ares_queue_wait_empty(channel, fuzz_int32_t_2);
    (void)wait_status; // Use result to avoid unused variable warning

    // 7. Cleanup allocated resources
    if (soa_out != nullptr) {
        ares_free_data(soa_out);
    }

    // Process any pending operations (optional but good practice)
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    ares_fds(channel, &read_fds, &write_fds);

    // Process with no active file descriptors
    ares_process_fd(channel, ARES_SOCKET_BAD, fuzz_int32_t_3);

    // Destroy channel
    ares_destroy(channel);

    // 8. Library cleanup (optional - fuzzer may call this multiple times)
    // Note: In a real fuzzing environment, we might want to call this
    // only once at the end of all fuzzing iterations
    ares_library_cleanup();

    return 0;
}