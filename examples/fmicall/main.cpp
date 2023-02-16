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

typedef struct _func_tbl_t {
    func_t f1;
    func_t f2;
    func_t f3;
} func_tbl_t;

/* func_tbl_t func_tbl; */

/* typedef struct _instance_t { */
/*     func_tbl_t *ft; */
/* } instance_t; */

int main(int argc, char *argv[]) {

    func_tbl_t func_tbl;
    func_tbl.f1 = &foo;
    func_tbl.f2 = &bar;
    func_tbl.f3 = &baz;

    func_tbl.f1(1);
    func_tbl.f2(2);
    func_tbl.f3(3);

    /* func_t f1 = &foo; */
    /* f1(1); */
    /* func_t f2 = &bar; */
    /* f2(2); */
    /* func_t f3 = &baz; */
    /* f3(3); */

    /* func_t f = func_tbl.f1; */
    /* f(1); */
    /* f = func_tbl.f2; */
    /* f(2); */
    /* f = func_tbl.f3; */
    /* f(3); */

    /* instance_t inst; */
    /* inst.ft = (func_tbl_t*)malloc(sizeof(func_tbl_t)); */
    /* inst.ft->f1 = &foo; */
    /* inst.ft->f2 = &bar; */
    /* inst.ft->f3 = &baz; */
    /* inst.ft->f1(1); */
    /* inst.ft->f2(2); */
    /* inst.ft->f3(3); */

    /* func_t ar[3] = {foo, bar, baz}; */
    /* for (int i = 0; i < 3; i++) { */
    /*     func_t f = ar[i]; */
    /*     f(i+1); */
    /* } */

    return 0;
 }

