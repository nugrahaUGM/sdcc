/*-------------------------------------------------------------------------
   gptrget1.c - get 1 byte value from generic pointer

   Copyright (C) 1999, Sandeep Dutta . sandeep.dutta@usa.net
   Adopted for pic16 port by Vangelis Rokas, 2004 <vrokas AT otenet.gr>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2.1, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

/* the return value is expected to be in WREG, therefore we choose return
 * type void here. Generic pointer is expected to be in WREG:PRODL:FSR0L,
 * so function arguments are void, too */

extern POSTINC0;
extern POSTINC1;
extern PREINC1;
extern INDF0;
extern FSR0L;
extern FSR0H;
extern WREG;
extern TBLPTRL;
extern TBLPTRH;
extern TBLPTRU;
extern TABLAT;
extern PRODL;


void _gptrget1(void) __naked
{
  __asm
    /* decode generic pointer MSB (in WREG) bits 6 and 7:
     * 00 -> code
     * 01 -> EEPROM
     * 10 -> data
     * 11 -> unimplemented
     */
    btfss	_WREG, 7
    bra		_lab_01_
    
    ; data pointer
    ; data are already in FSR0
    movff	_PRODL, _FSR0H    

    movf	_POSTINC0, w

    return
    

_lab_01_:
    ; code or eeprom
    btfsc	_WREG, 6
    bra		_lab_02_
    
    ; code pointer
    movff	_FSR0L, _TBLPTRL
    movff	_PRODL, _TBLPTRH
    movwf	_TBLPTRU
    
    tblrd*+
    
    ; result in WREG
    movf	_TABLAT, w
    
    return 
  
_lab_02_:
    ; EEPROM pointer

    ; unimplemented yet

_end_:

  return
  __endasm;
}
