;*****************************************************************************
;*
;*                            Open Watcom Project
;*
;* Copyright (c) 2002-2022 The Open Watcom Contributors. All Rights Reserved.
;*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
;*
;*  ========================================================================
;*
;*    This file contains Original Code and/or Modifications of Original
;*    Code as defined in and that are subject to the Sybase Open Watcom
;*    Public License version 1.0 (the 'License'). You may not use this file
;*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
;*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
;*    provided with the Original Code and Modifications, and is also
;*    available at www.sybase.com/developer/opensource.
;*
;*    The Original Code and all software distributed under the License are
;*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
;*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
;*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
;*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
;*    NON-INFRINGEMENT. Please see the License for the specific language
;*    governing rights and limitations under the License.
;*
;*  ========================================================================
;*
;* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
;*               DESCRIBE IT HERE!
;*
;*****************************************************************************


;
;       386 DOS Extender Equates for __Extender variable
;
X_ERGO                  equ     0       ; Ergo OS/386 (unsupported)
X_RATIONAL              equ     1       ; DOS/4G(W) or compatible
X_PHARLAP_V1            equ     2       ; PharLap 386 | DOS
X_PHARLAP_V2            equ     3
X_PHARLAP_V3            equ     4
X_PHARLAP_V4            equ     5
X_PHARLAP_V5            equ     6
X_PHARLAP_V6            equ     7       ; PharLap TNT
X_PHARLAP_V7            equ     8
X_PHARLAP_V8            equ     9
X_INTEL                 equ     10      ; Intel CodeBuilder (unsupported)
X_WIN386                equ     11      ; Watcom Win386
X_HX                    equ     12      ; HX DOS Extender
;
;       386 DOS Extender Equates for __ExtenderSubtype variable
;
XS_NORMAL               equ     0

XS_PHARLAP_NORMAL       equ     0
XS_PHARLAP_FUJITSU      equ     1

XS_RATIONAL_ZEROBASE    equ     0
XS_RATIONAL_NONZEROBASE equ     1
