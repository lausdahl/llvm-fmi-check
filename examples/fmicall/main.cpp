#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

typedef int(*func_t)(int);

int foo(int p) {
    return p*2;
}

int bar(int p) {
    return p+2;
}

int baz(int p) {
    return p/2;
}

struct func_tbl_t {
    func_t f1;
    func_t f2;
    func_t f3;
} func_tbl;

int main(int argc, char *argv[]) {
    func_tbl.f1 = &foo;
    func_tbl.f2 = &bar;
    func_tbl.f3 = &baz;

    func_tbl.f1(1);
    func_tbl.f2(2);
    func_tbl.f3(3);

    /* func_t f = func_tbl.f1; */
    /* f(1); */
    /* f = func_tbl.f2; */
    /* f(2); */
    /* f = func_tbl.f3; */
    /* f(3); */

    func_t f = &foo;
    f(1);
    f = &bar;
    f(2);
    f = &baz;
    f(3);

    /* func_t ar[3] = {foo, bar, baz}; */
    /* for (int i = 0; i < 3; i++) { */
    /*     func_t f = ar[i]; */
    /*     f(i+1); */
    /* } */

    return 0;
 }

