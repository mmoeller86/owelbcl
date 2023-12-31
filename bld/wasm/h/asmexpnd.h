/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#if defined( _STANDALONE_ )

extern bool ExpandAllConsts( token_buffer *tokbuf, token_idx start_pos, bool early_only );
extern bool ExpandProcString( token_buffer *tokbuf, token_idx index, bool *expanded );
extern bool ExpandSymbol( token_buffer *tokbuf, token_idx i, bool early_only, bool *expanded );
extern void AddTokens( token_buffer *tokbuf, token_idx start, token_idx count );
extern bool DefineConstant( token_buffer *tokbuf, token_idx i, bool redefine, bool expand_early );
extern bool StoreConstant( const char *name, const char *value, bool redefine );
extern bool StoreConstantNumber( const char *name, long value, bool redefine );
extern void MakeConstantUnderscored( const char * );

extern bool ExpandTheConstant( token_buffer *tokbuf, token_idx start_pos, bool early_only, bool flag_msg );

#endif

extern bool ExpandTheWorld( token_buffer *tokbuf, token_idx start_pos, bool early_only, bool flag_msg );
