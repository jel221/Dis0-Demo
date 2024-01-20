Neat little demo for Headers and how gcc computes struct member offsets!

Problem 1:
```
gcc -c app.c -o app.o
arch -x86_64 gcc -c app.c -o app.o (if you have an M1 or M2 chip)
```
Examine `objdump -D app.o`, the return value is 16, which is `sizeof(helper_args_t)`
```
44: b8 10 00 00 00                movl    $16, %eax
```
When you uncomment _#define ABC_ on top of `lib.h`, it turns into
```
44: b8 18 00 00 00                movl    $24, %eax
```

Problem 2:
```
gcc -DABC -c app.c -o app.o
arch -x86_64 gcc -DABC -c app.c -o app.o (if you have an M1 or M2 chip)
```

Again, run `objdump` on app.o:

```
      1d: 48 89 45 e0                   movq    %rax, -32(%rbp)
      21: c6 45 e8 2f                   movb    $47, -24(%rbp)
      25: 48 8d 7d d8                   leaq    -40(%rbp), %rdi
      29: e8 00 00 00 00                callq   0x2e <_main+0x2e>
      2e: 48 89 45 d0                   movq    %rax, -48(%rbp)
```

So to explain, `%rdi` is the first argument to helper_func. (Review x86_64 calling conventions. C can choose to pass arguments through either the stack or registers if there are fewer than 4 arguments)
The address of `helper_args` (`&helper_args` in app.c) starts at `-40(%rbp)`, but the member string and target appear to be at `-32(%rbp)` and `-24(%rbp)` respectively! So the first 8 bytes starting at `-40(%rbp` is `char* aux` as expected.

Comment on the `-40(%rbp)`:  This might be confusing because it appears to be indexing into the address in %rbp, but leaq loads the effective address in a register with a provided constant offset. So `%rax` will get the value `%rbp - 40`.
(Convince yourself that this is correct. 47 is the ASCII representation of '/'.)
```
gcc -c lib.c -o lib.o (again, add arch -x86_64 if you're on an M1 or M2 Mac)
```
```
       0: 55                            pushq   %rbp
       1: 48 89 e5                      movq    %rsp, %rbp
       4: 48 89 7d f0                   movq    %rdi, -16(%rbp)
       8: c7 45 ec 00 00 00 00          movl    $0, -20(%rbp)
       f: 48 8b 45 f0                   movq    -16(%rbp), %rax
      13: 48 8b 00                      movq    (%rax), %rax
```
Recall that `%rdi` was `&helper_args`. To prepare for the main body of the function which repeatedly uses this pointer, a copy of this pointer is stored in `-16(%rbp)`.

_(An aside: Why this location specifically? This is where it would be if the argument was passed normally through the stack. `(%rbp)` currently stores the previous stack frame pointer, after executing `pushq %rbp`. `-8(%rbp)` would be the saved instructional pointer.)_

It gets copied to the `%rax` register afterward. So `%rax` now stores `&helper_args` from app.c:main. Now it accesses `(%rax)`, an offset of 0 from the pointer, believing this to be the address of the member string. However, this is supposed to store `char* aux` according to what we saw in `app.o`! If it were to be consistent with `app.o`, it should actually load from `-8(%rax)`.

So now this program exhibits undefined behavior. (In fact, it will most likely segfault.)
