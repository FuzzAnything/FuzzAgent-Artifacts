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
//<ID> 437
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"pcap_statustostr":[[3721,10,3721,16,0,0,4,0],[3723,2,3723,19,0,0,4,0]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_list_tstamp_types","pcap_statustostr","pcap_free_tstamp_types","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_dump_fopen","pcap_close","pcap_next_ex","pcap_dump","pcap_statustostr","pcap_statustostr","pcap_statustostr","pcap_statustostr","pcap_dump_flush","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_list_tstamp_types","pcap_statustostr","pcap_free_tstamp_types","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_dump_fopen","pcap_next_ex","pcap_dump","pcap_statustostr","pcap_statustostr","pcap_dump_flush","pcap_dump_close","pcap_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Open input data as a FILE* for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Open pcap file with timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap = pcap_fopen_offline_with_tstamp_precision(in_file, fuzz_uint32_t_1, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // List available timestamp types
    int *tstamp_types = nullptr;
    int tstamp_count = pcap_list_tstamp_types(pcap, &tstamp_types);
    if (tstamp_count > 0 && tstamp_types) {
        // Use first timestamp type if available
        for (int i = 0; i < tstamp_count; i++) {
            pcap_statustostr(tstamp_types[i]); // Convert status to string
        }
        pcap_free_tstamp_types(tstamp_types);
    }

    // List available data link types
    int *dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap, &dlt_list);
    if (dlt_count > 0 && dlt_list) {
        // Try to set data link type to first available
        pcap_set_datalink(pcap, dlt_list[0]);
        pcap_free_datalinks(dlt_list);
    }

    // Open dumper for output
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap, out_file);
    if (!dumper) {
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Process packets from input
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet = nullptr;
    int status = 0;

    while ((status = pcap_next_ex(pcap, &header, &packet)) == 1) {
        // Dump packet to output
        pcap_dump((u_char *)dumper, header, packet);
        
        // Convert status to string (though status=1 here)
        pcap_statustostr(fuzz_int32_t_2);
    }

    // Handle end of file or errors
    if (status == 0) {
        pcap_statustostr(fuzz_int32_t_3); // No more packets
    } else if (status == -1) {
        pcap_statustostr(fuzz_int32_t_4); // Error
    } else if (status == -2) {
        pcap_statustostr(fuzz_int32_t_5); // Break loop
    }

    // Flush dumper
    pcap_dump_flush(dumper);

    // Cleanup
    pcap_dump_close(dumper);
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}