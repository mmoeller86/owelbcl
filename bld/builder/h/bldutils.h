/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2022 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Builder & Langdat utility functions.
*
****************************************************************************/


#include <stdio.h>
#include <stddef.h>
#include "bool.h"

#define MAX_LINE    (4096 + 1)

#define IS_BLANK(c) ((c)==' ' || (c)=='\t')

#define SKIP_BLANKS(p)  while( IS_BLANK( *(p) ) ) (p)++

extern FILE         *LogFile;

extern void         Fatal( const char *, ... );
extern void         Log( bool quiet, const char *, ... );
extern void         LogFlush( void );
extern void         OpenLog( const char * );
extern void         CloseLog( void );
extern const char   *SkipBlanks( const char * );
extern char         *GetPathOrFile( const char *p, char *buffer );
