#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

#define MYATTR(x) __attribute__ ((annotate(x)))

typedef int(*func1_t)(int);
typedef int(*func2_t)(int, int);
typedef int(*func3_t)(int, int, int);

// annotation here only needed if function directly referenced as global
int foo1(int p) MYATTR("func1") {
    return p*2;
}

// annotation here only needed if function directly referenced as global
int foo2(int p, int q) MYATTR("func2") {
    return p + q;
}

// annotation here only needed if function directly referenced as global
int foo3(int p, int q, int r) MYATTR("func3") {
    return p + q + r;
}

typedef struct _func_tbl_t {
    func1_t f1 MYATTR("func1");
    func2_t f2 MYATTR("func2");
    func3_t f3 MYATTR("func3");
} func_tbl_t;

typedef struct _instance_t {
    func_tbl_t *ft;
    int comp;
} instance_t;

int main(int argc, char *argv[]) {

    /* func_tbl_t func_tbl; */
    /* func_tbl.f1 = &foo1; */
    /* func_tbl.f2 = &foo2; */
    /* func_tbl.f3 = &foo3; */

    /* func_tbl.f1(1); */
    /* func_tbl.f2(1,2); */
    /* func_tbl.f3(1,2,3); */

    /* // detect using global annotate */
    /* func1_t f1 = &foo1; */
    /* f1(1); */
    /* func2_t f2 = &foo2; */
    /* f2(1,2); */
    /* func3_t f3 = &foo3; */
    /* f3(1,2,3); */

    instance_t inst1;
    inst1.ft = (func_tbl_t*)malloc(sizeof(func_tbl_t));
    inst1.ft->f1 = &foo1;
    inst1.ft->f2 = &foo2;
    inst1.ft->f3 = &foo3;
    inst1.comp = 1;
    inst1.ft->f1(inst1.comp);
    inst1.ft->f2(inst1.comp,2);
    inst1.ft->f3(inst1.comp,2,3);

    instance_t inst2;
    inst2.ft = (func_tbl_t*)malloc(sizeof(func_tbl_t));
    inst2.ft->f1 = &foo1;
    inst2.ft->f2 = &foo2;
    inst2.ft->f3 = &foo3;
    inst2.comp = 1;
    inst2.ft->f1(inst2.comp);
    inst2.ft->f2(inst2.comp,2);
    inst2.ft->f3(inst2.comp,2,3);
    /* func_tbl_t *ft = (func_tbl_t*)malloc(sizeof(func_tbl_t)); */
    /* /1* func_tbl2_t ft; *1/ */
    /* ft->f1 = &foo1; */
    /* ft->f2 = &foo2; */
    /* ft->f3 = &foo3; */
    /* ft->f1(1); */
    /* ft->f2(1,2); */
    /* ft->f3(1,2,3); */
    /* /1* func_t ar[3] = {foo, bar, baz}; *1/ */
    /* for (int i = 0; i < 3; i++) { */
    /*     func_t f = ar[i]; */
    /*     f(i+1); */
    /* } */

    return 0;
 }

