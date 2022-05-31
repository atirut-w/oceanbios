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
boot_sig: .res 2

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
    .byte "Insert a bootable disk and restart the system", $0
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
    lda compname
    cmp #$ff
    beq noboot
    jsr isdrive
    cmp #0
    bne yesboot

    lda compname
    cmp #$ff
    beq noboot

    lda #0
    sta $246 ; Next component
    ; jsr bell
    bra next_component

yesboot:
    lda compid
    ldx compid+1
    sta $260
    stx $261

    jsr ld_bootsector

    ; Boot signature check
    lda boot_sig
    cmp #$55
    bne noboot
    lda boot_sig+1
    cmp #$aa
    bne noboot

bootable:
    lda #<bootmsg
    ldx #>bootmsg
    jsr print
    lda compid
    jsr printhex
    lda compid+1
    jsr printhex
    lda #<dots
    ldx #>dots
    jsr print

    jsr boot

noboot:
    rts

searchmsg: .byte "Searching for bootable media...", $d, $0
bootmsg: .byte "Booting from ", $0
dots: .byte "...", $d, $0
.endproc

.proc boot
    ; Reset stack pointer
    ldx #0
    txs
    ; Jump to boot sector
    jmp $1000
.endproc

.proc ld_bootsector
    ; lda compid
    ; jsr printhex
    ; lda compid+1
    ; jsr printhex

    lda $262 ; Sector size in units of 256 bytes
    cmp #0
    beq nodrive ; Zero sector size = no drive mapped here

    lda #<$0001
    ldx #>$0001
    sta $266
    stx $267 ; Load sector 1

    ldy #0

load_part1:
    lda $263 ; R/W port
    sta $1000,y
    iny
    cpy #0
    bne load_part1

load_part2:
    lda $263 ; R/W port
    sta $1100,y
    iny
    cpy #0
    bne load_part2

    bra done

nodrive:
    rts

done:
    ; jsr bell
    ; lda #<loadedmsg
    ; ldx #>loadedmsg
    ; jsr print

    lda $11fe
    sta boot_sig
    lda $11ff
    sta boot_sig+1
    rts

; loadedmsg: .byte "Boot sector loaded", $d, $0
.endproc

.proc read_component
    ldy #0

read_name_char:
    lda $246
    sta compname,y
    cmp #$00
    beq read_name_done
    cmp #$ff
    beq done
    iny
    bra read_name_char

read_name_done:
    ldy #0

read_uuid_char:
    lda $246
    sta compid,y
    cpy #16
    beq done
    iny
    bra read_uuid_char

done:
    ; lda compid
    ; jsr printhex
    ; lda compid+1
    ; jsr printhex

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
