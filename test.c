#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void *rbx;
    void *rsp;
    void *rbp;
    void *r12;
    void *r13;
    void *r14;
    void *r15;
} Calleereg;

typedef struct {
    Calleereg *calleereg;
    Calleereg *fcalleereg;
    void *jumpaddr;
    void *args;
} Context;

extern void *get(Context *c);
extern void yield(Context *c, void *v);

Context *init(void (*f)(Context *), void *args, int size) {
    Context *c = malloc(sizeof(Context));
    c->calleereg = malloc(sizeof(Calleereg));
    c->fcalleereg = malloc(sizeof(Calleereg));

    // stackの設定
    int sz = 2 * 1024 * 1024;
    void *rsplast = malloc(sz);
    c->fcalleereg->rsp = rsplast+sz;
    
    // 開始地点の指定
    c->jumpaddr = f;

    // 引数のコピー
    c->args = malloc(size);
    memcpy(c->args, args, size);
    return c;
}


void g(Context *c, int n, char ar[]) {
    if (n == 1) {
        yield(c, ar);
    } else {
        for(int i = 0; i < n; i++) {
            g(c, n - 1, ar);
            int tmp = ar[n - 1];
            if (n % 2 == 0) {
                ar[n - 1] = ar[i];
                ar[i] = tmp;
            } else {
                ar[n - 1] = ar[0];
                ar[0] = tmp;
            }
        }
    }
}

void gen(Context *c) {
    char a[4] = {'a', 'b', 'c', 'd'};
    g(c, 4, a);
    yield(c, NULL);
}


int main() {
    char *x;
    Context *c = init(gen, NULL, 0);
    for(x = get(c); x; x = get(c)) {
        printf("%c %c %c %c\n", x[0], x[1], x[2], x[3]);
    }
}