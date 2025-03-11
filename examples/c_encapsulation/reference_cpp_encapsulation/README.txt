(gdb) list 15
10      {
11          Module module = Module(222);
12
13          int result = 0;
14          int bar = 4444;
15
16          result = module.addToFoo(bar);
17
18          printf("result is: %d\n", result);
19          printf("getBeacon() is: %d\n", module.getBeacon());

(gdb) r
Starting program: /home/tank/my/repositories/github/pottumuusi/rogue-forever/examples/c_encapsulation/reference_cpp_encapsulation/module_test
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, main (argc=1, argv=0x7fffffffde88) at ./main.cpp:16
16          result = module.addToFoo(bar);

(gdb) info registers
rax            0x7fffffffdd50      140737488346448
rbx            0x7fffffffde88      140737488346760
rcx            0x555555557db8      93824992247224
rdx            0xde                222
rsi            0xde                222
rdi            0x7fffffffdd50      140737488346448
rbp            0x7fffffffdd70      0x7fffffffdd70
rsp            0x7fffffffdd40      0x7fffffffdd40
r8             0x0                 0
r9             0x7ffff7fcf680      140737353938560
r10            0x7ffff7fcb878      140737353922680
r11            0x7ffff7fe1940      140737354012992
r12            0x0                 0
r13            0x7fffffffde98      140737488346776
r14            0x555555557db8      93824992247224
r15            0x7ffff7ffd020      140737354125344
rip            0x555555555178      0x555555555178 <main(int, char**)+47>
eflags         0x206               [ PF IF ]
cs             0x33                51
ss             0x2b                43
ds             0x0                 0
es             0x0                 0
fs             0x0                 0
gs             0x0                 0

(gdb) x/2 0x7fffffffdd50
0x7fffffffdd50: 0x000000de      0x000015b3

(gdb) x/20 0x7fffffffdd28
0x7fffffffdd28: 0xffffdd50      0x00007fff      0xffffdd70      0x00007fff
0x7fffffffdd38: 0x5555516a      0x00005555      0xffffde88      0x00007fff
0x7fffffffdd48: 0x55559010      0x00000001      0x000000de      0x000015b3
0x7fffffffdd58: 0x0000115c      0x00000000      0x00000000      0x00000000
0x7fffffffdd68: 0xffffde88      0x00007fff      0x00000001      0x00000000

(gdb) info proc mappings
process 354271
Mapped address spaces:

          Start Addr           End Addr       Size     Offset  Perms  objfile
      0x555555554000     0x555555555000     0x1000        0x0  r--p   /home/tank/my/repositories/github/pottumuusi/rogue-forever/examples/c_encapsulation/reference_cpp_encapsulation/module_test
      0x555555555000     0x555555556000     0x1000     0x1000  r-xp   /home/tank/my/repositories/github/pottumuusi/rogue-forever/examples/c_encapsulation/reference_cpp_encapsulation/module_test
      0x555555556000     0x555555557000     0x1000     0x2000  r--p   /home/tank/my/repositories/github/pottumuusi/rogue-forever/examples/c_encapsulation/reference_cpp_encapsulation/module_test
      0x555555557000     0x555555558000     0x1000     0x2000  r--p   /home/tank/my/repositories/github/pottumuusi/rogue-forever/examples/c_encapsulation/reference_cpp_encapsulation/module_test
      0x555555558000     0x555555559000     0x1000     0x3000  rw-p   /home/tank/my/repositories/github/pottumuusi/rogue-forever/examples/c_encapsulation/reference_cpp_encapsulation/module_test
      0x555555559000     0x55555557a000    0x21000        0x0  rw-p   [heap]
      0x7ffff7a1f000     0x7ffff7a45000    0x26000        0x0  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7a45000     0x7ffff7b9a000   0x155000    0x26000  r-xp   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7b9a000     0x7ffff7bed000    0x53000   0x17b000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7bed000     0x7ffff7bf1000     0x4000   0x1ce000  r--p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7bf1000     0x7ffff7bf3000     0x2000   0x1d2000  rw-p   /usr/lib/x86_64-linux-gnu/libc.so.6
      0x7ffff7bf3000     0x7ffff7c00000     0xd000        0x0  rw-p
      0x7ffff7c00000     0x7ffff7c99000    0x99000        0x0  r--p   /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30
      0x7ffff7c99000     0x7ffff7d9a000   0x101000    0x99000  r-xp   /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30
      0x7ffff7d9a000     0x7ffff7e09000    0x6f000   0x19a000  r--p   /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30
      0x7ffff7e09000     0x7ffff7e14000     0xb000   0x209000  r--p   /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30
      0x7ffff7e14000     0x7ffff7e17000     0x3000   0x214000  rw-p   /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30
      0x7ffff7e17000     0x7ffff7e1a000     0x3000        0x0  rw-p
      0x7ffff7ea9000     0x7ffff7ead000     0x4000        0x0  rw-p
      0x7ffff7ead000     0x7ffff7ebd000    0x10000        0x0  r--p   /usr/lib/x86_64-linux-gnu/libm.so.6
--Type <RET> for more, q to quit, c to continue without paging--

      0x7ffff7f30000     0x7ffff7f8a000    0x5a000    0x83000  r--p   /usr/lib/x86_64-linux-gnu/libm.so.6
      0x7ffff7f8a000     0x7ffff7f8b000     0x1000    0xdc000  r--p   /usr/lib/x86_64-linux-gnu/libm.so.6
      0x7ffff7f8b000     0x7ffff7f8c000     0x1000    0xdd000  rw-p   /usr/lib/x86_64-linux-gnu/libm.so.6
      0x7ffff7f8c000     0x7ffff7f8f000     0x3000        0x0  r--p   /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff7f8f000     0x7ffff7fa6000    0x17000     0x3000  r-xp   /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff7fa6000     0x7ffff7faa000     0x4000    0x1a000  r--p   /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff7faa000     0x7ffff7fab000     0x1000    0x1d000  r--p   /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff7fab000     0x7ffff7fac000     0x1000    0x1e000  rw-p   /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
      0x7ffff7fc3000     0x7ffff7fc5000     0x2000        0x0  rw-p
      0x7ffff7fc5000     0x7ffff7fc9000     0x4000        0x0  r--p   [vvar]
      0x7ffff7fc9000     0x7ffff7fcb000     0x2000        0x0  r-xp   [vdso]
      0x7ffff7fcb000     0x7ffff7fcc000     0x1000        0x0  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7fcc000     0x7ffff7ff1000    0x25000     0x1000  r-xp   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ff1000     0x7ffff7ffb000     0xa000    0x26000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffb000     0x7ffff7ffd000     0x2000    0x30000  r--p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffff7ffd000     0x7ffff7fff000     0x2000    0x32000  rw-p   /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
      0x7ffffffde000     0x7ffffffff000    0x21000        0x0  rw-p   [stack]

The member variables of C++ seem to reside in stack. 222 gets passed to Module
constructor which assigns the value to member variable `foo`. Before call to
addToFoo(), values of %rax and %rdi are equal. Then
%rdi is moved to -0x8(%rbp), 
-0x8(%rbp) is moved to %rax
and finally %rax is dereferenced, the result being stored to %edx, which is
used as one of the operands in addition. When resting at the breakpoint on line
16, %rax contains 0x7fffffffdd50. Dereferencing 0x7fffffffdd50 results in
0x000000de which is 222 in decimal numbers.
