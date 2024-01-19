Neat little demo to make your discussion a bit more interactive and exciting!

Problem 1:
```
gcc -c app.c -o app.o
arch -x86_64 gcc -c app.c -o app.o (if you have an M1 or M2 chip)
```
Demonstrate that when you examine `objdump -D app.o`, the return value is 16, which is `sizeof(helper_args_t)`
```
44: b8 10 00 00 00                movl    $16, %eax
```
When you uncomment #define ABC on top of lib.h, it turns into
```
44: b8 18 00 00 00                movl    $24, %eax
```

Problem 2:
```
gcc -DABC -c app.c -o app.o
arch -x86_64 gcc -DABC -c app.c -o app.o (if you have an M1 or M2 chip)
```

Again, run objdump:

```
      1d: 48 89 45 e0                   movq    %rax, -32(%rbp)
      21: c6 45 e8 2f                   movb    $47, -24(%rbp)
      25: 48 8d 7d d8                   leaq    -40(%rbp), %rdi
      29: e8 00 00 00 00                callq   0x2e <_main+0x2e>
      2e: 48 89 45 d0                   movq    %rax, -48(%rbp)
```

So to explain, %rdi is the first argument to helper_func.
The address of helper_args starts at -40(%rbp), but the member string and target appear to be at -32(%rbp) and -24(%rbp) respectively!
(Convince yourself that this is correct. 47 is the ASCII representation of '/'.)
```
gcc -c lib.c -o lib.o (again, add arch -x86_64 if you're on an M1 or M2 Mac)
       0: 55                            pushq   %rbp
       1: 48 89 e5                      movq    %rsp, %rbp
       4: 48 89 7d f0                   movq    %rdi, -16(%rbp)
       8: c7 45 ec 00 00 00 00          movl    $0, -20(%rbp)
       f: 48 8b 45 f0                   movq    -16(%rbp), %rax
      13: 48 8b 00                      movq    (%rax), %rax
```
Now it believes (%rax) to be the offset of the member string (so 0 offset from the pointer argument helper_args_t* args stored in %rdi), which was not the case for app! This is happening because it's missing the first member aux.

So now this program has an undefined behavior.
