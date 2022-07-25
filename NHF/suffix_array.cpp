#include <string>
#include <stdexcept>
#include <algorithm>

#include "suffix_array.h"

/**
 * N�vekv� sorba rakja a suffix t�mb tartalm�t a < oper�torral.
 */
void SuffixArray::rendez() {
    std::sort(suffix, suffix + szoveg.length());
}

/**
 * A sz�vegb�l felt�lti a suffix t�mb�t.
 * Elmenti a sz�veget a suffix t�mbben, majd elhagyja az els� bet�t �s ezt ism�tli, amig a sz�veg el nem fogy (a sz�veg nem v�ltozik).
 * A suffix t�mbnek el�re le kell foglalni szoveg.length() helyet.
 */
void SuffixArray::suffixGeneralas() {
    for (size_t i = 0; i < szoveg.length(); ++i)
        suffix[i] = szoveg.substr(i);
}

/**
 * Iter�tor konstruktor.
 * 
 * @param f Az els� elem pointere ami megfelel a kriteriumoknak
 * @param l Az utolso elem pointere ami megfelel a kriteriumoknak
 */
SuffixArray::iterator::iterator(std::string* f, std::string* l) : first(f), last(l) {}

/**
 * Pre inkremens oper�tor.
 * Hib�t dob, ha az end()-en t�l pr�b�lj�k n�velni.
 * 
 * @return A megn�velt iter�tor
 */
SuffixArray::iterator& SuffixArray::iterator::operator++() {
    if (first == nullptr)
        throw std::out_of_range("Tulindexeles");
    if (first == last)
        first = last = nullptr;
    if (first != nullptr)
        first++;
    return *this;
}

/**
 * Post inkremens oper�tor.
 * Hib�t dob, ha az end()-en t�l pr�b�lj�k n�velni.
 * 
 * @return Az iter�tor a n�vel�s el�tt
 */
SuffixArray::iterator SuffixArray::iterator::operator++(int) {
    iterator tmp(*this);
    operator++();
    return tmp;
}

/**
 * K�t iter�tor egyenl�s�g�t vizsg�lja
 *
 * @return Egyenl�-e a k�t iter�tor
 */
bool SuffixArray::iterator::operator==(const iterator& rhs) const {
    return first == rhs.first && last == rhs.last;
}

/**
 * K�t iter�tor nem egyenl�s�g�t vizsg�lja
 *
 * @return Nem egyenl�-e a k�t iter�tor
 */
bool SuffixArray::iterator::operator!=(const iterator& rhs) const {
    return !(*this == rhs);
}

/**
 * Derefer�l� oper�tor.
 * Vissza adja az iter�tor �ltal mutatott string-et.
 * Hib�t dob, ha az iter�tor egyenl� az end()-el.
 *
 * @return Az iter�tor �ltal mutatott string
 */
const std::string& SuffixArray::iterator::operator*() const {
    if (first == nullptr)
        throw std::out_of_range("Tulindexeles");
    return *first;
}

/**
 * Ny�l oper�tor.
 * Vissza adja az iter�tor �ltal mutatott string pointert.
 * Hib�t dob, ha az iter�tor egyenl� az end()-el.
 * 
 * @return A mutatott string pointerje
 */
const std::string* SuffixArray::iterator::operator->() const {
    if (first == nullptr)
        throw std::out_of_range("Tulindexeles");
    return first;
}

/**
 * C st�lus� stringb�l konstruktor.
 */
SuffixArray::SuffixArray(const char* str) : szoveg(str), suffix(new std::string[szoveg.length()]) {
    suffixGeneralas();
    rendez();
}

/**
 * Stringb�l konstruktor.
 */
SuffixArray::SuffixArray(const std::string& str) : szoveg(str), suffix(new std::string[szoveg.length()]) {
    suffixGeneralas();
    rendez();
}

/**
 * M�sol� konstruktor.
 */
SuffixArray::SuffixArray(const SuffixArray& rhs) : szoveg(rhs.szoveg), suffix(new std::string[szoveg.length()]) {
    std::copy(rhs.suffix, rhs.suffix + szoveg.length(), suffix);
    rendez();
}

/**
 * Vissza adja a SuffixArray sz�veg�t.
 * 
 * @return A SuffixArray sz�vege
 */
const char* SuffixArray::c_str() const {
    return szoveg.c_str();
}

/**
 * Megkeresi az �sszes karaktersorozatot a SuffixArray-ben, ami a kapott karakterrel kezd�dik.
 * Ha nincsen megfelel� talalat akkor az iter�tor egyenl� lesz az end() �rt�kkel.
 *
 * @param str A SuffixArrayben tartalmazand� karaktersorozat
 * @return Egy iter�tor a tal�latokkal
 */
SuffixArray::iterator SuffixArray::keres(const char c) const {
    return keres(std::string(1, c));
}

/**
 * Megkeresi az �sszes karaktersorozatot a SuffixArray-ben, ami a kapott karakterekkel kezd�dik.
 * Ha nincsen megfelel� talalat akkor az iter�tor egyenl� lesz az end() �rt�kkel.
 *
 * @param str A SuffixArrayben tartalmazand� karaktersorozat
 * @return Egy iter�tor a tal�latokkal
 */
SuffixArray::iterator SuffixArray::keres(const char* str) const {
    return keres(std::string(str));
}

/**
 * Megkeresi az �sszes karaktersorozatot a SuffixArray-ben, ami a kapott karakterekkel kezd�dik.
 * Ha nincsen megfelel� talalat akkor az iter�tor egyenl� lesz az end() �rt�kkel.
 * 
 * @param str A SuffixArrayben tartalmazand� karaktersorozat
 * @return Egy iter�tor a tal�latokkal
 */
SuffixArray::iterator SuffixArray::keres(const std::string& str) const {
    if (szoveg.length() == 0)
        return iterator();

    std::string *start, *futo;
    start = futo = std::lower_bound(suffix, suffix + szoveg.length() - 1, str);
    while (futo != suffix + szoveg.length() && szoveg.length() >= str.length() && futo->substr(0, str.length()) == str)
        futo++;

    if (start == futo)
        return iterator();
    else
        return iterator(start, --futo);
    return iterator();
}

/**
 * Ad egy iter�tort, ami minden iter�tornak a v�g��l szolg�l.
 * 
 * @return Egy "nullptr" iter�tor 
 */
SuffixArray::iterator SuffixArray::end() const {
    return iterator();
}

/**
 * L�trehoz egy �j SuffixArrayt, ami a k�t SuffixArray �sszef�z�tt sz�veg�t tartalmazza.
 * 
 * @param rhs A hozz�f�zend� SuffixArray
 * @return Egy �j SuffixArray, ami a k�t 
 */
SuffixArray SuffixArray::operator+(const SuffixArray& rhs) const {
    return SuffixArray(szoveg + rhs.szoveg);
}

/**
 * �rt�kad� oper�tor.
 * 
 * @param rhs M�soland� SuffixArray
 */
SuffixArray& SuffixArray::operator=(const SuffixArray& rhs) {
    if (this == &rhs)
        return *this;

    szoveg = rhs.szoveg;
    delete[] suffix;
    suffix = new std::string[szoveg.length()];
    std::copy(rhs.suffix, rhs.suffix + szoveg.length(), suffix);
    return *this;
}

/**
 * Hozz�f�zi a kapott parameter sz�veget a saj�tjahoz.
 * 
 * @param rhs A hozz�f�zend� SuffixArray
 */
SuffixArray& SuffixArray::operator+=(const SuffixArray& rhs) {
    szoveg += rhs.szoveg;
    delete[] suffix;
    suffix = new std::string[szoveg.length()];
    suffixGeneralas();
    return *this;
}

/**
 * Megkeresi a k�t SuffixArray leghosszabb k�z�s szubstring�t.
 * 
 * @param rhs
 * @return A leghosszabb k�z�s szubstringet tartalmaz� SuffixArray
 */
std::string SuffixArray::operator&(const SuffixArray& rhs) const {
    if (szoveg.length() > rhs.szoveg.length())
        return leghosszabb_kozos_substr(rhs.szoveg, rhs.szoveg.length());
    else
        return rhs.leghosszabb_kozos_substr(szoveg, szoveg.length());
}

/**
 * Rekurz�van megkeresi a leghosszabb k�z�s szubstring�t.
 * 
 * @param str A k�z�s r�szt tartalmaz� string
 * @param len A keresett k�z�s r�sz hossza
 * @return
 */
std::string SuffixArray::leghosszabb_kozos_substr(const std::string& str, size_t len) const {
    for (size_t i = 0; i < str.length() - len + 1; i++)
        if (keres(str.substr(i, len)) != end())
            return str.substr(i, len);

    if (len > 1)
        return leghosszabb_kozos_substr(str, len - 1);

    return "";
}

/**
 * Destruktor.
 * Felszabad�tja a dinamikusan lefoglalt mem�ri�t.
 */
SuffixArray::~SuffixArray() {
    delete[] suffix;
}