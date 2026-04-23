/**
 * @file     Chapter_09_test.cpp
 * @author   KaKaRot
 * @date     23/4/2026
 * @brief    test program for Chapter 09 exercises
 * @details  test program for Chapter 09 exercises
 */

 #include "Person.h"
 #include "Chrono.h"

 int main()
 {
     Person p1, p2;
     std::cout << "Enter details for person 1:\n";
     p1.read_names();
     p1.read_ages();

     std::cout << "Enter details for person 2:\n";
     p2.read_names();
     p2.read_ages();

     std::cout << "\nPerson 1:\n" << p1;
     std::cout << "Person 2:\n" << p2;

     if (p1 == p2) {
         std::cout << "The two persons are the same.\n";
     } else {
         std::cout << "The two persons are different.\n";
     }

     return 0;
 }