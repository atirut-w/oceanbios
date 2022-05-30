SERIALTERM = $2ff;

.segment "ZEROPAGE"

tmp1: .res 1
tmp2: .res 1
tmp3: .res 1
tmp4: .res 1

.segment "ROM"

.proc main
    lda #0
    jsr putchar
    lda #4
    jsr putchar

    lda #<text
    ldx #>text
    jsr print

    lda #7
    jsr putchar

    jsr hang

text:
    .byte "Custom BIOS test...", $d
    .byte "...newline, anyone? No idea why they don't use $0a lol.", 0
.endproc

.proc nmi
    rti
.endproc

.proc irq
    rti ; TODO: BIOS calls via IRQ
.endproc

; Other subroutines

.proc print
    sta tmp1
    stx tmp2
    ldy #0

loop:
    lda (tmp1),y
    cmp #$0
    beq end
    sta SERIALTERM
    iny
    bne loop

end:
    rts
.endproc

.proc putchar
    sta SERIALTERM
    rts
.endproc

.proc hang
    bra hang
.endproc

.segment "VECTORS"
.addr nmi
.addr main
.addr irq
