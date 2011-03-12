
#include <OSD.ixx>

#ifdef WNT

//---------------------------- Windows NT System --------------------------------

#define STRICT
#ifdef NOUSER
#undef NOUSER
#endif
#include <windows.h>

#include <OSD_Exception_ACCESS_VIOLATION.hxx>
#include <OSD_Exception_ARRAY_BOUNDS_EXCEEDED.hxx>
#include <OSD_Exception_ILLEGAL_INSTRUCTION.hxx>
#include <OSD_Exception_IN_PAGE_ERROR.hxx>
#include <OSD_Exception_INT_DIVIDE_BY_ZERO.hxx>
#include <OSD_Exception_INT_OVERFLOW.hxx>
#include <OSD_Exception_INVALID_DISPOSITION.hxx>
#include <OSD_Exception_NONCONTINUABLE_EXCEPTION.hxx>
#include <OSD_Exception_PRIV_INSTRUCTION.hxx>
#include <OSD_Exception_STACK_OVERFLOW.hxx>
#include <OSD_Exception_STATUS_NO_MEMORY.hxx>
#include <OSD_Exception_CTRL_BREAK.hxx>

#include <OSD_Environment.hxx>
#include <Standard_Underflow.hxx>
#include <Standard_DivideByZero.hxx>
#include <Standard_Overflow.hxx>
#include <Standard_ProgramError.hxx>
#include <Standard_Mutex.hxx>

#include <OSD_WNT_1.hxx>

#ifdef _MSC_VER
#include <eh.h>
#include <malloc.h>
#endif

#include <process.h>
#include <signal.h>
#include <float.h>

static Standard_Boolean fMsgBox;
static Standard_Boolean fFltExceptions;
static Standard_Boolean fDbgLoaded;
static Standard_Boolean fCtrlBrk;

static LONG __fastcall _osd_raise ( DWORD, LPTSTR );
static BOOL WINAPI     _osd_ctrl_break_handler ( DWORD );

extern "C" Standard_EXPORT LONG _osd_debug   ( void );

/*  Roman Lygin, October 16, 2008
    Detailed description of this file implementation.

    Functions in this file are used to process exceptions and system signals and
    convert them into C++ exceptions, inheriting Standard_Failure.

    To install this mapping, the OSD::SetSignal() method be called in the user's
    code. Refer to its documentation for more details.

    SetSignal() sets up 3 entry points that can be used by the operating system:
    1. SigWntHandler() - will only be used if user calls ::raise() function with
       signal type set in OSD::SetSignal() - SIGSEGV, SIGFPE, SIGILL (the latter
       will likely be removed in the future)
    2. OSD::WntHandler() - will be used when user's code is compiled with /EHs
       option and unless user sets his own exception handler with 
       ::SetUnhandledExceptionFilter().
    3. TranslateSE() - will be used when user's code is compiled with /EHa
       option and unless user sets his own structured exception (SE) translator
       with _se_set_translator().

    All three functions above call CallHandler() which forms a textual message
    depending on the type of arrived exception/signal. If the environment
    variable CSF_DEBUG_MODE is defined, then the message box will pop up and
    ask the user if he wants to continue.
    Eventually CallHandler() calls _osd_raise() that creates C++ exceptions
    inheriting Standard_Failure and throws them (Standard_Failure::Raise()).

    There is also a special handler for Ctrl-C and Ctrl-Break keystrokes that
    simply ignores them.

    The functions are thread-safe and are protected with a mutex. Signals and
    exceptions are asyncronous and can arrive from within different threads.

    NOTE: The code should be simplified and made consisten with Unix 
    implementation (e.g. supported signals, enforced/loose signal handler
    setting depending on whether user-defined handlers were already set;
    removed message box, etc).
*/

MB_DESC fatalErrorDesc[] = {
									  
                 { MBT_ICON,   ( int )IDI_HAND              },
                 { MBT_BUTTON, IDYES,    TEXT( "Continue" ) },
                 { MBT_BUTTON, IDNO,     TEXT( "Debugger" ) },
                 { MBT_BUTTON, IDCANCEL, TEXT( "Stop"     ) }

};

static LONG CallHandler (DWORD, ptrdiff_t, ptrdiff_t);
static void SIGWntHandler (int, int);

static Standard_Mutex TheMutex; //used to forbid simultaneous execution of setting/executing handlers

//# define _OSD_FPX ( _EM_INVALID | _EM_DENORMAL | _EM_ZERODIVIDE | _EM_OVERFLOW | _EM_UNDERFLOW )
# define _OSD_FPX ( _EM_INVALID | _EM_DENORMAL | _EM_ZERODIVIDE | _EM_OVERFLOW )

//============================================================================
//==== WntHandler
//============================================================================

Standard_Integer OSD :: WntHandler ( const Standard_Address exceptionInfo )
{

 LPEXCEPTION_POINTERS lpXP = ( LPEXCEPTION_POINTERS )exceptionInfo;
 DWORD                dwExceptionCode = lpXP -> ExceptionRecord -> ExceptionCode;

// cout << "WntHandler " << dwExceptionCode << " " << lpXP->ExceptionRecord->ExceptionInformation[1]
//      << " " <<lpXP->ExceptionRecord->ExceptionInformation[0] << endl ;

 return CallHandler( dwExceptionCode ,
                     lpXP -> ExceptionRecord -> ExceptionInformation[ 1 ] ,
                     lpXP -> ExceptionRecord -> ExceptionInformation[ 0 ] ) ;

}

//============================================================================
//==== SIGWntHandler
//============================================================================


static void SIGWntHandler(int signum , int sub_code ) {

#if !defined(__CYGWIN32__) && !defined(__MINGW32__)
    Standard_Mutex::Sentry aSentry (TheMutex); //lock the mutex to prevent simultaneous handling
    switch( signum ) {
    case SIGFPE :
      if ( signal( signum , ( void (*)(int) ) &SIGWntHandler ) == SIG_ERR )
            cout << "signal error" << endl ;
          switch( sub_code ) {
          case _FPE_INVALID :
            CallHandler( EXCEPTION_FLT_INVALID_OPERATION ,0,0) ;
            break ;
          case _FPE_DENORMAL :
            CallHandler( EXCEPTION_FLT_DENORMAL_OPERAND ,0,0) ;
            break ;
          case _FPE_ZERODIVIDE :
            CallHandler( EXCEPTION_FLT_DIVIDE_BY_ZERO ,0,0) ;
            break ;
          case _FPE_OVERFLOW :
            CallHandler( EXCEPTION_FLT_OVERFLOW ,0,0) ;
            break ;
          case _FPE_UNDERFLOW :
            CallHandler( EXCEPTION_FLT_UNDERFLOW ,0,0) ;
            break ;
          case _FPE_INEXACT :
            CallHandler( EXCEPTION_FLT_INEXACT_RESULT ,0,0) ;
            break ;
          default:
            cout << "SIGWntHandler(default) -> Standard_NumericError::Raise(\"Floating Point Error\");"
                 << endl ;
            Standard_NumericError::Raise("Floating Point Error");
            break ;
          }
          break ;
	case SIGSEGV :
      if ( signal( signum , ( void (*)(int) )  &SIGWntHandler ) == SIG_ERR )
            cout << "signal error" << endl ;
          CallHandler( EXCEPTION_ACCESS_VIOLATION ,0,0) ;
	  break ;
    case SIGILL :
      if ( signal( signum , ( void (*)(int) )  &SIGWntHandler ) == SIG_ERR )
            cout << "signal error" << endl ;
          CallHandler( EXCEPTION_ILLEGAL_INSTRUCTION ,0,0) ;
	  break ;
        default:
          cout << "SIGWntHandler unexpected signal : "
               << signum << endl ;
          break ;
	}
// cout << "return from SIGWntHandler -> DebugBreak " << endl ;
 DebugBreak ();

#endif

}

//============================================================================
//==== CallHandler 
//============================================================================

static LONG CallHandler (DWORD dwExceptionCode ,
			 ptrdiff_t ExceptionInformation1 ,
			 ptrdiff_t ExceptionInformation0)
{

#if !defined(__CYGWIN32__) && !defined(__MINGW32__)

 Standard_Mutex::Sentry aSentry (TheMutex); //lock the mutex to prevent simultaneous handling
 static TCHAR         buffer[ 2048 ];
 int                  flterr = 0;

 buffer[0] = '\0' ;

// cout << "CallHandler " << dwExceptionCode << endl ;
 switch ( dwExceptionCode ) {
 
  case EXCEPTION_FLT_DENORMAL_OPERAND:
//      cout << "CallHandler : EXCEPTION_FLT_DENORMAL_OPERAND:" << endl ;
      lstrcpy (  buffer, TEXT( "FLT DENORMAL OPERAND" )  );
      flterr = 1 ;
      break ;
  case EXCEPTION_FLT_DIVIDE_BY_ZERO:
//      cout << "CallHandler : EXCEPTION_FLT_DIVIDE_BY_ZERO:" << endl ;
      lstrcpy (  buffer, TEXT( "FLT DIVIDE BY ZERO" )  );
      flterr = 1 ;
      break ;
  case EXCEPTION_FLT_INEXACT_RESULT:
//      cout << "CallHandler : EXCEPTION_FLT_INEXACT_RESULT:" << endl ;
      lstrcpy (  buffer, TEXT( "FLT INEXACT RESULT" )  );
      flterr = 1 ;
      break ;
  case EXCEPTION_FLT_INVALID_OPERATION:
//      cout << "CallHandler : EXCEPTION_FLT_INVALID_OPERATION:" << endl ;
      lstrcpy (  buffer, TEXT( "FLT INVALID OPERATION" )  );
      flterr = 1 ;
      break ;
  case EXCEPTION_FLT_OVERFLOW:
//      cout << "CallHandler : EXCEPTION_FLT_OVERFLOW:" << endl ;
      lstrcpy (  buffer, TEXT( "FLT OVERFLOW" )  );
      flterr = 1 ;
      break ;
  case EXCEPTION_FLT_STACK_CHECK:
//      cout << "CallHandler : EXCEPTION_FLT_STACK_CHECK:" << endl ;
      lstrcpy (  buffer, TEXT( "FLT STACK CHECK" )  );
      flterr = 1 ;
      break ;
  case EXCEPTION_FLT_UNDERFLOW:
//      cout << "CallHandler : EXCEPTION_FLT_UNDERFLOW:" << endl ;
      lstrcpy (  buffer, TEXT( "FLT UNDERFLOW" )  );
      flterr = 1 ;
      break ;
  case STATUS_FLOAT_MULTIPLE_TRAPS:
//      cout << "CallHandler : EXCEPTION_FLT_UNDERFLOW:" << endl ;
      lstrcpy (  buffer, TEXT( "FLT MULTIPLE TRAPS (possible overflow in conversion of double to integer)" )  );
      flterr = 1 ;
      break ;
  case STATUS_FLOAT_MULTIPLE_FAULTS:
//      cout << "CallHandler : EXCEPTION_FLT_UNDERFLOW:" << endl ;
      lstrcpy (  buffer, TEXT( "FLT MULTIPLE FAULTS" )  );
      flterr = 1 ;
      break ;

  case STATUS_NO_MEMORY:
//      cout << "CallHandler : STATUS_NO_MEMORY:" << endl ;
      OSD_Exception_STATUS_NO_MEMORY ::
      Raise (  TEXT( "MEMORY ALLOCATION ERROR ( no room in the process heap )" )  );

  case EXCEPTION_ACCESS_VIOLATION:
//      cout << "CallHandler : EXCEPTION_ACCESS_VIOLATION:" << endl ;
      wsprintf ( buffer, TEXT( "%s%s%s0x%.8p%s%s%s" ), TEXT( "ACCESS VIOLATION" ),
                 fMsgBox ? "\n" : " ", TEXT( "at address " ),
                 ExceptionInformation1 ,
                 TEXT( " during '" ),
                 ExceptionInformation0 ? TEXT( "WRITE" ) : TEXT( "READ" ),
                 TEXT( "' operation" ));
  break;

  case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
//      cout << "CallHandler : EXCEPTION_ARRAY_BOUNDS_EXCEEDED:" << endl ;
      lstrcpy (  buffer, TEXT( "ARRAY BOUNDS EXCEEDED" )  );
  break;

  case EXCEPTION_DATATYPE_MISALIGNMENT:
//      cout << "CallHandler : EXCEPTION_DATATYPE_MISALIGNMENT:" << endl ;
      lstrcpy (  buffer, TEXT( "DATATYPE MISALIGNMENT" )  );
  break;

  case EXCEPTION_ILLEGAL_INSTRUCTION:
//      cout << "CallHandler : EXCEPTION_ILLEGAL_INSTRUCTION:" << endl ;
      lstrcpy (  buffer, TEXT( "ILLEGAL INSTRUCTION" )  );
  break;

  case EXCEPTION_IN_PAGE_ERROR:
//      cout << "CallHandler : EXCEPTION_IN_PAGE_ERROR:" << endl ;
      lstrcpy (  buffer, TEXT( "IN_PAGE ERROR" )  );
  break;

  case EXCEPTION_INT_DIVIDE_BY_ZERO:
//      cout << "CallHandler : EXCEPTION_INT_DIVIDE_BY_ZERO:" << endl ;
      lstrcpy (  buffer, TEXT( "INTEGER DIVISION BY ZERO" )  );
  break;

  case EXCEPTION_INT_OVERFLOW:
//      cout << "CallHandler : EXCEPTION_INT_OVERFLOW:" << endl ;
      lstrcpy (  buffer, TEXT( "INTEGER OVERFLOW" )  );
  break;

  case EXCEPTION_INVALID_DISPOSITION:
//      cout << "CallHandler : EXCEPTION_INVALID_DISPOSITION:" << endl ;
      lstrcpy (  buffer, TEXT( "INVALID DISPOSITION" )  );
  break;

  case EXCEPTION_NONCONTINUABLE_EXCEPTION:
//      cout << "CallHandler : EXCEPTION_NONCONTINUABLE_EXCEPTION:" << endl ;
      lstrcpy (  buffer, TEXT( "NONCONTINUABLE EXCEPTION" )  );
  break;

  case EXCEPTION_PRIV_INSTRUCTION:
//      cout << "CallHandler : EXCEPTION_PRIV_INSTRUCTION:" << endl ;
      lstrcpy (  buffer, TEXT( "PRIVELEGED INSTRUCTION ENCOUNTERED" )  );
  break;

  case EXCEPTION_STACK_OVERFLOW:
//      cout << "CallHandler : EXCEPTION_STACK_OVERFLOW:" << endl ;
#if defined( _MSC_VER ) && ( _MSC_VER >= 1300 )
    // try recovering from stack overflow: available in MS VC++ 7.0
    if (!_resetstkoflw())
      lstrcpy (  buffer, TEXT( "Unrecoverable STACK OVERFLOW" )  );
    else
#endif
      lstrcpy (  buffer, TEXT( "STACK OVERFLOW" )  );
  break;
 
  default:
    wsprintf( buffer, TEXT("unknown exception code 0x%x, params 0x%p 0x%p"),
	      dwExceptionCode, ExceptionInformation1, ExceptionInformation0 );

 }  // end switch

 int idx = lstrlen ( buffer );


 if ( idx && fMsgBox && dwExceptionCode != EXCEPTION_NONCONTINUABLE_EXCEPTION ) {
     //reset FP operations before message box, otherwise it may fail to show up
    _fpreset () ;
    _clearfp() ;

  MessageBeep ( MB_ICONHAND );
  int msgID = MsgBox ( NULL, buffer, TEXT( "Error detected" ), 4, fatalErrorDesc );
//  cout << "flterr" << flterr << " fFltExceptions " << fFltExceptions << endl ;
  if ( flterr ) {
    if ( !fFltExceptions )
      return EXCEPTION_EXECUTE_HANDLER;
    _fpreset () ;
    _clearfp() ;
    _controlfp ( 0, _OSD_FPX ) ;          // JR add :
//    cout << "OSD::WntHandler _controlfp( 0, _OSD_FPX ) " << hex << _controlfp(0,0) << dec << endl ;
  }
  buffer[ idx ] = 0;
  switch ( msgID ) {
   case IDYES: {
    PTCHAR ptr = _tcschr (  buffer, TEXT( '\n' )  );
    if ( ptr != NULL )
      *ptr = TEXT( ' ' );
//    cout << "CallHandler " << dwExceptionCode << endl ;
    _osd_raise ( dwExceptionCode, buffer );
   }  // IDYES
   case IDNO:
    LONG action ;
    action = _osd_debug ();
//    cout << "return from CallHandler -> DebugBreak " << endl ;
    DebugBreak ();
    _osd_raise ( dwExceptionCode, buffer );
//    cout << "CallHandler return : " << action << endl ;
    return action ;
   case IDCANCEL:
    exit ( 0xFFFF );
   }  // end switch
 }
 else {
   if ( flterr ) {
     if ( !fFltExceptions )
       return EXCEPTION_EXECUTE_HANDLER;
     _fpreset () ;
     _clearfp() ;
     _controlfp ( 0, _OSD_FPX ) ;          // JR add :
//     cout << "OSD::WntHandler _controlfp( 0, _OSD_FPX ) " << hex << _controlfp(0,0) << dec << endl ;
   }
 }

 return _osd_raise ( dwExceptionCode, buffer );

#else
 return 0;
#endif

}  // end OSD :: WntHandler

//=======================================================================
//function : TranslateSE
//purpose  : Translate Structural Exceptions into C++ exceptions
//           Will be used when user's code is compiled with /EHa option
//=======================================================================

#ifdef _MSC_VER
static void TranslateSE( unsigned int theCode, EXCEPTION_POINTERS* theExcPtr )
{
  Standard_Mutex::Sentry aSentry (TheMutex); //lock the mutex to prevent simultaneous handling
  ptrdiff_t info1 = 0, info0 = 0;
  if ( theExcPtr ) {
    info1 = theExcPtr->ExceptionRecord->ExceptionInformation[1];
    info0 = theExcPtr->ExceptionRecord->ExceptionInformation[0];
  }
  CallHandler(theCode, info1, info0);
}
#endif

//============================================================================
//==== SetSignal 
//============================================================================

#ifdef _MSC_VER
// MSV 31.08.2005
// If we compile this file under MSVC 7.1 with the default options for 
// exception handling (/GX or /EHsc) then the following warning is issued:
//   warning C4535: calling _set_se_translator() requires /EHa
// Till now all worked with the default options, and there was no difference
// found in exception handling behaviour between /EHa and /EHs options. 
// So, here we disable the warning, and leave the default compiler options.
// If some reason appears to turn to /EHa option this pragma can be removed.
#pragma warning (disable:4535)
// Roman Lygin 15.10.2008 - continuation of above note by MSV.
// Open CASCADE itself is compiled by default with /EHs (or /GX) and MSVC
// complains about _set_se_translator(). However it is correctly inserted
// and used when user's code compiled with /EHa.
#endif

// Roman Lygin 15.10.2008
/*! <b>Windows-specific notes:<\b>

    As of Open CASCADE 6.3.x OSD::SetSignal() on Windows (compiled with MS VC++)
    sets 3 main handlers:
    \li Signal handlers (via ::signal() functions) that translate system signals
        (SIGSEGV, SIGFPE, SIGILL) into C++ exceptions (classes inheriting
        Standard_Failure). They only be called if user calls ::raise() function
        with one of supported signal type set.
    \li Exception handler OSD::WntHandler() (via ::SetUnhandledExceptionFilter())
        that will be used when user's code is compiled with /EHs option.
    \li Structured exception (SE) translator (via _set_se_translator()) that
        translates SE exceptions (aka asynchronous exceptions) into the 
        C++ exceptions inheriting Standard_Failure. This translator will be
        used when user's code is compiled with /EHa option.

    This approach ensures that regardless of the option the user chooses to
    compile his code with (/EHs or /EHa), signals (or SE exceptions) will be
    translated into Open CASCADE C++ exceptions.

    If \a aFloatingSignal is TRUE then floating point exceptions will be
    generated in accordance with the mask
    <tt>_EM_INVALID | _EM_DENORMAL | _EM_ZERODIVIDE | _EM_OVERFLOW<\tt> that is
    used to call ::_controlfp() system function. If \a aFloatingSignal is FALSE
    corresponding operations (e.g. division by zero) will gracefully complete
    without an exception.

    The OSD::SetUseSETranslator() method used until Open CASCADE 6.3 is removed.

    <b>Unix-specific notes:<\b>
    OSD::SetSignal() sets handlers (via ::sigaction()) for multiple signals
    (SIGFPE, SIGSEGV, etc). Currently the number of handled signals is much
    greater than for Windows, in the future this may change to provide better
    consistency with Windows.

    \a aFloatingSignal is recognized on Sun Solaris, Linux, and SGI Irix to
    generate floating-point exception according to the mask
    <tt>FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW</tt> (in Linux conventions).

    When compiled with OBJS macro defined, already set signal handlers (e.g.
    by Data Base Managers) are not redefined.

    <b>Common notes:<\b>

    If OSD::SetSignal() method is used in at least one thread, it must also be
    called in any other thread where Open CASCADE will be used, to ensure
    consistency of behavior. Its \aFloatingSignal argument must be consistent
    across threads.

    Keep in mind that whether the C++ exception will really be thrown (i.e.
    ::throw() will be called) is regulated by the NO_CXX_EXCEPTIONS and
    OCC_CONVERT_SIGNALS macros used during compilation of Open CASCADE and
    user's code. Refer to Foundation Classes User's Guide for further details.
*/
void OSD :: SetSignal ( const Standard_Boolean aFloatingSignal ) {

#if !defined(__CYGWIN32__) && !defined(__MINGW32__)

  Standard_Mutex::Sentry aSentry (TheMutex); //lock the mutex to prevent simultaneous handling
  LPTOP_LEVEL_EXCEPTION_FILTER aPreviousFilter ;

  OSD_Environment         env (  TEXT( "CSF_DEBUG_MODE" )  );
  TCollection_AsciiString val;

  val = env.Value ();

  if (  !env.Failed ()  ) {
    cout << "Environment variable CSF_DEBUG_MODE setted." << endl ;
    fMsgBox = Standard_True;
  }
  else {
    fMsgBox = Standard_False;
  }

  //Set exception handler (ignored when running under debugger). It will be used in most cases
  //when user's code is compiled with /EHs
  //Replaces the existing top-level exception filter for all existing and all future threads
  //in the calling process
  aPreviousFilter =
   ::SetUnhandledExceptionFilter ((LPTOP_LEVEL_EXCEPTION_FILTER)&OSD::WntHandler);

  //Signal handlers will only be used when the method ::raise() will be used
  //Handlers must be set for every thread
  if ( signal( SIGSEGV , ( void (*)(int) ) &SIGWntHandler ) == SIG_ERR )
    cout << "signal(OSD::SetSignal) error" << endl ;
  if ( signal( SIGFPE , ( void (*)(int) ) &SIGWntHandler ) == SIG_ERR )
    cout << "signal(OSD::SetSignal) error" << endl ;
  if ( signal( SIGILL , ( void (*)(int) ) &SIGWntHandler ) == SIG_ERR )
    cout << "signal(OSD::SetSignal) error" << endl ;

  //Set Ctrl-C and Ctrl-Break handler
  fCtrlBrk = Standard_False;
  SetConsoleCtrlHandler ( &_osd_ctrl_break_handler, TRUE );

#ifdef _MSC_VER
  _se_translator_function pOldSeFunc = _set_se_translator( TranslateSE );
#endif

  fFltExceptions = aFloatingSignal;
  if ( aFloatingSignal ) {
    _controlfp ( 0, _OSD_FPX );          // JR add :
  }
  else {
    _controlfp ( _OSD_FPX , _OSD_FPX );          // JR add :
  }

#endif

}  // end OSD :: SetSignal

//============================================================================
//==== ControlBreak 
//============================================================================

void OSD :: ControlBreak () {

 if ( fCtrlBrk ) {
 
  fCtrlBrk = Standard_False;
  OSD_Exception_CTRL_BREAK :: Raise (  TEXT( "*** INTERRUPT ***" )  );
 
 }  // end if

}  // end OSD :: ControlBreak

//============================================================================
//==== _osd_ctrl_break_handler 
//============================================================================

static BOOL WINAPI _osd_ctrl_break_handler ( DWORD dwCode ) {

 if ( dwCode == CTRL_C_EVENT || dwCode == CTRL_BREAK_EVENT ) {

  MessageBeep ( MB_ICONEXCLAMATION );
  fCtrlBrk = Standard_True;

 } else

  exit ( 254 );

 return TRUE;

}  // end _osd_ctrl_break_handler

//============================================================================
//==== _osd_raise 
//============================================================================

static LONG __fastcall _osd_raise ( DWORD dwCode, LPTSTR msg ) 
{
  if (  msg[ 0 ] == TEXT( '\x03' )  ) ++msg;

  switch ( dwCode ) {
    
  case EXCEPTION_ACCESS_VIOLATION: OSD_Exception_ACCESS_VIOLATION :: Raise ( msg ); break;
  case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: OSD_Exception_ARRAY_BOUNDS_EXCEEDED :: Raise ( msg ); break;
  case EXCEPTION_DATATYPE_MISALIGNMENT: Standard_ProgramError :: Raise ( msg ); break;
  case EXCEPTION_ILLEGAL_INSTRUCTION: OSD_Exception_ILLEGAL_INSTRUCTION :: Raise ( msg ); break;
  case EXCEPTION_IN_PAGE_ERROR: OSD_Exception_IN_PAGE_ERROR :: Raise ( msg ); break;
  case EXCEPTION_INT_DIVIDE_BY_ZERO: Standard_DivideByZero :: Raise ( msg );  break;
  case EXCEPTION_INT_OVERFLOW: OSD_Exception_INT_OVERFLOW :: Raise ( msg ); break;
  case EXCEPTION_INVALID_DISPOSITION: OSD_Exception_INVALID_DISPOSITION :: Raise ( msg ); break;
  case EXCEPTION_NONCONTINUABLE_EXCEPTION: OSD_Exception_NONCONTINUABLE_EXCEPTION :: Raise ( msg ); break;
  case EXCEPTION_PRIV_INSTRUCTION: OSD_Exception_PRIV_INSTRUCTION :: Raise ( msg ); break;
  case EXCEPTION_STACK_OVERFLOW: OSD_Exception_STACK_OVERFLOW :: Raise ( msg ); break;
  case EXCEPTION_FLT_DIVIDE_BY_ZERO: Standard_DivideByZero :: Raise ( msg ); break;
  case EXCEPTION_FLT_STACK_CHECK:
  case EXCEPTION_FLT_OVERFLOW:
    Standard_Overflow :: Raise ( msg ); break;
  case EXCEPTION_FLT_UNDERFLOW: Standard_Underflow :: Raise ( msg ); break;
  case EXCEPTION_FLT_INVALID_OPERATION:
  case EXCEPTION_FLT_DENORMAL_OPERAND:
  case EXCEPTION_FLT_INEXACT_RESULT:
  case STATUS_FLOAT_MULTIPLE_TRAPS:
  case STATUS_FLOAT_MULTIPLE_FAULTS:
    Standard_NumericError :: Raise ( msg ); break;
  default:
    break;
  }  // end switch

  return EXCEPTION_EXECUTE_HANDLER;

}  // end _osd_raise

//============================================================================
//==== _osd_debug 
//============================================================================

#if defined(__CYGWIN32__) || defined(__MINGW32__)
#define __try
#define __finally
#define __leave return 0
#endif

LONG _osd_debug ( void ) {

 LONG action ;

 if ( !fDbgLoaded ) {
 
  HKEY                hKey;
  HANDLE              hEvent = INVALID_HANDLE_VALUE;
  DWORD               dwKeyType;
  DWORD               dwValueLen;
  TCHAR               keyValue[ MAX_PATH ];
  TCHAR               cmdLine[ MAX_PATH ];
  SECURITY_ATTRIBUTES sa;
  PROCESS_INFORMATION pi;
  STARTUPINFO         si;

  __try {
  
   if (  RegOpenKey (
          HKEY_LOCAL_MACHINE,
          TEXT( "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug" ),
          &hKey
         ) != ERROR_SUCCESS
   ) __leave;

   dwValueLen = sizeof ( keyValue );
  
   if (  RegQueryValueEx (
          hKey, TEXT( "Debugger" ), NULL, &dwKeyType, ( unsigned char* )keyValue, &dwValueLen
         ) != ERROR_SUCCESS
   ) __leave;

   sa.nLength              = sizeof ( SECURITY_ATTRIBUTES );
   sa.lpSecurityDescriptor = NULL;
   sa.bInheritHandle       = TRUE;

   if (   (  hEvent = CreateEvent ( &sa, TRUE, FALSE, NULL )  ) == NULL   ) __leave;

   wsprintf (  cmdLine, keyValue, GetCurrentProcessId (), hEvent  );

   ZeroMemory (  &si, sizeof ( STARTUPINFO )  );

   si.cb      = sizeof ( STARTUPINFO );
   si.dwFlags = STARTF_FORCEONFEEDBACK;

//   cout << "_osd_debug -> CreateProcess" << endl ;
   if (  !CreateProcess (
           NULL, cmdLine, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE,
           NULL, NULL, &si, &pi
          )
   ) __leave;

//   cout << "_osd_debug -> WaitForSingleObject " << endl ;
   WaitForSingleObject ( hEvent, INFINITE );
//   cout << "_osd_debug <- WaitForSingleObject -> CloseHandle " << endl ;

   CloseHandle ( pi.hProcess );
   CloseHandle ( pi.hThread  );

//   cout << "_osd_debug fDbgLoaded  " << endl ;
   fDbgLoaded = TRUE;
  
  }  // end __try

  __finally {
  
//   cout << "_osd_debug -> CloseHandle(hKey) " << endl ;
   if ( hKey   != INVALID_HANDLE_VALUE ) CloseHandle ( hKey   );
//   cout << "_osd_debug -> CloseHandle(hEvent) " << endl ;
   if ( hEvent != INVALID_HANDLE_VALUE ) CloseHandle ( hEvent );
//   cout << "_osd_debug end __finally " << endl ;
  
  }  // end __finally

 }  /* end if */

 action = fDbgLoaded ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_EXECUTE_HANDLER;
// cout << "_osd_debug return " << action << " EXCEPTION_CONTINUE_EXECUTION("
//      << EXCEPTION_CONTINUE_EXECUTION << ")" << endl ;
 return action ;

}  // end _osd_debug

#if defined(__CYGWIN32__) || defined(__MINGW32__)
#undef __try
#undef __finally
#undef __leave
#endif

// Must be there for compatibility with UNIX system code ----------------------

//void OSD::Handler(const OSD_Signals aSig,
//                  const OSD_Signals aCode){}
void OSD::Handler(const OSD_Signals theSignal,
		  const Standard_Address theSigInfo,
		  const Standard_Address theContext) {}

void OSD::SegvHandler(const OSD_Signals aSig,
                      const Standard_Address code,
                      const Standard_Address scp){}

#endif // WNT
