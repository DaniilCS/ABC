format PE console

entry Start

include 'win32a.inc'

section '.data' data readable writable
                            resStr db 'Result: %d', 0

         A dd ?
         B dd ?
         C dd ?

         emptyStr db '%d', 0
         point db ',', 0

         NULL = 0

section '.code' code readable executable

        Start:
              mov [A], 100
              mov [B], 29

              mov eax, [A]
              mov ecx, [B]
              mov edx, 0

              div ecx
              mov [C], edx

              push eax
              push resStr
              call[printf]

              push point
              call[printf]

              mov ebx, 0

              lp:
                  mov eax, [C]
                  mov ecx, [B]
                  imul eax, 10

                  mov edx, 0
                  div ecx
                  mov [C], edx

                  push eax
                  push emptyStr
                  call [printf]

                  add ebx, 1
                  cmp ebx, 6
              jne lp

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