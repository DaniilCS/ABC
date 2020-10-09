; вариант задания 5
format PE console
entry start

include 'win32a.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable

        strVecSize     db 'Size of vector A: ', 0
        strIncorSize   db 'Incorrect size of vector = %d', 10, 0
        strVecElemI    db '[%d]? ', 0
        strScanInt     db '%d', 0
        strX           db 'Value of X = ', 0
        strPrintB        db 'Array B:', 10, 0
        strVecElemOut  db '[%d] = %d', 10, 0
        newline db '', 10, 0

        vec_size     dd 0
        X            dd 0
        count        dd 0
        i            dd ?
        tmp          dd ?
        tmpB         dd ?
        vec          rd 100
        vecB         rd 100

;--------------------------------------------------------------------------
section '.code' code readable executable
start:
        call VectorAndLength
        call XInput
        call NullVect
        call FindX
        call NullVect
        call FillB
        call NullVect
        call PrintB


        finish:
        call [getch]

        push 0
        call [ExitProcess]

VectorAndLength:
        push strVecSize ;запрос на считывание с консоли размера массива А
        call [printf]
        add esp, 4

        push vec_size   ;считывание с консоли размера массива А
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vec_size] ;записываем длинну массива в регистр
        cmp eax, 0
        jg  getVector ;обработка неправильной длинны


        push [vec_size]
        push strIncorSize
        call [printf]
        call [getch]
        push 0
        call [ExitProcess]

getVector:; считываем массив из консоли(как в нулевике с суммой)
        xor ecx, ecx
        mov ebx, vec
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge endInputVector

        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        mov ebx, vec
        xor ecx, ecx
        ret

XInput:; считывание значения X
        push strX
        call [printf]
        add esp, 4

        push X
        push strScanInt
        call [scanf]
        add esp, 8
        ret

NullVect: ;обнуление счетчиков и регистров для массивов
        xor ecx, ecx
        mov [i], 0
        xor eax, eax
        mov ebx, vec
        mov edx, vecB
        ret

FindX: ; нахождение элементов равных X в исходном массиве и количество их вхождений
        cmp ecx, [vec_size]
        jge FindFin
        mov eax, [ebx]
        cmp eax, [X]
        je CountPlusPlus
        inc ecx
        add ebx, 4
        jmp FindX

CountPlusPlus:
        inc [count]
        inc ecx
        add ebx, 4
        jmp FindX

FindFin:
        ret

FillB:
       cmp ecx, [vec_size]
       jge FindFin
       mov eax, [ebx]
       cmp eax, [X]
       jne addElement
       add ebx, 4
       add ecx, 1
       jmp FillB

addElement:
        mov [edx], eax
        add edx, 4
        add ebx, 4
        add ecx, 1
        mov [i], ecx
        jmp FillB

PrintB:   ; вывод строки strBVec и всего массива B
        mov [tmp], ecx
        push strPrintB
        call [printf]
        mov ecx, [tmp]
        mov edx, vecB
        add esp, 4
        mov eax, [vec_size]
        sub eax, [count]
Print:  ;вывод элементов массива B
        cmp ecx, eax
        jge theEnd
        mov [tmp], ecx
        mov [tmpB], eax
        mov [i], edx
        push dword [edx]
        push ecx
        push strVecElemOut
        call [printf]
        add esp, 8
        mov ecx, [tmp]
        mov eax, [tmpB]
        mov edx, [i]
        add edx, 4
        add ecx, 1
        jmp Print
theEnd:
        jmp finish

;--------------------------------------------------------------------------
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'                                    