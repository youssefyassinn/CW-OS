#include <iostream>
#include <openssl/des.h>
using namespace std;

// this function encrypts the password using DES 25 times
string enc(string p, int s) {
    DES_cblock k = {0}, b = {0}, r;
    DES_key_schedule sch;

    // put password into key 
    for (int i = 0; i < 8 && i < p.length(); i++)
        k[i] = p[i];

    DES_set_key_unchecked(&k, &sch);

    // using salt to make starting block
    b[0] = s / 256;
    b[1] = s % 256;

    // repeat encryption 25 times
    for (int i = 0; i < 25; i++) {
        DES_ecb_encrypt(&b, &r, &sch, DES_ENCRYPT);
        b = r;
    }

    // making final string (salt:hash)
    string out = to_string(s) + ":";
    for (int i = 0; i < 8; i++)
        out += to_string((int)r[i]);

    return out;
}

int main() {
    string myPass[10] = {
        "money1","meen","ana3","dead4","good5",
        "test1","test2","hello1","abc123","pass99"
    };

    int mySalt[10] = {
        111,222,333,444,555,
        666,777,888,999,1234
    };

    cout << "Encrypted passwords:\n";

    // printing encrypted passwords
    for (int i = 0; i < 10; i++) {
        cout << myPass[i] << " -> " << enc(myPass[i], mySalt[i]) << endl;
    }

    // checking part
    cout << "\nChecking password:\n";

    string saved = enc("money1", 111);

    if (enc("money1", 111) == saved)
        cout << "money1 correct\n";
    else
        cout << "money1 wrong\n";

    if (enc("wrong", 111) == saved)
        cout << "wrong correct\n";
    else
        cout << "wrong wrong\n";

    return 0;
}
