#include <iostream>
#include <openssl/des.h>
using namespace std;

// this function does the encryption
string enc(string p, int s) {
    DES_cblock k = {0}, b = {0}, r;
    DES_key_schedule sch;

    // put password into key (only first 8 chars)
    for (int i = 0; i < 8 && i < p.length(); i++)
        k[i] = p[i];

    // setup DES
    DES_set_key_unchecked(&k, &sch);

    // use salt to make starting block
    b[0] = s / 256;
    b[1] = s % 256;

    // repeat DES 25 times 
    for (int i = 0; i < 25; i++) {
        DES_ecb_encrypt(&b, &r, &sch, DES_ENCRYPT);

        // copy result back (arrays cant be assigned directly)
        for (int j = 0; j < 8; j++)
            b[j] = r[j];
    }

    // make final string 
    string out = to_string(s) + ":";
    for (int i = 0; i < 8; i++)
        out += to_string((int)r[i]);

    return out;
}

int main() {

    // some passwords i chose
    string myPass[10] = {
        "money1","hello2","main3","insane4","true5",
        "test1","test2","hello1","abc123","pass99"
    };

    // salts (just random numbers)
    int mySalt[10] = {
        111,222,333,444,555,
        666,777,888,999,1234
    };

    cout << "Encrypted passwords:\n";

    // encrypt all passwords
    for (int i = 0; i < 10; i++) {
        cout << myPass[i] << " -> " << enc(myPass[i], mySalt[i]) << endl;
    }

    // now checking if password is correct
    cout << "\nChecking password:\n";

    string saved = enc("money1", 111);

    // correct password test
    if (enc("money1", 111) == saved)
        cout << "money1 correct\n";
    else
        cout << "money1 wrong\n";

    // wrong password test
    if (enc("wrong", 111) == saved)
        cout << "wrong correct\n";
    else
        cout << "wrong wrong\n";

    return 0;
}