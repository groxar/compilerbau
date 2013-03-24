/*
 * =====================================================================================
 *
 *       Filename:  stTestHash.c
 *
 *    Description:  Erwin
 *
 *        Version:  1.0
 *        Created:  03/22/2013 02:41:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include	<stdlib.h>
#include "symboltable.c"
#include "stdio.h"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
    int
main ( int argc, char *argv[] )
{

    symbol_t *a;
    a->name= "cs";
    symbol_t *b;
    b->type = 1;
    b->address = 0;
    b->name = "asdsa";
    HASH_ADD_INT( b, name, a);
    printf("%d", HASH_FIND_INT(b, ,
    return 0;
}				/* ----------  end of function main  ---------- */
