/****************************************************************************
*
*                            Open Watcom Project
*
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


#include "wlib.h"
#include "ar.h"
#include "convert.h"
#include "roundmac.h"

#include "clibext.h"


static void AllocFNameTab( const char *name, libfile io, arch_header *arch )
/**************************************************************************/
{
    MemFree( arch->fnametab );
    GetFileContents( name, io, arch, &arch->fnametab );
}

static void AllocFFNameTab( const char *name, libfile io, arch_header *arch )
/***************************************************************************/
{
    MemFree( arch->ffnametab );
    GetFileContents( name, io, arch, &arch->ffnametab );
    arch->nextffname = arch->ffnametab;
    arch->lastffname = arch->nextffname + arch->size;
}


void LibWalk( libfile io, const char *name, void (*rtn)( arch_header *, libfile io ) )
/************************************************************************************/
{
    ar_header           ar;
    arch_header         arch;
    size_t              bytes_read;
//    int                 dict_count;
    long                pos;

//    dict_count = 0;
    arch.fnametab = NULL;
    arch.ffnametab = NULL;
    while( (bytes_read = LibRead( io, &ar, AR_HEADER_SIZE )) != 0 ) {
        if( bytes_read != AR_HEADER_SIZE ) {
            BadLibrary( name );
        }
        if( strncmp( ar.header_ident, AR_HEADER_IDENT, AR_HEADER_IDENT_LEN ) ) {
            BadLibrary( name );
        }
        GetARHeaderValues( &ar, &arch );
        pos = LibTell( io );
        if( ar.name[0] == '/' && ar.name[1] == ' ' && ar.name[2] == ' ' ) {
            // Ignore symbol table.
/*
            dict_count++;
            if( dict_count == 2 ) {
                error = readDict( &arch );
            } else {
                error = MoveAheadFrom( &arch );
                updateNewArchive( &arch );
            }
*/
        } else if( ar.name[0] == '/' && ar.name[1] == '/' && ar.name[2] == ' ' ) {
            AllocFNameTab( name, io, &arch );
        } else if( ar.name[0] == '/' && ar.name[1] == '/' && ar.name[2] == '/' ) {
            AllocFFNameTab( name, io, &arch );
        } else {
            arch.name = GetARName( io, &ar, &arch );
            arch.ffname = GetFFName( &arch );
            rtn( &arch, io );
            MemFree( arch.name );
            MemFree( arch.ffname );
        }
        arch.size = __ROUND_UP_SIZE_EVEN( arch.size );
        LibSeek( io, pos + arch.size, SEEK_SET );
    }
    MemFree( arch.fnametab );
    MemFree( arch.ffnametab );
}

void OMFLibWalk( libfile io, char *name, void (*rtn)( arch_header *arch, libfile io ) )
/*************************************************************************************/
{
    unsigned_16     pagelen;
    long            offset;
    arch_header     arch;
    char            buff[MAX_IMPORT_STRING];
    unsigned_8      len;
    unsigned_16     rec_len;
    unsigned_8      type;

    if( LibRead( io, &type, sizeof( type ) ) != sizeof( type ) )
        return; // nyi - FALSE?
    if( LibRead( io, &rec_len, sizeof( rec_len ) ) != sizeof( rec_len ) )
        return;
    pagelen = GET_LE_16( rec_len );
    offset = pagelen;
    pagelen += 3;
    if( Options.page_size == 0 ) {
        Options.page_size = pagelen;
    }
    LibSeek( io, offset, SEEK_CUR );
    NewArchHeader( &arch, name );
    offset = LibTell( io );
    while( LibRead( io, &type, sizeof( type ) ) == sizeof( type ) && ( type == CMD_THEADR ) ) {
        LibSeek( io, 2, SEEK_CUR );
        if( LibRead( io, &len, sizeof( len ) ) != sizeof( len ) )
            break;
        if( LibRead( io, buff, len ) != len )
            break;
        buff[len] = '\0';
        arch.name = buff;
        LibSeek( io, offset, SEEK_SET );
        rtn( &arch, io );
        offset = LibTell( io );
        offset = __ROUND_UP_SIZE( offset, pagelen );
        LibSeek( io, offset, SEEK_SET );
    }
}
