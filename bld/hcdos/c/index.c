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
* Description:  InfoBench help file index creation.
*
****************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bool.h"
#include "hcdos.h"
#include "helpidx.h"
#include "index.h"


#define STR_CNT 2

#define PAGE_UNDEFINED  ((unsigned)-1)

static unsigned dataPageCnt;
static unsigned indexPageCnt;
static unsigned topicCnt;


static unsigned markDataPages( void )
{
    unsigned            pagenum;
    a_helpnode          *curnode;
    unsigned            size;
    unsigned            nodesize;

    size = sizeof( HelpPageHeader );
    pagenum = 0;
    topicCnt = 0;
    for( curnode = HelpNodes; curnode != NULL; curnode = curnode->next ) {
        topicCnt++;
        nodesize = sizeof( PageIndexEntry ) + strlen( curnode->name ) + 1;
        if( nodesize + size > HLP_PAGE_SIZE ) {
            pagenum++;
            size = sizeof( HelpPageHeader );
        }
        size += nodesize;
        curnode->data_page = pagenum;
    }
    return( pagenum + 1 );
}


static unsigned pagesInNextLevel( unsigned pages_in_this_level )
{
    unsigned            ret;

    ret = pages_in_this_level / INDEX_ENTRIES_PER_PAGE;
    if( pages_in_this_level % INDEX_ENTRIES_PER_PAGE != 0 )
        ret++;
    return( ret );
}


static unsigned calcIndexPages( unsigned pagecnt )
{
    unsigned    numpages;
    unsigned    next_level;

    numpages = 0;
    for( ;; ) {
        next_level = pagesInNextLevel( pagecnt );
        pagecnt = next_level;
        numpages += next_level;
        if( pagecnt == 1 ) {
            break;
        }
    }
    return( numpages );
}


static unsigned calcStringBlockSize( const char **helpstr )
{
    unsigned    blocksize;
    unsigned    i;

    blocksize = 0;
    if( helpstr != NULL ) {
        blocksize = ( STR_CNT + 1 ) * sizeof( uint_16 );
        for( i = 0; i < STR_CNT; i++ ) {
            if( helpstr[i] != NULL ) {
                blocksize += strlen( helpstr[i] ) + 1;
            }
        }
    }
    return( blocksize );
}


unsigned long CalcIndexSize( const char **helpstr )
{
    unsigned long       ret;

    dataPageCnt = markDataPages();
    indexPageCnt = calcIndexPages( dataPageCnt );
    ret = calcStringBlockSize( helpstr ) + ( dataPageCnt + indexPageCnt ) * HLP_PAGE_SIZE + dataPageCnt * sizeof( uint_16 );
    if( helpstr != NULL )
        return( ret + HELP_HEADER_SIZE );
    return( ret + HELP_HEADER_V1_SIZE );
}


static a_helpnode *findFirstPage( unsigned pagenum )
{
    a_helpnode          *curnode;

    for( curnode = HelpNodes; curnode != NULL && curnode->data_page + indexPageCnt != pagenum; curnode = curnode->next )
        ;
    return( curnode );
}


static unsigned countPageEntries( unsigned pagenum )
{
    unsigned            cnt;
    a_helpnode          *curnode;

    cnt = 0;
    for( curnode = findFirstPage( pagenum ); curnode != NULL && curnode->data_page + indexPageCnt == pagenum; curnode = curnode->next )
        cnt++;
    return( cnt );
}


static int writeOneDataPage( FILE *fout, unsigned pagenum )
{
    char                *pagedata;
    HelpPageHeader      *pagehdr;
    PageIndexEntry      *index;
    char                *strings;
    unsigned            stroff;
    unsigned            len;
    a_helpnode          *curnode;

    pagedata = malloc( HLP_PAGE_SIZE );
    memset( pagedata, 0, HLP_PAGE_SIZE );
    pagehdr = (HelpPageHeader *)pagedata;
    index = (PageIndexEntry *)( pagehdr + 1 );
    pagehdr->type = PAGE_DATA;
    pagehdr->num_entries = countPageEntries( pagenum );
    pagehdr->page_num = pagenum;
    strings = (char *)( index + pagehdr->num_entries );
    stroff = 0;
    for( curnode = findFirstPage( pagenum ); curnode != NULL && curnode->data_page + indexPageCnt == pagenum; curnode = curnode->next ) {
        index->name_offset = stroff;
        index->entry_offset = curnode->fpos;
        strcpy( strings, curnode->name );
        len = strlen( curnode->name ) + 1;
        stroff += len;
        strings += len;
        index++;
    }
    fwrite( pagedata, HLP_PAGE_SIZE, 1, fout );
    free( pagedata );
    return( 0 );
}


static int writeDataPages( FILE *fout )
{
    unsigned            i;

    for( i = 0; i < dataPageCnt; i++ ) {
        writeOneDataPage( fout, i + indexPageCnt );
    }
    return( 1 );
}


static char *getIndexString( void **pages, unsigned pagenum, bool datalevel )
{
    HelpIndexEntry      *entry;
    HelpPageHeader      *header;
    a_helpnode          *curnode;
    char                *name;

    if( datalevel ) {
        name = NULL;
        for( curnode = findFirstPage( pagenum ); curnode != NULL && curnode->data_page + indexPageCnt == pagenum; curnode = curnode->next )
            name = curnode->name;
        return( name );
    } else {
        header = pages[pagenum];
        entry = (HelpIndexEntry *)( header + 1 );
        return( entry[header->num_entries - 1].start );
    }
}


static void fillIndexPage( void **pages, unsigned curpage, unsigned pagenum, bool datalevel )
{
    HelpIndexEntry      *entry;
    HelpPageHeader      *header;
    unsigned            i;
    char                *src;

    header = pages[curpage];
    entry = (HelpIndexEntry *)( header + 1 );
    for( i = 0; i < header-> num_entries; i++ ) {
        entry->nextpage = pagenum;
        src = getIndexString( pages, pagenum, datalevel );
        strncpy( entry->start, src, INDEX_LEN );
        entry->start[INDEX_LEN - 1] = '\0';
        pagenum++;
        entry++;
    }
}


static void generateIndexLevel( void **pages, unsigned curbase,
                unsigned cur_level, unsigned prevbase, unsigned prev_level )
{
    unsigned            entries_per_page;
    unsigned            entries_last_page;
    unsigned            i;
    unsigned            pagenum;
    HelpPageHeader      *header;

    entries_per_page = prev_level / cur_level;
    if( prev_level % cur_level != 0 ) {
        entries_per_page ++;
    }
    entries_last_page = prev_level - entries_per_page * ( cur_level - 1 );
    if( prevbase == PAGE_UNDEFINED ) {
        pagenum = indexPageCnt;
    } else {
        pagenum = prevbase;
    }
    for( i = curbase; i < curbase + cur_level; i++ ) {
        pages[i] = malloc( HLP_PAGE_SIZE );
        memset( pages[i], 0, HLP_PAGE_SIZE );
    }
    /***********************************
     * for each B tree page in a level *
     ***********************************/
    for( i = curbase; i < curbase + cur_level; i++ ) {
        header = pages[i];
        header->type = PAGE_INDEX;
        if( i == curbase + cur_level - 1 ) {
            header->num_entries = entries_last_page;
        } else  {
            header->num_entries = entries_per_page;
        }
        header->page_num = i;
        fillIndexPage( pages, i, pagenum, prevbase == PAGE_UNDEFINED );
        pagenum += entries_per_page;
    }
}


static int writeIndexPages( FILE *fout )
{
    unsigned            prev_level;
    unsigned            cur_level;
    void                **pages;
    unsigned            curbase;
    unsigned            prevbase;
    unsigned            i;

    prev_level = dataPageCnt;
    prevbase = PAGE_UNDEFINED;
    pages = malloc( indexPageCnt * sizeof( void * ) );
    memset( pages, 0, indexPageCnt * sizeof( void * ) );
    curbase = indexPageCnt;

    /*************************
     * for each B tree level *
     *************************/
     for( ;; ) {
         cur_level = pagesInNextLevel( prev_level );
         curbase -= cur_level;
         generateIndexLevel( pages, curbase, cur_level, prevbase, prev_level );
         if( cur_level == 1 )
            break;
         prev_level = cur_level;
         prevbase = curbase;
     }
     for( i = 0; i < indexPageCnt; i++ ) {
         fwrite( pages[i], HLP_PAGE_SIZE, 1, fout );
         if( pages[i] != NULL ) {
             free( pages[i] );
         }
     }
     free( pages );
     return( 0 );
}


static int writePageItemNumIndex( FILE *fout )
{
    uint_16     *index;
    uint_16     indexsize;
    unsigned    i;

    indexsize = dataPageCnt * sizeof( uint_16 );
    index = malloc( indexsize );
    index[0] = 0; //countPageEntries( indexPageCnt );
    for( i=1; i < dataPageCnt; i++ ) {
        index[i] = index[i - 1] + countPageEntries( indexPageCnt + i - 1 );
    }
    fwrite( index, indexsize, 1, fout );
    free( index );
    return( 0 );
}


static int writeHeader( FILE *fout, const char **helpstr )
{
    uint_32     u32;
    uint_16     u16;

    u32 = HELP_SIG_1;
    fwrite( &u32, sizeof( u32 ), 1, fout );
    u32 = HELP_SIG_2;
    fwrite( &u32, sizeof( u32 ), 1, fout );
    if( helpstr != NULL ) {
        u16 = HELP_MAJ_VER;
    } else {
        u16 = HELP_MAJ_V1;
    }
    fwrite( &u16, sizeof( u16 ), 1, fout );
    u16 = HELP_MIN_VER;
    fwrite( &u16, sizeof( u16 ), 1, fout );
    u16 = indexPageCnt;
    fwrite( &u16, sizeof( u16 ), 1, fout );
    u16 = dataPageCnt;
    fwrite( &u16, sizeof( u16 ), 1, fout );
    u32 = topicCnt;
    fwrite( &u32, sizeof( u32 ), 1, fout );
    if( helpstr != NULL ) {
        u16 = calcStringBlockSize( helpstr );
        fwrite( &u16, sizeof( u16 ), 1, fout );
    }
    u16 = 3;
    u32 = 0;
    while( u16-- > 0 ) {
        fwrite( &u32, sizeof( u32 ), 1, fout );
    }
    return( 0 );
}

static void writeStrings( FILE *fout, const char **helpstr )
{
    unsigned    i;
    uint_16     tmp[STR_CNT + 1];

    if( helpstr != NULL ) {
        tmp[0] = STR_CNT;
        for( i = 0; i < STR_CNT; i++ ) {
            if( helpstr[i] != NULL ) {
                tmp[i + 1] = strlen( helpstr[i] ) + 1;
            } else {
                tmp[i + 1] = 0;
            }
        }
        fwrite( tmp, sizeof( uint_16 ) * ( STR_CNT + 1 ), 1, fout );
        for( i = 0; i < STR_CNT; i++ ) {
            if( helpstr[i] != NULL ) {
                fwrite( helpstr[i], tmp[i + 1], 1, fout );
            }
        }
    }
}

int WriteIndex( FILE *fout, const char **helpstr )
{
    fseek( fout, 0, SEEK_SET );
    writeHeader( fout, helpstr );
    writeStrings( fout, helpstr );
    writePageItemNumIndex( fout );
    writeIndexPages( fout );
    writeDataPages( fout );
    return( 0 );
}
