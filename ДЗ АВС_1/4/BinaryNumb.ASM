format PE console

entry Start

include 'win32a.inc'

section '.data' data readable writable
                            resStr db 'Result: %d', 0

         A dd ?
         B dd ?
         C dd ?

         NULL = 0

section '.code' code readable executable

        Start:
               mov ecx, 1111100b
               shr ecx, 1
               rol ecx, 2

               push ecx
               push resStr
               call[printf]

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