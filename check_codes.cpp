#include <iostream>
#include <string>
#include <cctype>
#include <vector>

bool validareisbn(const std::string &isbn_raw) {
    std::string isbn;
    for (char c: isbn_raw) {
        if (c != '-' && c != ' ') {
            isbn += c;
        }
    }


    if (isbn.length() != 10 && isbn.length() != 13) {
        return false;
    }

    // --- CAZUL ISBN-10 ---
    if (isbn.length() == 10) {
        int suma = 0;
        for (int i = 0; i < 9; i++) {
            if (!isdigit(isbn[i])) return false;
            suma += (isbn[i] - '0') * (10 - i);
        }

        char last = isbn[9];
        int lastDigit;
        if (last == 'X' || last == 'x') {
            lastDigit = 10;
        } else if (isdigit(last)) {
            lastDigit = last - '0';
        } else {
            return false;
        }

        suma += lastDigit;
        return (suma % 11 == 0);
    }

    // --- CAZUL ISBN-13 ---
    if (isbn.length() == 13) {
        int suma = 0;
        for (int i = 0; i < 12; i++) {
            if (!isdigit(isbn[i])) return false;
            int digit = isbn[i] - '0';

            if (i % 2 == 0) {
                suma += digit * 1;
            } else {
                suma += digit * 3;
            }
        }

        int rest = suma % 10;
        int checkDigit = (rest == 0) ? 0 : (10 - rest);

        if (!isdigit(isbn[12])) return false;
        return (isbn[12] - '0') == checkDigit;
    }

    return false;
}

bool verificareISSN(const std::string& ISSN) {
    if (ISSN.length()!=9 || ISSN[4]!='-')
        return false;
    int s=0;
    for (int i=0;i<7;i++) {
        const int index_c=(i<4)? i:i+1;
        if (!isdigit(ISSN[index_c]))
            return false;
        const int cif=ISSN[index_c]-'0';
        const int pondere=8-i;
        s+=cif*pondere;
    }
    char cif_control;
    if (s%11==0)
        cif_control='0';
      else {
          int v=11-s%11;
          if (v==10)
              cif_control='X';
          else {
              cif_control=v+'0';
          }
      }
    if (cif_control=='X') {
        return (ISSN[8]=='X'|| ISSN[8]=='x');
    }else {
        return ISSN[8]==cif_control;
    }
}

int main() {
    std::vector<std::string> isbns = {
        "978-973-46-1234-5",
        "978-973-46-5678-3",
        "978-973-46-9012-1",
        "978-973-50-1111-6",
        "978-973-50-2222-8",
        "978-973-60-3333-9",
        "978-973-60-4444-1"
    };
    
    std::vector<std::string> issns = {
        "1234-5679",
        "9876-5434"
    };

    std::cout << "Checking ISBNs:\n";
    for (const auto& s : isbns) {
        std::cout << s << ": " << (validareisbn(s) ? "VALID" : "INVALID") << "\n";
    }

    std::cout << "\nChecking ISSNs:\n";
    for (const auto& s : issns) {
        std::cout << s << ": " << (verificareISSN(s) ? "VALID" : "INVALID") << "\n";
    }

    return 0;
}
