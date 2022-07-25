#include <iostream>
#include <stdexcept>

#include "memtrace.h"
#include "gtest_lite.h"
#include "suffix_array.h"

int main() {
    TEST(Alap, konstruktor) {
        SuffixArray sa("Szoveg Szoveg");
        EXPECT_STREQ("Szoveg Szoveg", sa.c_str()) << "Hiba a szoveg masolasakor.";
    } END;

    TEST(Alap, masolo_konstruktor) {
        SuffixArray sa("Szoveg Szoveg");
        SuffixArray masolat(sa);
        EXPECT_STREQ(sa.c_str(), masolat.c_str()) << "Hiba a masolo konstruktorban.";
    } END;

    TEST(Alap, ertekado_operator1) {
        SuffixArray sa1("Szoveg Szoveg");
        SuffixArray sa2("");
        sa2 = sa1;
        EXPECT_STREQ("Szoveg Szoveg", sa2.c_str()) << "Hiba az ertekado operatorban.";
    } END;

    TEST(Alap, ertekado_operator2) {
        SuffixArray sa1("Szoveg Szoveg");
        sa1 = sa1;
        EXPECT_STREQ("Szoveg Szoveg", sa1.c_str()) << "Hiba az ertekado operatorban.";
    } END;

    TEST(Alap, hozzaado_operator) {
        SuffixArray sa1("Szoveg Szoveg");
        SuffixArray sa2(" Szoveg");
        sa1 += sa2;
        EXPECT_STREQ("Szoveg Szoveg Szoveg", sa1.c_str()) << "Hiba a hozzaado operatorban.";
    } END;

    TEST(Alap, osszeado_operator) {
        SuffixArray sa1("Szoveg Szoveg");
        SuffixArray sa2(" Szoveg");
        EXPECT_STREQ("Szoveg Szoveg Szoveg", (sa1 + sa2).c_str()) << "Hiba a osszeado operatorban.";
    } END;

    TEST(Iterator, kezdetu_szovegek1) {
        SuffixArray sa("Szoveg Szoveg");
        SuffixArray::iterator it = sa.keres(std::string("oveg"));
        EXPECT_STREQ(it->c_str(), "oveg") << "Hibas talalat a keresesben.";
        EXPECT_NO_THROW(it++);
        EXPECT_NO_THROW(*it);
        EXPECT_STREQ("oveg Szoveg", it->c_str()) << "Hibas talalat a keresesben.";
    } END;

    TEST(Iterator, kezdetu_szovegek2) {
        SuffixArray sa("Szoveg Szoveg");
        SuffixArray::iterator it = sa.keres("oveg");
        EXPECT_STREQ(it->c_str(), "oveg") << "Hibas talalat a keresesben.";
        EXPECT_NO_THROW(it++);
        EXPECT_NO_THROW(*it);
        EXPECT_STREQ("oveg Szoveg", it->c_str()) << "Hibas talalat a keresesben.";
    } END;

    TEST(Iterator, kezdetu_szovegek3) {
        SuffixArray sa("asdasdasd");
        SuffixArray::iterator it = sa.keres('d');
        EXPECT_STREQ("d", it->c_str()) << "Hibas talalat a keresesben.";
        EXPECT_NO_THROW(++it);
        EXPECT_STREQ("dasd", it->c_str()) << "Hibas talalat a keresesben.";
        EXPECT_NO_THROW(++it);
        EXPECT_STREQ("dasdasd", it->c_str()) << "Hibas talalat a keresesben.";
        EXPECT_NO_THROW(++it);
        EXPECT_FALSE(it != sa.end()) << "Tul sok talalat.";
    } END;

    TEST(Iterator, ures_stringben_kereses) {
        SuffixArray sa("");
        SuffixArray::iterator it = sa.keres("Hello");
        EXPECT_FALSE(it != sa.end()) << "Hibas iterator talalat.";
    } END;

    TEST(Iterator, iteralas_vege) {
        SuffixArray sa("Szoveg Szoveg");
        SuffixArray::iterator it = sa.keres("oveg");
        EXPECT_TRUE(it++ != sa.end());
        EXPECT_TRUE(it++ != sa.end());
        EXPECT_FALSE(it != sa.end());
    } END;

    TEST(Iterator, tulindexeles) {
        SuffixArray sa("Szoveg Szoveg");
        SuffixArray::iterator it = sa.keres("oveg");
        EXPECT_NO_THROW(++it);
        EXPECT_NO_THROW(++it);
        EXPECT_THROW(++it, std::out_of_range&);
    } END;

    TEST(Iterator, nullptr_dereferalas1) {
        SuffixArray sa("Szoveg Szoveg");
        SuffixArray::iterator it = sa.keres("oveg");
        EXPECT_NO_THROW(++it);
        EXPECT_NO_THROW(++it);
        EXPECT_THROW(*it, std::out_of_range&);
    } END;

    TEST(Iterator, nullptr_dereferalas2) {
        SuffixArray sa("Szoveg Szoveg");
        SuffixArray::iterator it = sa.keres("oveg");
        EXPECT_NO_THROW(++it);
        EXPECT_NO_THROW(++it);
        EXPECT_THROW(it->c_str(), std::out_of_range&);
    } END;

    TEST(Iterator, leghosszabb_kozos_szoveg1) {
        SuffixArray sa1("Hello World!");
        SuffixArray sa2("xyzHelloxyzWorld!xzy");
        std::string leghosszabb_kozos_str = sa1 & sa2;
        EXPECT_STREQ("World!", leghosszabb_kozos_str.c_str()) << "Hiba a leghosszabb kozos string meghatarozasakor.";
    } END;

    TEST(Iterator, leghosszabb_kozos_szoveg2) {
        SuffixArray sa1("asd");
        SuffixArray sa2("xyz");
        std::string leghosszabb_kozos_str = sa1 & sa2;
        EXPECT_STREQ("", leghosszabb_kozos_str.c_str()) << "Hiba a leghosszabb kozos string meghatarozasakor.";
    } END;

    TEST(Iterator, leghosszabb_kozos_szoveg3) {
        SuffixArray sa1("asdfghjklmnopqrst");
        SuffixArray sa2("sdflmnopdasdf");
        std::string leghosszabb_kozos_str = sa1 & sa2;
        EXPECT_STREQ("lmnop", leghosszabb_kozos_str.c_str()) << "Hiba a leghosszabb kozos string meghatarozasakor.";
    } END;

    try {
        std::cout << std::endl;
        std::string szoveg;
        std::cout << "Adj meg egy szoveget!" << std::endl;
        std::getline(std::cin, szoveg);
        SuffixArray sa(szoveg);
        std::cout << "Add meg a benne keresendo karaktersorozatot!" << std::endl;
        std::getline(std::cin, szoveg);
        std::cout << "A talalatok a kovetkezok:" << std::endl;
        SuffixArray::iterator it = sa.keres(szoveg);
        if (it == sa.end())
            std::cout << "Nem talalhato egyezes a szovegben.";
        else
            for (; it != sa.end(); ++it)
                std::cout << it->c_str() << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "Hiba tortent a program futasakozben." << std::endl;
        std::cout << "A hiba oka: " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cout << "Ismeretlen hiba tortent a program futasa kozben." << std::endl;
        return -1;
    }

    try {
        std::cout << std::endl;
        std::string szoveg;
        std::cout << "Adj meg egy szoveget!" << std::endl;
        std::getline(std::cin, szoveg);
        SuffixArray sa1(szoveg);
        std::cout << "Adj megy egy masik szoveget!" << std::endl;
        std::getline(std::cin, szoveg);
        SuffixArray sa2(szoveg);
        std::string leghosszabb_kozos_str = sa1 & sa2;
        
        if (leghosszabb_kozos_str.empty())
            std::cout << "Nem talalhato egyezes a ket szovegben.";
        else
            std::cout << "A leghosszabb kozos karaktersorozat: " << leghosszabb_kozos_str << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "Hiba tortent a program futasakozben." << std::endl;
        std::cout << "A hiba oka: " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cout << "Ismeretlen hiba tortent a program futasa kozben." << std::endl;
        return -1;
    }

    return 0;
}