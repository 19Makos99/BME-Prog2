#include <string>
#include <stdexcept>
#include <algorithm>

#include "suffix_array.h"

/**
 * Növekvö sorba rakja a suffix tömb tartalmát a < operátorral.
 */
void SuffixArray::rendez() {
    std::sort(suffix, suffix + szoveg.length());
}

/**
 * A szövegböl feltölti a suffix tömböt.
 * Elmenti a szöveget a suffix tömbben, majd elhagyja az elsö betüt és ezt ismétli, amig a szöveg el nem fogy (a szöveg nem változik).
 * A suffix tömbnek elöre le kell foglalni szoveg.length() helyet.
 */
void SuffixArray::suffixGeneralas() {
    for (size_t i = 0; i < szoveg.length(); ++i)
        suffix[i] = szoveg.substr(i);
}

/**
 * Iterátor konstruktor.
 * 
 * @param f Az elsö elem pointere ami megfelel a kriteriumoknak
 * @param l Az utolso elem pointere ami megfelel a kriteriumoknak
 */
SuffixArray::iterator::iterator(std::string* f, std::string* l) : first(f), last(l) {}

/**
 * Pre inkremens operátor.
 * Hibát dob, ha az end()-en túl próbálják növelni.
 * 
 * @return A megnövelt iterátor
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
 * Post inkremens operátor.
 * Hibát dob, ha az end()-en túl próbálják növelni.
 * 
 * @return Az iterátor a növelés elött
 */
SuffixArray::iterator SuffixArray::iterator::operator++(int) {
    iterator tmp(*this);
    operator++();
    return tmp;
}

/**
 * Két iterátor egyenlöségét vizsgálja
 *
 * @return Egyenlö-e a két iterátor
 */
bool SuffixArray::iterator::operator==(const iterator& rhs) const {
    return first == rhs.first && last == rhs.last;
}

/**
 * Két iterátor nem egyenlöségét vizsgálja
 *
 * @return Nem egyenlö-e a két iterátor
 */
bool SuffixArray::iterator::operator!=(const iterator& rhs) const {
    return !(*this == rhs);
}

/**
 * Dereferáló operátor.
 * Vissza adja az iterátor által mutatott string-et.
 * Hibát dob, ha az iterátor egyenlö az end()-el.
 *
 * @return Az iterátor által mutatott string
 */
const std::string& SuffixArray::iterator::operator*() const {
    if (first == nullptr)
        throw std::out_of_range("Tulindexeles");
    return *first;
}

/**
 * Nyíl operátor.
 * Vissza adja az iterátor által mutatott string pointert.
 * Hibát dob, ha az iterátor egyenlö az end()-el.
 * 
 * @return A mutatott string pointerje
 */
const std::string* SuffixArray::iterator::operator->() const {
    if (first == nullptr)
        throw std::out_of_range("Tulindexeles");
    return first;
}

/**
 * C stílusú stringböl konstruktor.
 */
SuffixArray::SuffixArray(const char* str) : szoveg(str), suffix(new std::string[szoveg.length()]) {
    suffixGeneralas();
    rendez();
}

/**
 * Stringböl konstruktor.
 */
SuffixArray::SuffixArray(const std::string& str) : szoveg(str), suffix(new std::string[szoveg.length()]) {
    suffixGeneralas();
    rendez();
}

/**
 * Másoló konstruktor.
 */
SuffixArray::SuffixArray(const SuffixArray& rhs) : szoveg(rhs.szoveg), suffix(new std::string[szoveg.length()]) {
    std::copy(rhs.suffix, rhs.suffix + szoveg.length(), suffix);
    rendez();
}

/**
 * Vissza adja a SuffixArray szövegét.
 * 
 * @return A SuffixArray szövege
 */
const char* SuffixArray::c_str() const {
    return szoveg.c_str();
}

/**
 * Megkeresi az összes karaktersorozatot a SuffixArray-ben, ami a kapott karakterrel kezdödik.
 * Ha nincsen megfelelö talalat akkor az iterátor egyenlö lesz az end() értékkel.
 *
 * @param str A SuffixArrayben tartalmazandó karaktersorozat
 * @return Egy iterátor a találatokkal
 */
SuffixArray::iterator SuffixArray::keres(const char c) const {
    return keres(std::string(1, c));
}

/**
 * Megkeresi az összes karaktersorozatot a SuffixArray-ben, ami a kapott karakterekkel kezdödik.
 * Ha nincsen megfelelö talalat akkor az iterátor egyenlö lesz az end() értékkel.
 *
 * @param str A SuffixArrayben tartalmazandó karaktersorozat
 * @return Egy iterátor a találatokkal
 */
SuffixArray::iterator SuffixArray::keres(const char* str) const {
    return keres(std::string(str));
}

/**
 * Megkeresi az összes karaktersorozatot a SuffixArray-ben, ami a kapott karakterekkel kezdödik.
 * Ha nincsen megfelelö talalat akkor az iterátor egyenlö lesz az end() értékkel.
 * 
 * @param str A SuffixArrayben tartalmazandó karaktersorozat
 * @return Egy iterátor a találatokkal
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
 * Ad egy iterátort, ami minden iterátornak a végéül szolgál.
 * 
 * @return Egy "nullptr" iterátor 
 */
SuffixArray::iterator SuffixArray::end() const {
    return iterator();
}

/**
 * Létrehoz egy új SuffixArrayt, ami a két SuffixArray összefüzött szövegét tartalmazza.
 * 
 * @param rhs A hozzáfüzendö SuffixArray
 * @return Egy új SuffixArray, ami a két 
 */
SuffixArray SuffixArray::operator+(const SuffixArray& rhs) const {
    return SuffixArray(szoveg + rhs.szoveg);
}

/**
 * Értékadó operátor.
 * 
 * @param rhs Másolandó SuffixArray
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
 * Hozzáfüzi a kapott parameter szöveget a sajátjahoz.
 * 
 * @param rhs A hozzáfüzendö SuffixArray
 */
SuffixArray& SuffixArray::operator+=(const SuffixArray& rhs) {
    szoveg += rhs.szoveg;
    delete[] suffix;
    suffix = new std::string[szoveg.length()];
    suffixGeneralas();
    return *this;
}

/**
 * Megkeresi a két SuffixArray leghosszabb közös szubstringét.
 * 
 * @param rhs
 * @return A leghosszabb közös szubstringet tartalmazó SuffixArray
 */
std::string SuffixArray::operator&(const SuffixArray& rhs) const {
    if (szoveg.length() > rhs.szoveg.length())
        return leghosszabb_kozos_substr(rhs.szoveg, rhs.szoveg.length());
    else
        return rhs.leghosszabb_kozos_substr(szoveg, szoveg.length());
}

/**
 * Rekurzívan megkeresi a leghosszabb közös szubstringét.
 * 
 * @param str A közös részt tartalmazó string
 * @param len A keresett közös rész hossza
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
 * Felszabadítja a dinamikusan lefoglalt memóriát.
 */
SuffixArray::~SuffixArray() {
    delete[] suffix;
}