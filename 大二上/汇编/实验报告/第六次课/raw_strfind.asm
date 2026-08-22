        DATA  SEGMENT

     BUFFERA  DB        32
              DB        0
              DB        32 DUP(0)
     BUFFERB  DB        32
              DB        0
              DB        32 DUP(0)
           PROMPTSTRINGA  DB        'input A string:',0AH,0DH,'$'
           PROMPTSTRINGB  DB        'input B string:',0AH,0DH,'$'
       PROMPTSTRINGENTER  DB        0AH,0DH,'$'
 FOUNDSTRING  DB        'FOUND IT',0AH,0DH,'$'
          NOTFOUNDSTRING  DB        'NOT FOUND IT',0AH,0DH,'$'
        DATA  ENDS
        CODE  SEGMENT
              ASSUME    CS:CODE,DS:DATA

      START:
;input A B string
              MOV       DX,DATA
              MOV       DS,DX

              MOV       DX,OFFSET PROMPTSTRINGA
              MOV       AH,09H
              INT       21H
              MOV       DX,SEG BUFFERA
              MOV       DS,DX
              MOV       DX,OFFSET BUFFERA
              MOV       AH,0AH
              INT       21H

              MOV       DX,OFFSET PROMPTSTRINGENTER
              MOV       AH,09H
              INT       21H

              MOV       DX,OFFSET PROMPTSTRINGB
              MOV       AH,09H
              INT       21H
              MOV       DX,SEG BUFFERB
              MOV       ES,DX
              MOV       DX,OFFSET BUFFERB
              MOV       AH,0AH
              INT       21H

              MOV       DX,OFFSET PROMPTSTRINGENTER
              MOV       AH,09H
              INT       21H
;look for if A in B
              MOV       SI,OFFSET BUFFERA
              MOV       DI,OFFSET BUFFERB
              ADD       SI,02H
              ADD       DI,01H
              PUSH      SI
              PUSH      DI

              MOV       BX,ES:[DI]
              MOV       BH,00H

              MOV       CX,DS:[SI-01H]
              MOV       CH,00H
              PUSH      CX
      AGAIN:
              POP       CX
              POP       DI
              INC       DI
              POP       SI

              PUSH      SI
              PUSH      DI
              PUSH      CX
        REPZ  CMPSB
              CMP       CX,0
              JZ        FOUND
              SUB       BL,01H
              JZ        NOTFOUND
              JMP       AGAIN
      FOUND:  MOV       DX,OFFSET FOUNDSTRING
              MOV       AH,09H
              INT       21H
              JMP       ENDPRO
   NOTFOUND:  MOV       DX,OFFSET NOTFOUNDSTRING
              MOV       AH,09H
              INT       21H
     ENDPRO:  MOV       AX,4C00H
              INT       21H
        CODE  ENDS
              END       START
