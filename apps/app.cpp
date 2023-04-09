#include <iostream>
#include "ipre.h"

using namespace std;

int main() {
    cout << "Testing in the app." << endl;

    // Init core and setup.
    core_init();
    pc_param_set_any();

    // Testing.
    g base, test_value;
    gen(base);

    g1_mul_dig(test_value, base, 2);

    g table[200];
    cout << RLC_EP_TABLE_MAX << endl << endl;

    g1_mul_pre(table, base);

    g1_print(table[0]);
    cout << endl;
    g1_print(table[1]);
    cout << endl;
    g1_print(table[2]);
    cout << endl;
    g1_print(test_value);
    cout << endl;
    cout << (g1_cmp(table[9], test_value) == RLC_EQ) << endl;
}