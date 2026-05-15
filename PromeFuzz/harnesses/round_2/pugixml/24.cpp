// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
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

static pugi::xml_document create_test_document() {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    pugi::xml_node child1 = root.append_child("child1");
    child1.append_attribute("attr1") = "value1";
    child1.append_attribute("attr2") = "value2";
    
    pugi::xml_node child2 = root.append_child("child2");
    child2.append_attribute("attr3") = "value3";
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    pugi::xml_document doc = create_test_document();
    pugi::xml_node root = doc.root();
    
    try {
        // 1. pugi::xml_node::child
        size_t child_name_len = std::min(Size, static_cast<size_t>(100));
        std::string child_name(reinterpret_cast<const char*>(Data), child_name_len);
        if (!child_name.empty()) {
            child_name.push_back('\0');
            pugi::xml_node child_node = root.child(child_name.c_str());
            
            // 2. pugi::xml_node::append_attribute
            size_t attr_name1_len = std::min(Size, static_cast<size_t>(50));
            std::string attr_name1(reinterpret_cast<const char*>(Data), attr_name1_len);
            if (!attr_name1.empty()) {
                attr_name1.push_back('\0');
                pugi::xml_attribute attr1 = child_node.append_attribute(attr_name1.c_str());
                
                // 3. pugi::xml_attribute::set_value (all 3 overloads)
                if (attr1) {
                    size_t value_str_len = std::min(Size, static_cast<size_t>(200));
                    std::string value_str(reinterpret_cast<const char*>(Data), value_str_len);
                    if (!value_str.empty()) {
                        value_str.push_back('\0');
                        attr1.set_value(value_str.c_str());
                    }
                    
                    int int_val = static_cast<int>(Data[0]);
                    attr1.set_value(int_val);
                    
                    unsigned int uint_val = static_cast<unsigned int>(Data[0]);
                    attr1.set_value(uint_val);
                }
                
                // 4. pugi::xml_node::append_attribute
                if (Size > 10) {
                    size_t attr_name2_len = std::min(Size - 10, static_cast<size_t>(50));
                    std::string attr_name2(reinterpret_cast<const char*>(Data) + 10, attr_name2_len);
                    if (!attr_name2.empty()) {
                        attr_name2.push_back('\0');
                        pugi::xml_attribute attr2 = child_node.append_attribute(attr_name2.c_str());
                    }
                }
                
                // 5. pugi::xml_node::append_attribute
                if (Size > 20) {
                    size_t attr_name3_len = std::min(Size - 20, static_cast<size_t>(50));
                    std::string attr_name3(reinterpret_cast<const char*>(Data) + 20, attr_name3_len);
                    if (!attr_name3.empty()) {
                        attr_name3.push_back('\0');
                        pugi::xml_attribute attr3 = child_node.append_attribute(attr_name3.c_str());
                        
                        // 6. pugi::xml_attribute::set_value (all 3 overloads)
                        if (attr3) {
                            if (Size > 30) {
                                size_t value_str2_len = std::min(Size - 30, static_cast<size_t>(200));
                                std::string value_str2(reinterpret_cast<const char*>(Data) + 30, value_str2_len);
                                if (!value_str2.empty()) {
                                    value_str2.push_back('\0');
                                    attr3.set_value(value_str2.c_str());
                                }
                            }
                            
                            if (Size > 1) {
                                int int_val2 = static_cast<int>(Data[1]);
                                attr3.set_value(int_val2);
                                
                                unsigned int uint_val2 = static_cast<unsigned int>(Data[1]);
                                attr3.set_value(uint_val2);
                            }
                        }
                    }
                }
                
                // 7. pugi::xml_node::append_attribute
                if (Size > 40) {
                    size_t attr_name4_len = std::min(Size - 40, static_cast<size_t>(50));
                    std::string attr_name4(reinterpret_cast<const char*>(Data) + 40, attr_name4_len);
                    if (!attr_name4.empty()) {
                        attr_name4.push_back('\0');
                        pugi::xml_attribute attr4 = child_node.append_attribute(attr_name4.c_str());
                    }
                }
                
                // 8. pugi::xml_node::append_attribute
                if (Size > 50) {
                    size_t attr_name5_len = std::min(Size - 50, static_cast<size_t>(50));
                    std::string attr_name5(reinterpret_cast<const char*>(Data) + 50, attr_name5_len);
                    if (!attr_name5.empty()) {
                        attr_name5.push_back('\0');
                        pugi::xml_attribute attr5 = child_node.append_attribute(attr_name5.c_str());
                        
                        // 9. pugi::xml_attribute::set_value (all 3 overloads)
                        if (attr5) {
                            if (Size > 60) {
                                size_t value_str3_len = std::min(Size - 60, static_cast<size_t>(200));
                                std::string value_str3(reinterpret_cast<const char*>(Data) + 60, value_str3_len);
                                if (!value_str3.empty()) {
                                    value_str3.push_back('\0');
                                    attr5.set_value(value_str3.c_str());
                                }
                            }
                            
                            if (Size > 2) {
                                int int_val3 = static_cast<int>(Data[2]);
                                attr5.set_value(int_val3);
                                
                                unsigned int uint_val3 = static_cast<unsigned int>(Data[2]);
                                attr5.set_value(uint_val3);
                            }
                        }
                    }
                }
            }
        }
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}