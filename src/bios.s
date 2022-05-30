SERIALTERM = $2ff;

.segment "ZEROPAGE"

tmp1: .res 1
tmp2: .res 1
tmp3: .res 1
tmp4: .res 1
ptr1: .res 2
ptr2: .res 2
ptr3: .res 2
ptr4: .res 2

compname: .res 16
compid: .res 16

.segment "ROM"

.proc main
    lda #0
    jsr putchar ; Reinitialize terminal

    lda #<boottext
    ldx #>boottext
    jsr print

    jsr tryboot

    lda #<noboot
    ldx #>noboot
    jsr print

    jsr hang

boottext:
    .byte "Ocean BIOS Copyright 2022 Atirut Wattanamongkol", $d, $d, $0
noboot:
    .byte $7
    .byte "No bootable media found", $d
    .byte "Please insert a bootable disk and restart the system", $0
.endproc

.proc dummy_handler
    rti
.endproc

.proc tryboot
    lda #<searchmsg
    ldx #>searchmsg
    jsr print

    lda #$ff
    sta $246 ; Reset component list cursor

next_component:
    jsr read_component
    jsr isdrive
    cmp #0
    bne yesboot

    lda compname
    cmp #$ff
    beq noboot

    lda #0
    sta $246 ; Next component
    bra next_component

yesboot:
    lda compid
    ldx compid+1
    sta $260
    stx $261

    jsr ld_bootsector
    rts

noboot:
    rts

searchmsg: .byte "Searching for bootable media...", $d, $0
.endproc

.proc ld_bootsector
    rts
.endproc

.proc read_component
    ldy #0

read_name_char:
    lda $246
    sta compname,y
    cpy #16
    beq read_name_done
    iny
    bra read_name_char

read_name_done:
    ldy #0

read_uuid_char:
    lda $247
    sta compid,y
    cpy #16
    beq done
    iny
    bra read_uuid_char

done:
    ; lda #<compname
    ; ldx #>compname
    ; jsr print
    ; lda #<newline
    ; ldx #>newline
    ; jsr print

    rts

newline: .byte $d, $0
.endproc

.proc isdrive
    lda #<drivetype
    ldx #>drivetype
    sta ptr1
    stx ptr1+1
    ldy #0

next:
    lda compname,y
    cmp drivetype,y
    bne no
    cpy #4
    beq yes
    iny
    bra next

no:
    lda #0
    rts
yes:
    lda #1
    rts

drivetype: .byte "drive", $0
.endproc

; Convenience subroutines

.proc print
    sta ptr1
    stx ptr1+1
    ldy #0

loop:
    lda (ptr1),y
    cmp #$0
    beq end
    sta SERIALTERM
    iny
    bra loop

end:
    rts

.endproc

.proc putchar
    sta SERIALTERM
    rts
.endproc

.proc printhex
    sta tmp1
    and #$f0
    
    lsr
    lsr
    lsr
    lsr

    tay
    lda lookup,y
    sta SERIALTERM

    lda tmp1
    and #$0f
    tay
    lda lookup,y
    sta SERIALTERM

    rts

lookup: .byte "0123456789abcdef"
.endproc

.proc bell
    lda #$7
    jsr putchar
    rts
.endproc

.proc hang
    bra hang
.endproc

.segment "VECTORS"
.addr dummy_handler
.addr main
.addr dummy_handler
