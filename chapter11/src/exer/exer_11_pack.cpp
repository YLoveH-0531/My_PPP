/**
 * @file exex_11_pack.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-22
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include <fstream>
#include <cstddef>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif 

const std::string DATA = DATA_DIR;


struct Packet {
    char   type;    
    int    length;  
    char   flag;    
    double value;
};

#pragma pack(push, 1)
struct Packet_Packed {
    char   type;    
    int    length;  
    char   flag;    
    double value;
};
#pragma pack(pop)

int main()
{
    std::cout << "Size of Packet: " << sizeof(Packet) << " bytes" << std::endl;
    std::cout << "Size of Packet_Packed: " << sizeof(Packet_Packed) << " bytes" << std::endl;

    Packet pkt {'A', 100, 'X', 3.14};
    Packet_Packed pkt_packed {'B', 200, 'Y', 6.28};

    //layout
    std::cout << "Packet memory layout:" << std::endl;
    std::cout << "type " << offsetof(Packet, type) << std::endl;
    std::cout << "length " << offsetof(Packet, length) << std::endl;
    std::cout << "flag " << offsetof(Packet, flag) << std::endl;
    std::cout << "value " << offsetof(Packet, value) << std::endl;

    std::cout << "Packet_Packed memory layout:" << std::endl;
    std::cout << "type " << offsetof(Packet_Packed, type) << std::endl;
    std::cout << "length " << offsetof(Packet_Packed, length) << std::endl;
    std::cout << "flag " << offsetof(Packet_Packed, flag) << std::endl;
    std::cout << "value " << offsetof(Packet_Packed, value) << std::endl;

    std::ofstream outFile(DATA + "packet_data.txt");
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1;
    }

    outFile << "Packet Data:" << std::endl;
    outFile << "Type: " << pkt.type << std::endl;
    outFile << "Length: " << pkt.length << std::endl;
    outFile << "Flag: " << pkt.flag << std::endl;
    outFile << "Value: " << pkt.value << std::endl;
    outFile.close();

    outFile.open(DATA + "packet_packed_data.txt");
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1;
    }

    outFile << "Packet_Packed Data:" << std::endl;
    outFile << "Type: " << pkt_packed.type << std::endl;
    outFile << "Length: " << pkt_packed.length << std::endl;
    outFile << "Flag: " << pkt_packed.flag << std::endl;
    outFile << "Value: " << pkt_packed.value << std::endl;
    outFile.close();

    std::ifstream inFile(DATA + "packet_data.txt");
    if (!inFile) {
        std::cerr << "Error opening file for reading." << std::endl;
        return 1;
    }

    std::string line;
    std::cout << "Reading Packet data from file:" << std::endl;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
    inFile.close();

    inFile.open(DATA + "packet_packed_data.txt");
    if (!inFile) {
        std::cerr << "Error opening file for reading." << std::endl;
        return 1;
    }

    std::cout << "Reading Packet_Packed data from file:" << std::endl;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }

    return 0;
}
