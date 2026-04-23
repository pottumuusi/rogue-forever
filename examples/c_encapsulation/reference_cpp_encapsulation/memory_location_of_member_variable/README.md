Objektin jäsenmuuttujan paikka muistissa
========================================

Loppuun kirjasin yhteenvedon. Tämän materiaalin kasaamisen yhteydessä löytyi
ohjelman osa, jonka olin ymmärtänyt väärin! Teksti etenee ohjelman ydinkohtien
esittelystä debuggaamiseen.

Proseduurissa _ZN6Module8addToFooEi (metodi addToFoo) käytetään jäsenmuuttujaa
`foo` yhteenlaskussa.
```
int Module::addToFoo(int operand)
{
    return foo + operand;
}
```
```
0000000000001192 <_ZN6Module8addToFooEi>:
...
    11a6:	01 d0                	add    %edx,%eax
```

Rekisteriin `eax` on siirretty `bar` muuttujan arvo 4444 (0x115c).
```
0000000000001192 <_ZN6Module8addToFooEi>:
...
    119a:	89 75 f4             	mov    %esi,-0xc(%rbp)
...
    11a3:	8b 45 f4             	mov    -0xc(%rbp),%eax
```
```
00000000000011bb <main>:
...
    11e3:	c7 45 e8 5c 11 00 00 	movl   $0x115c,-0x18(%rbp)
    11ea:	8b 55 e8             	mov    -0x18(%rbp),%edx
...
    11f1:	89 d6                	mov    %edx,%esi
    11f3:	48 89 c7             	mov    %rax,%rdi
    11f6:	e8 97 ff ff ff       	call   1192 <_ZN6Module8addToFooEi>
```

Rekisteri `edx` sisältää siis jäsenmuuttujan `foo` arvon. Arvo päätyy
rekisteriin seuraavien käskyjen mukaisesti:
```
0000000000001192 <_ZN6Module8addToFooEi>:
...
    1196:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
...
    119d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    11a1:	8b 10                	mov    (%rax),%edx
```
```
00000000000011bb <main>:
...
    11ed:	48 8d 45 e0          	lea    -0x20(%rbp),%rax
...
    11f1:	89 d6                	mov    %edx,%esi
    11f3:	48 89 c7             	mov    %rax,%rdi
    11f6:	e8 97 ff ff ff       	call   1192 <_ZN6Module8addToFooEi>
```

Alunperin taisin katsoa, ettei main tai _start proseduureissa kirjoitusta tehdä
tarkalleen osoitteeseen -0x20(%rbp). Nyt huomasin, että main proseduurin alussa
pinoon kirjoitetaan -0x20(%rbp) lähistölle osoitteeseen -0x24(%rbp):
```
00000000000011bb <main>:
...
    11c4:	89 7d dc             	mov    %edi,-0x24(%rbp)
```

Tähänastinen disassembly on ohjelmasta, jota kääntäessä ei ole käytetty -g
lippua. Seuraavana gdb komentoja, joilla tutkin ohjelmaa. Pinoon osoittavan
osoittavan osoitteen kirjoittaminen %rax rekisteriin ja tämän osoitteen takana
olevan arvon muutos olivat tärkeimmät tekijät ohjelman ymmärtämiseen.

```
Breakpoint 1, main (argc=1, argv=0x7fffffffde38) at main.cpp:16
16          Module module = Module(222);
```
```
(gdb) info registers
...
rip            0x5555555551cb      0x5555555551cb <main(int, char**)+16>
```
```
(gdb) disassemble 0x5555555551cb
Dump of assembler code for function main(int, char**):
   0x00005555555551bb <+0>:     push   %rbp
   0x00005555555551bc <+1>:     mov    %rsp,%rbp
   0x00005555555551bf <+4>:     push   %rbx
   0x00005555555551c0 <+5>:     sub    $0x28,%rsp
   0x00005555555551c4 <+9>:     mov    %edi,-0x24(%rbp)
   0x00005555555551c7 <+12>:    mov    %rsi,-0x30(%rbp)
=> 0x00005555555551cb <+16>:    lea    -0x20(%rbp),%rax
```
```
(gdb) info registers
...
rax            0x5555555551bb      93824992235963
rdi            0x1                 1
rbp            0x7fffffffdd20      0x7fffffffdd20
```
```
python3
>>> hex(0x7fffffffdd20 - 0x24)
'0x7fffffffdcfc'
>>> hex(0x7fffffffdd20 - 0x20)
'0x7fffffffdd00'
>>>
```
```
(gdb) x 0x7fffffffdcfc
0x7fffffffdcfc: 0x00000001
(gdb) x 0x7fffffffdd00
0x7fffffffdd00: 0x00000027
(gdb) x/2 0x7fffffffdd00
0x7fffffffdd00: 0x00000027      0x00000000
```
```
(gdb) stepi
0x00005555555551cf      16          Module module = Module(222);
(gdb) disassemble 0x5555555551cb
...
   0x00005555555551cb <+16>:    lea    -0x20(%rbp),%rax
=> 0x00005555555551cf <+20>:    mov    $0xde,%esi
(gdb) info registers
rax            0x7fffffffdd00      140737488346368
```
```
(gdb) step
Module::Module (this=0x7fffffffdd00, _foo=222) at Module.cpp:8
8           : foo{_foo}
(gdb) step
10          if (1 == _foo) {
(gdb)
15          foo = _foo;
(gdb)
16      }
(gdb) x 0x7fffffffdd00
0x7fffffffdd00: 0x000000de
(gdb)
```
```
python3
>>> int(0x000000de)
222
```

Huomasin, että osoitteessa, johon jäsenmuuttujan arvo tallennetaan, oli '27'
ennen constructorin suorittamaa alustusta. Tässä vaiheessa aloin ymmärtää
tilanteen hieman eri tavalla.
1. Tyyppiä Module oleva module muuttuja on main funktion sisällä päätyen
   pinomuistiin.
2. %rax rekisteriin tallennettiinkin arvo käyttäen `lea`, eikä `mov` käskyä.

Suurin sekaannus oli, kun luulin, että -0x20(%rbp) osoitteen takaa luetaan
osoitin %rax rekisteriin. Jäsenmuuttujan sijaitseminen pinossa käy myös
järkeen, kun main funktion mukaan Module luokan objekti sijoitetaan pinoon.
Pinomuistin osoiteavaruuden voi tarkistaa komennolla:
```
(gdb) info proc mappings
```
