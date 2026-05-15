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
#include <pcap/bluetooth.h>
}
extern "C" {
#include <pcap/can_socketcan.h>
}
extern "C" {
#include <pcap/compiler-tests.h>
}
extern "C" {
#include <pcap/dlt.h>
}
extern "C" {
#include <pcap/funcattrs.h>
}
extern "C" {
#include <pcap/ipnet.h>
}
extern "C" {
#include <pcap/nflog.h>
}
extern "C" {
#include <pcap/pcap-inttypes.h>
}
extern "C" {
#include <pcap/pcap.h>
}
extern "C" {
#include <pcap/sll.h>
}
extern "C" {
#include <pcap/socket.h>
}
extern "C" {
#include <pcap/usb.h>
}
extern "C" {
#include <pcap/vlan.h>
}
//<ID> 376
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 8
//<Quality> {"density":13,"unique_branches":{"pcap_datalink_name_to_val":[[3419,14,3419,41,0,0,4,0],[3420,7,3420,57,0,0,4,0],[3420,7,3420,57,0,0,4,1]],"pcap_set_snaplen":[[2599,6,2599,32,0,0,4,0]],"pcapint_strcasecmp":[[3208,9,3208,41,0,0,4,0],[3208,9,3208,41,0,0,4,1],[3209,7,3209,21,0,0,4,0],[3209,7,3209,21,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_snapshot","pcap_set_snaplen","pcap_can_set_rfmon","pcap_list_tstamp_types","pcap_set_tstamp_type","pcap_free_tstamp_types","pcap_datalink_name_to_val","bpf_validate","pcap_dump_fopen","pcap_close","pcap_dump_file","pcap_dispatch","pcap_freealldevs","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_snapshot","pcap_set_snaplen","pcap_can_set_rfmon","pcap_list_tstamp_types","pcap_set_tstamp_type","pcap_free_tstamp_types","pcap_datalink_name_to_val","bpf_validate","pcap_dump_fopen","pcap_dump_file","pcap_dispatch","pcap_freealldevs","pcap_dump_close","pcap_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpcap APIs to achieve the event of reading, processing, and writing packet capture data:

*/


// Include libpcap headers

// Custom handler for processing packets
static void  id_000006_packet_handler(u_char *user, const struct pcap_pkthdr *hdr, const u_char *data) {
    pcap_dumper_t *dumper = (pcap_dumper_t *)user;
    if (dumper != nullptr) {
        pcap_dump(user, hdr, data);
    }
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for writing processed packets
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Open pcap file for offline reading
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Get snapshot length
    int snapshot_len = pcap_snapshot(pcap_handle);
    
    // 2. Set snapshot length (even if same as current)
    pcap_set_snaplen(pcap_handle, snapshot_len);

    // 3. Check if can set rfmon (monitor mode) - though for offline file this will likely fail
    pcap_can_set_rfmon(pcap_handle);

    // 4. Set timestamp type (use first valid type if possible)
    int *tstamp_types = nullptr;
    int tstamp_type_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
    if (tstamp_type_count > 0) {
        // Try to set the first available timestamp type
        pcap_set_tstamp_type(pcap_handle, tstamp_types[0]);
        pcap_free_tstamp_types(tstamp_types);
    }

    // 5. Convert data link name to value (using a common type)
    const char *linktype_name = "EN10MB";
    pcap_datalink_name_to_val(fuzz_str_1);

    // 6. Create simple BPF filter for validation
    struct bpf_insn filter_insns[] = {
        { 0x28, 0, 0, 0x0000000c },
        { 0x15, 0, 1, 0x00000800 },
        { 0x6, 0, 0, 0x00000000 },
        { 0x6, 0, 0, 0xffffffff }
    };
    
    // 7. Validate BPF filter
    bpf_validate(filter_insns, sizeof(filter_insns)/sizeof(filter_insns[0]));

    // 8. Create dumper for output
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap_handle, out_file);
    if (!dumper) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 9. Get dump file (API usage)
    FILE *dump_file = pcap_dump_file(dumper);
    (void)dump_file; // Use to avoid unused variable warning

    // 10. Process packets using dispatch
    int packet_count = pcap_dispatch(pcap_handle, fuzz_int32_t_2,  id_000006_packet_handler, (u_char *)dumper);

    // 11. Free device list (even if not used in this context)
    pcap_freealldevs(nullptr);

    // Cleanup
    pcap_dump_close(dumper);
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}