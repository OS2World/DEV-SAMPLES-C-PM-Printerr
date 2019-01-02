
#define  INCL_GPI
#define  INCL_WIN
#include <os2.h>
#include <stdlib.h>
#include <string.h>

#define     PRIVATE     0
#define     STRSIZE     512
#define     STR1OFFSET  0
#define     STR2OFFSET  500

/**************************************************************
 *  printerr:   uses the WinGetErrInfo API to retrieve
 *              error info from the last error in an anchor
 *              block.  Then displays the error message
 *              contained in the error info as well as the
 *              line number and file name of offending program.
 *
 **************************************************************/
VOID far pascal _saveregs _loadds printerr ( PSZ pszFilename
                                             , USHORT usLineNo, HAB hab )
{
    PERRINFO perr;                      // for WinGetErrorInfo
    NPBYTE     npb;                     // temporary pointer
    PBYTE      pb;                      // points to err message
    SEL        sel;                     // we allocate memory for string
    PSZ        pszStr1, pszStr2;        // temporary string pointers

  // allocate memory for our temporary strings

    if (DosAllocSeg ( STRSIZE, &sel, PRIVATE ) )
    {
        WinMessageBox (   HWND_DESKTOP
                        , HWND_DESKTOP
                        , "Unable to allocate in printerr"
                        , "Error"
                        , 0
                        , MB_OK | MB_ICONEXCLAMATION | MB_MOVEABLE );
        return;
    }

  // We allocate 512 bytes and then use it as two strings:
  //    pszStr1 -  first 500 bytes
  //    pszStr2 -  last  12  bytes

    pszStr1 = MAKEP ( sel, STR1OFFSET   );
    pszStr2 = MAKEP ( sel, STR2OFFSET );

  // retrieve a structure containing error info
    perr = WinGetErrorInfo (hab);

  // point to the error message string in the structure
    pb = MAKEP ( SELECTOROF(perr), perr->offaoffszMsg );
    npb = (NPBYTE) *pb;
    pb = MAKEP (SELECTOROF(perr), npb );

  // format the pszStr1 with a string containing line # and filename
    strcpy ( pszStr1, "Error in line " );
    itoa   ( usLineNo, pszStr2, 10 );
    strcat ( pszStr1, pszStr2 );
    strcat ( pszStr1, " of file " );
    strcat ( pszStr1, pszFilename );

  // display the strings in a message box
    WinMessageBox ( HWND_DESKTOP
                    , HWND_DESKTOP
                    , pb               // error message
                    , pszStr1          // line # and filename
                    , 0
                    , MB_OK | MB_ICONEXCLAMATION | MB_MOVEABLE );

    WinFreeErrorInfo (hab);
    DosFreeSeg ( sel );
}
