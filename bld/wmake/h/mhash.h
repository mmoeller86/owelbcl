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
* Description:  mhash.c interfaces
*
****************************************************************************/


#ifndef _MHASH_H
#define _MHASH_H    1

typedef UINT16  HASH_T;

typedef NODE                HASHNODE;
typedef struct HashTable    HASHTAB;

struct HashTable {
    HASH_T      prime;
    HASHNODE    *nodes[1];
};

extern HASH_T   Hash( const char *name, HASH_T prime );

extern HASHTAB  *NewHashTab( HASH_T prime );
extern void     AddHashNode( HASHTAB *tab, HASHNODE *node );
extern bool     WalkHashTab( HASHTAB *tab, bool (*func)(void *node,void *ptr), void *ptr );
extern void     FreeHashTab( HASHTAB *tab );
extern HASHNODE *FindHashNode( HASHTAB *tab, const char *name, case_sensitivity caseSensitive );
extern HASHNODE *RemHashNode( HASHTAB *tab, const char *name, case_sensitivity caseSensitive );

#endif
