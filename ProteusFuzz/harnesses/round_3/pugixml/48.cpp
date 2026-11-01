// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string_view>

static pugi::xml_document create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.append_child("root");
        return doc;
    }
    
    // Try to parse as XML
    if (doc.load_buffer(data, size)) {
        return doc;
    }
    
    // If parsing fails, create a minimal document to ensure valid state
    doc.append_child("root");
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc = create_document_from_fuzz_data(Data, Size);
    
    // Ensure we have at least a root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }
    
    // Add some initial children to work with
    if (!root.child("child1")) {
        root.append_child("child1");
    }
    if (!root.child("child2")) {
        root.append_child("child2");
    }
    
    // Step 1: pugi::xml_node::child
    pugi::xml_node node1 = root.child("child1");
    
    // Step 2: pugi::xml_node::child
    pugi::xml_node node2 = node1.child("nested");
    if (!node2) {
        node2 = node1.append_child("nested");
    }
    
    // Step 3: pugi::xml_node::insert_copy_after
    pugi::xml_node proto1 = node2;
    pugi::xml_node after1 = node1;
    pugi::xml_node inserted1 = node1.insert_copy_after(proto1, after1);
    
    // Step 4: pugi::xml_node::child
    pugi::xml_node node3 = inserted1.child("nested");
    
    // Step 5: pugi::xml_node::insert_copy_after
    pugi::xml_node proto2 = node3;
    pugi::xml_node after2 = inserted1;
    pugi::xml_node inserted2 = inserted1.insert_copy_after(proto2, after2);
    
    // Step 6: pugi::xml_node::child
    pugi::xml_node node4 = inserted2.child("nested");
    
    // Step 7: pugi::xml_node::child
    pugi::xml_node node5 = node4.child("deep");
    if (!node5) {
        node5 = node4.append_child("deep");
    }
    
    // Step 8: pugi::xml_node::child
    pugi::xml_node node6 = node5.child("deeper");
    
    // Step 9: pugi::xml_node::child
    pugi::xml_node node7 = node6.child("deepest");
    
    // Step 10: pugi::xml_node::insert_copy_after
    pugi::xml_node proto3 = node7;
    pugi::xml_node after3 = node6;
    pugi::xml_node inserted3 = node6.insert_copy_after(proto3, after3);
    
    // Step 11: pugi::xml_node::child
    pugi::xml_node node8 = inserted3.child("deepest");
    
    // Step 12: pugi::xml_node::first_child
    pugi::xml_node node9 = node8.first_child();
    
    // Step 13: pugi::xml_node::name
    const pugi::char_t* name1 = node9.name();
    (void)name1; // Suppress unused warning
    
    // Step 14: pugi::xml_node::child
    pugi::xml_node node10 = node9.child("test");
    
    // Step 15: pugi::xml_node::insert_copy_after
    pugi::xml_node proto4 = node10;
    pugi::xml_node after4 = node9;
    pugi::xml_node inserted4 = node9.insert_copy_after(proto4, after4);
    
    // Step 16: pugi::xml_node::child
    pugi::xml_node node11 = inserted4.child("test");
    
    // Step 17: pugi::xml_node::first_child
    pugi::xml_node node12 = node11.first_child();
    
    // Step 18: pugi::xml_node::child
    pugi::xml_node node13 = node12.child("item");
    
    // Step 19: pugi::xml_node::last_child
    pugi::xml_node node14 = node12.last_child();
    
    // Step 20: pugi::xml_node::value
    const pugi::char_t* value1 = node14.value();
    (void)value1; // Suppress unused warning
    
    // Step 21: pugi::xml_node::child
    pugi::xml_node node15 = node14.child("data");
    
    // Step 22: pugi::xml_node::insert_copy_after
    pugi::xml_node proto5 = node15;
    pugi::xml_node after5 = node14;
    pugi::xml_node inserted5 = node14.insert_copy_after(proto5, after5);
    
    // Step 23: pugi::xml_node::child
    pugi::xml_node node16 = inserted5.child("data");
    
    // Step 24: pugi::xml_node::first_child
    pugi::xml_node node17 = node16.first_child();
    
    // Step 25: pugi::xml_node::child
    pugi::xml_node node18 = node17.child("value");
    
    // Step 26: pugi::xml_node::first_child
    pugi::xml_node node19 = node18.first_child();
    
    // Step 27: pugi::xml_node::value
    const pugi::char_t* value2 = node19.value();
    (void)value2; // Suppress unused warning
    
    return 0;
}