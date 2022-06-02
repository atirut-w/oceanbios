.export __STARTUP__
.import _main
.importzp sp

.proc __STARTUP__
    lda #$ff
    ldx #$2f
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
.addr __STARTUP__
.addr interrupt
