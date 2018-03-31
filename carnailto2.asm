; -----------------------------------------------------------------------
; Plantilla generada por Piklab
    #include <p16f84.inc>

    ;LIST P = PIC16F84A

; -----------------------------------------------------------------------
; Bits de configuración: adapte los parámetros a su necesidad
    __CONFIG _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF

; -----------------------------------------------------------------------
; Declaración de variables
INT_VAR UDATA 0x0C
w_saved      RES 1 ; variable utilizada para guardar contexto
status_saved RES 1 ; variable utilizada para guardar contexto

var1         RES 1 ; variable de ejemplo

CBLOCK  0x20

ENDC
; -----------------------------------------------------------------------
; reiniciar vector
STARTUP CODE 0x000
    nop                    ; requerido para el depurado ICD2
    movlw   high start     ; cargar el byte superior de la etiqueta «start»
    movwf   PCLATH         ; inicializar PCLATH
    goto    start          ; ir al inicio del código principal

; vector de interrupciones
INT_VECTOR CODE 0x004
    goto    interrupt      ; ir al inicio del código de interrupción

; código reubicable
PROG CODE
interrupt
    movwf   w_saved        ; guardar contexto
    swapf   STATUS,w
    movwf   status_saved
    ; << agregue el código de interrupción >>
    swapf   status_saved,w ; restaurar contexto
    movwf   STATUS
    swapf   w_saved,f
    swapf   w_saved,w
    retfie

start
    ; << agregue el código principal >>

    call LCD_Inicializa
    movlw 'H'
    call LCD_Caracter
    movlw 'o'
    call LCD_Caracter
    sleep
    goto    $              ; bucle infinito

    #INCLUDE <lcd4bit.inc>
    #INCLUDE <retardos.inc>
    END
