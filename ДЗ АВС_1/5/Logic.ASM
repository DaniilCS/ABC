format PE console

entry Start

include 'win32a.inc'

section '.data' data readable writable
                            resStr db 'Result: %d', 0
                            str1 db 'ZF = 1, op resultis False', 0
                            str2 db 'ZF = 0, op result TRUE', 0

         A dd ?
         B dd ?
         C dd ?

         NULL = 0

section '.code' code readable executable

        Start:
              mov eax, 10000b
              test eax, 110111b

              jz ifZFTrue


              push str2
              call[printf]

              ifZFTrue:
              push str1
              call[printf]

              finish:
              call[getch]

              push NULL
              call[ExitProcess]

section '.idata' import data readable
        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
        ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               getch, '_getch',\
               scanf, 'scanf'