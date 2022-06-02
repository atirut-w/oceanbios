.export __STARTUP__
.import _main
.importzp sp

.proc __STARTUP__
    lda #$00
    ldx #$ef
    sta sp
    stx sp+1

    jsr _main
hang: jmp hang
.endproc

.proc interrupt
    rti
.endproc

.segment "VECTORS"

.addr interrupt
.addr _main
.addr interrupt
