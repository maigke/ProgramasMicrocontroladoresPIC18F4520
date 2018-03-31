; -----------------------------------------------------------------------
; Plantilla generada por Piklab
    #include <p16f84a.inc>

; -----------------------------------------------------------------------
; Bits de configuración: adapte los parámetros a su necesidad
;    __CONFIG _RC_OSC & _WDT_ON & _PWRTE_OFF & _CP_OFF
    __CONFIG _HS_OSC & _WDT_OFF & _PWRTE_OFF & _CP_OFF    

; -----------------------------------------------------------------------
; Declaración de variables
INT_VAR UDATA 0x0C
w_saved      RES 1 ; variable utilizada para guardar contexto
status_saved RES 1 ; variable utilizada para guardar contexto

var1         RES 1 ; variable de ejemplo

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
    call retraso_0_01_seg
    goto    $              ; bucle infinito

    END
    
; Código de retardo generado por PikLoops (dom dic-2017-03 01:57:32)
; Tiempo de retardo = 0.00999960  con  Osc = 20.00000000MHz

retraso_0_01_seg:
	movlw	D'65'
	movwf	ContadorB
	movlw	D'237'
	movwf	ContadorA
retraso_0.01_seg_bucle
	decfsz	ContadorA,1
	goto	retraso_0.01_seg_bucle
	decfsz	ContadorB,1
	goto	retraso_0.01_seg_bucle
	return
    
