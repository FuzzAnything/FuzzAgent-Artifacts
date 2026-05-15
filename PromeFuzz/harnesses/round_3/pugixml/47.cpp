// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

static std::string create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len = 100) {
    size_t len = size < max_len ? size : max_len;
    std::string result(reinterpret_cast<const char*>(data), len);
    
    // Ensure null termination
    if (!result.empty() && result.back() != '\0') {
        result.push_back('\0');
    } else if (result.empty()) {
        result = "\0";
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* Data, size_t Size) {
    // Create a dummy XML file with fuzzer data
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return 0;
    
    // Write some XML structure with fuzzer data
    std::string xml_start = "<?xml version=\"1.0\"?><root>";
    std::string xml_content = create_null_terminated_string(Data, Size, 500);
    std::string xml_end = "</root>";
    
    file << xml_start;
    // Add some child nodes for testing
    file << "<child1>value1</child1>";
    file << "<child2>value2</child2>";
    file << "<child3>value3</child3>";
    file << "<node>";
    file.write(reinterpret_cast<const char*>(Data), Size > 1000 ? 1000 : Size);
    file << "</node>";
    file << xml_end;
    file.close();
    
    // Parse the XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (!result) {
        // If parsing fails, create a minimal document for testing
        doc.append_child("root");
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }
    
    // Create some child nodes for testing
    pugi::xml_node child1 = root.append_child("child1");
    child1.append_child(pugi::node_pcdata).set_value("test1");
    
    pugi::xml_node child2 = root.append_child("child2");
    child2.append_child(pugi::node_pcdata).set_value("test2");
    
    pugi::xml_node child3 = root.append_child("child3");
    child3.append_child(pugi::node_pcdata).set_value("test3");
    
    // Create string parameters from fuzzer data
    std::string str1 = create_null_terminated_string(Data, Size, 50);
    std::string str2 = create_null_terminated_string(Data + (Size > 50 ? 50 : 0), 
                                                     Size > 50 ? Size - 50 : 0, 50);
    std::string str3 = create_null_terminated_string(Data + (Size > 100 ? 100 : 0), 
                                                     Size > 100 ? Size - 100 : 0, 50);
    
    // Execute the sequence of API calls
    try {
        // 1. pugi::xml_node::child
        pugi::xml_node node1 = root.child(str1.c_str());
        
        // 2. pugi::xml_node::child
        pugi::xml_node node2 = root.child(str2.c_str());
        
        // 3. pugi::xml_node::insert_copy_after
        pugi::xml_node node3 = root.insert_copy_after(child1, node1);
        
        // 4. pugi::xml_node::child
        pugi::xml_node node4 = root.child(str3.c_str());
        
        // 5. pugi::xml_node::insert_copy_after
        pugi::xml_node node5 = root.insert_copy_after(child2, node2);
        
        // 6. pugi::xml_node::child
        pugi::xml_node node6 = root.child("child1");
        
        // 7. pugi::xml_node::child
        pugi::xml_node node7 = root.child("child2");
        
        // 8. pugi::xml_node::child
        pugi::xml_node node8 = root.child("child3");
        
        // 9. pugi::xml_node::child
        pugi::xml_node node9 = root.child(str1.c_str());
        
        // 10. pugi::xml_node::insert_copy_after
        pugi::xml_node node10 = root.insert_copy_after(child3, node3);
        
        // 11. pugi::xml_node::child
        pugi::xml_node node11 = node1.child(str2.c_str());
        
        // 12. pugi::xml_node::first_child
        pugi::xml_node node12 = root.first_child();
        
        // 13. pugi::xml_node::name
        if (node12) {
            const char* name = node12.name();
            (void)name; // Avoid unused variable warning
        }
        
        // 14. pugi::xml_node::child
        pugi::xml_node node14 = node12.child(str3.c_str());
        
        // 15. pugi::xml_node::insert_copy_after
        pugi::xml_node node15 = root.insert_copy_after(node12, node4);
        
        // 16. pugi::xml_node::child
        pugi::xml_node node16 = node15.child(str1.c_str());
        
        // 17. pugi::xml_node::first_child
        pugi::xml_node node17 = node16.first_child();
        
        // 18. pugi::xml_node::child
        pugi::xml_node node18 = node17.child(str2.c_str());
        
        // 19. pugi::xml_node::last_child
        pugi::xml_node node19 = root.last_child();
        
        // 20. pugi::xml_node::value
        if (node19) {
            const char* value = node19.value();
            (void)value; // Avoid unused variable warning
        }
        
        // 21. pugi::xml_node::child
        pugi::xml_node node21 = node19.child(str3.c_str());
        
        // 22. pugi::xml_node::insert_copy_after
        pugi::xml_node node22 = root.insert_copy_after(node19, node5);
        
        // 23. pugi::xml_node::child
        pugi::xml_node node23 = node22.child(str1.c_str());
        
        // 24. pugi::xml_node::first_child
        pugi::xml_node node24 = node23.first_child();
        
        // 25. pugi::xml_node::child
        pugi::xml_node node25 = node24.child(str2.c_str());
        
        // 26. pugi::xml_node::first_child
        pugi::xml_node node26 = node25.first_child();
        
        // 27. pugi::xml_node::value
        if (node26) {
            const char* value = node26.value();
            (void)value; // Avoid unused variable warning
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Clean up
    std::remove("./dummy_file");
    
    return 0;
}