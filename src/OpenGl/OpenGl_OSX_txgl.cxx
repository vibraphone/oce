#include <OpenGl_tgl_all.hxx>
#include <OpenGl_telem_util.hxx>
#include <stdio.h>

#include <OpenGl_Memory.hxx>

#include <OpenGl_txgl.hxx>

int call_util_osd_getenv( char * , char * , int ) ;

typedef NCollection_DataMap<Tint, GLCONTEXT> GLContextMap;

static GLContextMap _Txgl_Map;


static  int BackDitherProp = False;     /* Dithering pour le background */
static  int DitherProp = True;          /* Dithering pour le trace  */
static  GLXContext previous_ctx = 0;    /* Use for share display list */
static  GLXContext dead_ctx;            /* Context to be destroyed */
static  Display *dead_dpy;              /* Display associated with dead_ctx */

/*----------------------------------------------------------------------*/
/*
* Constantes
*/

#define NO_TRACE

#define CALL_DEF_STRING_LENGTH 132

#define WIN_HTBL_SIZE 23

/*----------------------------------------------------------------------*/
/*
* Fonctions statiques
*/

static GLvoid printVisualInfo( Display *, XVisualInfo *glxVisual );

WINDOW TxglCreateWindow(DISPLAY  *disp, WINDOW par,
                        Tint x, Tint y, Tint w, Tint h, Tint bw,
                        Tfloat bgcolr, Tfloat bgcolg, Tfloat bgcolb )
  {
    GLCONTEXT ctx;
    static int sdesc[11];
    Colormap cmap;
    XVisualInfo* vis=NULL;
    /*    XVisualInfo tmplt;*/
    XSetWindowAttributes cwa;
    XColor color;
    /*    Tint i, n, nret;*/
    Tint n;
    Tint scr;
    int value;
    char string[CALL_DEF_STRING_LENGTH];
    int DBuffer = True;
    XWindowAttributes wattr;

    WINDOW win;

    unsigned long mask = 0;
    /*    unsigned long background_pixel = 0;*/

    if (call_util_osd_getenv("CALL_OPENGL_NO_DBF", string, CALL_DEF_STRING_LENGTH))
      DBuffer    = False;

    if (call_util_osd_getenv("JWR_PIXMAP_DB", string, CALL_DEF_STRING_LENGTH))
      TelSetPixmapDB(1);

    XGetWindowAttributes( disp , par , &wattr );

    n = 0;
    sdesc[n] = GLX_RGBA;n++;

    sdesc[n] = GLX_DEPTH_SIZE;n++;
    sdesc[n] = 1;n++;

    sdesc[n] = GLX_RED_SIZE;n++;
    sdesc[n] = (wattr.depth <= 8) ? 0 : 1;n++;

    sdesc[n] = GLX_GREEN_SIZE;n++;
    sdesc[n] = (wattr.depth <= 8) ? 0 : 1;n++;

    sdesc[n] = GLX_BLUE_SIZE;n++;
    sdesc[n] = (wattr.depth <= 8) ? 0 : 1;n++;

    if (DBuffer) {
      sdesc[n] = GLX_DOUBLEBUFFER;n++;
    }

    sdesc[n] = None;n++;

    scr = DefaultScreen( disp );

#if defined(__linux) || defined(Linux)
    {
      XVisualInfo vinfo;
      int ninfo;
      unsigned long vmask = VisualIDMask |  VisualScreenMask;
      vinfo.visualid = wattr.visual->visualid;
      vinfo.screen = DefaultScreen( disp );
      vis = XGetVisualInfo( disp, vmask, &vinfo, &ninfo);
    }
#endif

    if( !vis )
      vis = glXChooseVisual( disp, scr, sdesc );
    if( !vis) return TFailure;

#ifdef TRACE
    printf ("TxglCreateWindow \n");
    printf ("Informations sur le visual\n");
    printf ("par visualid %x%x %d\n", wattr.visual->visualid, wattr.visual->visualid);
    printf ("vis visualid 0x%x %d\n", vis->visualid, vis->visualid);
    printf ("vis depth %d\n", vis->depth);
    printf ("vis class %d\n", vis->class);
    printf ("vis red_mask %ld\n", vis->red_mask);
    printf ("vis green_mask %ld\n", vis->green_mask);
    printf ("vis blue_mask %ld\n", vis->blue_mask);
    printf ("vis colormap_size %d\n", vis->colormap_size);
    printf ("vis bits_per_rgb %d\n", vis->bits_per_rgb);
    printVisualInfo( disp, vis );
#endif

    /*
    * Le BackDitherProp est utilise pour le clear du background
    * Pour eviter une difference de couleurs avec la couleur choisie
    * par l'application (XWindow) il faut desactiver le dithering
    * au dessus de 8 plans.
    * 
    * Pour le DitherProp:
    * On cherchera a activer le Dithering que si le Visual a au moins
    * 8 plans pour le GLX_RED_SIZE. Le test est plus sur car on peut
    * avoir une profondeur superieure a 12 mais avoir besoin du dithering.
    * (Carte Impact avec GLX_RED_SIZE a 5 par exemple)
    */

    glXGetConfig( disp, vis, GLX_RED_SIZE, &value );

    if ( value < 8 ) {
      DitherProp = True;
    }
    else
    {
      DitherProp = False;
    }

    if ( vis->depth <= 8 ) {
      BackDitherProp = True;
    }
    else
    {
      BackDitherProp = False;
    }

#ifdef TRACE
    printf("Dithering %d BackDithering %d \n",DitherProp,BackDitherProp);
#endif

    if (call_util_osd_getenv ("CALL_OPENGL_NO_DITHER", string, CALL_DEF_STRING_LENGTH))
      DitherProp = False;

    if (call_util_osd_getenv ("CALL_OPENGL_NO_BACKDITHER", string, CALL_DEF_STRING_LENGTH))
      BackDitherProp = False;

    if (dead_ctx) {
      /* recover display lists from dead_ctx, then destroy it */
      ctx = glXCreateContext( disp, vis, dead_ctx, GL_TRUE );
      glXDestroyContext(dead_dpy, dead_ctx);
      dead_ctx = 0;
    } else if (previous_ctx == 0) {
      ctx = glXCreateContext( disp, vis, NULL, GL_TRUE );
    } else {
      /* ctx est une copie du previous */
      ctx = glXCreateContext( disp, vis, previous_ctx, GL_TRUE );
    }
    previous_ctx = ctx;

    if( !ctx) return TFailure;

    cmap = XCreateColormap( disp,  par, vis->visual, AllocNone );

    color.red = (unsigned short) (bgcolr * 0xFFFF);
    color.green = (unsigned short) (bgcolg * 0xFFFF);
    color.blue  = (unsigned short) (bgcolb * 0xFFFF);
    color.flags = DoRed | DoGreen | DoBlue;
    XAllocColor( disp, cmap, &color );

    cwa.colormap  = cmap;
    cwa.event_mask  = StructureNotifyMask;
    cwa.border_pixel  = color.pixel;
    cwa.background_pixel = color.pixel;

    mask = CWBackPixel | CWColormap | CWBorderPixel | CWEventMask;

    if( vis->visualid == wattr.visual->visualid ) {
      win = par;
    } 
    else 
    {
      win = XCreateWindow( disp, par, x, y, w, h, bw,
        vis->depth, InputOutput, vis->visual,
        mask, &cwa );
    }

#ifdef TRACE
    printf ("TxglCreateWindow win %x par %x \n", win, par);
#endif

    XSetWindowBackground( disp, win, cwa.background_pixel );
    XClearWindow( disp, win );

    /* if in Pixmap double buffering mode, set up pixmap */

    if (TelTestPixmapDB())
    {
      GC gc;
      Pixmap pixmap;
      GLXPixmap glxpixmap;

      printf("setting up pixmap double buffering\n");

      gc = XCreateGC(disp, win, 0, NULL);

      pixmap = XCreatePixmap(disp, win, w, h, vis->depth);

      glxpixmap = glXCreateGLXPixmap(disp, vis, pixmap);

      glXMakeCurrent(disp, glxpixmap, ctx);

      glDrawBuffer(GL_FRONT);

      TelSetPixmapDBParams(disp, win, w, h, vis->depth, gc, pixmap, glxpixmap, ctx);
    }

    XFree((char*)vis);  

    _Txgl_Map.Bind( (Tint)win, ctx );

    return win;
  }

  WINDOW
    TxglSetWindow( DISPLAY  *disp, WINDOW par, GLCONTEXT ctx)
  {
    XVisualInfo* vis;
    char string[CALL_DEF_STRING_LENGTH];
    XWindowAttributes wattr;

    XGetWindowAttributes( disp , par , &wattr );
    {
      unsigned long vmask = VisualIDMask |  VisualScreenMask;
      XVisualInfo vinfo;
      int ninfo;
      vinfo.visualid = wattr.visual->visualid;
      vinfo.screen = DefaultScreen( disp );
      vis = XGetVisualInfo( disp, vmask, &vinfo, &ninfo);
    }

    if( !vis) return TFailure;

#ifdef TRACE
    printf ("TxglSetWindow \n");
    printf ("Informations sur le visual\n");
    printf ("par visualid %x%x %d\n", wattr.visual->visualid, wattr.visual->visualid);
    printf ("vis visualid 0x%x %d\n", vis->visualid, vis->visualid);
    printf ("vis depth %d\n", vis->depth);
    printf ("vis class %d\n", vis->class);
    printf ("vis red_mask %ld\n", vis->red_mask);
    printf ("vis green_mask %ld\n", vis->green_mask);
    printf ("vis blue_mask %ld\n", vis->blue_mask);
    printf ("vis colormap_size %d\n", vis->colormap_size);
    printf ("vis bits_per_rgb %d\n", vis->bits_per_rgb);
    printVisualInfo( disp, vis );
#endif

    /*
    * Le BackDitherProp est utilise pour le clear du background
    * Pour eviter une difference de couleurs avec la couleur choisie
    * par l'application (XWindow) il faut desactiver le dithering
    * au dessus de 8 plans.
    * 
    * Pour le DitherProp:
    * On cherchera a activer le Dithering que si le Visual a au moins
    * 8 plans pour le GLX_RED_SIZE. Le test est plus sur car on peut
    * avoir une profondeur superieure a 12 mais avoir besoin du dithering.
    * (Carte Impact avec GLX_RED_SIZE a 5 par exemple)
    */

    {
      int value;
      glXGetConfig( disp, vis, GLX_RED_SIZE, &value );

      if ( value < 8 ) {
        DitherProp = True;
      } else {
        DitherProp = False;
      }

      if ( vis->depth <= 8 ) {
        BackDitherProp = True;
      } else {
        BackDitherProp = False;
      }
    }

#ifdef TRACE
    printf("Dithering %d BackDithering %d \n",DitherProp,BackDitherProp);
#endif

    if (call_util_osd_getenv ("CALL_OPENGL_NO_DITHER", string, CALL_DEF_STRING_LENGTH))
      DitherProp = False;

    if (call_util_osd_getenv ("CALL_OPENGL_NO_BACKDITHER", string, CALL_DEF_STRING_LENGTH))
      BackDitherProp = False;

    previous_ctx = ctx;

    XFree((char*)vis);  

    _Txgl_Map.Bind( ( Tint )par, ctx );

    return par;

  }

  /*----------------------------------------------------------------------*/

  TStatus
    TxglWinset( DISPLAY *disp, WINDOW win )
  {

    Bool  i;
    GLXContext ctx;
    GLenum errorcode;
    const GLubyte *errorstring;

    if ( !_Txgl_Map.IsBound( win ) )
      return TFailure;

    ctx = _Txgl_Map.ChangeFind( win );

#ifdef TRACE
    printf ("TxglWinset::glXMakeCurrent %x \n", win);
#endif
    if (TelTestPixmapDB())
    {
      i = glXMakeCurrent(disp, TelGetGLXPixmap(), ctx);
    }
    else
    {
      i = glXMakeCurrent(disp, win, ctx);  /* TRUE/FALSE */
    }
    if (!i)
    {
      errorcode = glGetError();
      errorstring = gluErrorString(errorcode);
      printf("glXMakeCurrent failed: %d %s\n", errorcode, errorstring);
    }

    return  i == True ? TSuccess : TFailure;
  }

  /*----------------------------------------------------------------------*/

  WINDOW TxglGetSubWindow( DISPLAY *disp, WINDOW win )
  {

    Window root, parent, *child, w;
    unsigned int num;

    if( XQueryTree( disp, win, &root, &parent, &child, &num ) )
    {
      if (! num) return win;
      w = child[0];
      XFree( (char *)child );
      return w;
    }
    else
      return 0;

  }

  void TxglDestroyWindow( DISPLAY *disp, WINDOW win )
  {

    GLXContext ctx;
    Tint dummy;

    if ( !_Txgl_Map.IsBound(win) )
      return;
    ctx = _Txgl_Map.ChangeFind(win);

    /* FSXXX sync necessary if non-direct rendering */
    glXWaitGL();

    _Txgl_Map.UnBind( win );

    if (previous_ctx == ctx) {
      /* san -- OCC12977: it's important to put some valid GLXContext or null into
      previous_ctx here, otherwise next glxCreateContext() will crash on some ATI Radeon cards
      */
      previous_ctx = NULL;
      if ( _Txgl_Map.Size() > 0 ) {
        GLContextMap::Iterator it(_Txgl_Map);
        previous_ctx = it.Value();
      }
      /*
      * if this is the last remaining context, do not destroy it yet, to avoid
      * losing any shared display lists (fonts...)
      */
      if (previous_ctx) {
        glXDestroyContext(disp, ctx);
      } else {
        dead_ctx = ctx;
        dead_dpy = disp;
      }
    } else {
      glXDestroyContext(disp, ctx);
    }

  }

  /*----------------------------------------------------------------------*/

  int TxglGetDither(void) {
    return DitherProp;
  }

  /*----------------------------------------------------------------------*/

  int TxglGetBackDither(void) {
    return BackDitherProp;
  }


  GLCONTEXT TxglGetContext( WINDOW win ) {
    GLCONTEXT ctx = NULL;
    if ( _Txgl_Map.IsBound( (Tint) win ) )
      ctx = _Txgl_Map.Find( win );
    return ctx;  
  }


  enum { ZERO = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN };

  /* Unused :*/
#ifdef TRACE
  static const char* className( int class ) {
    static char      *classes[] = {
      "StaticGray",
      "GrayScale",
      "StaticColor",
      "PseudoColor",
      "TrueColor",
      "DirectColor",
    };

    if ( class < ZERO || class > FIVE )
      return "unknown";
    else
      return classes[class];
  }

  static GLvoid printVisualInfo( DISPLAY  *display, XVisualInfo *glxVisual ) {

#define TrueFalse(x)  ( x ? "True" : "False" )

    int        tmp;

    printf("\n" );
    printf("   X Visual Information ...\n\n" );
    printf("\tvisualid : 0x%x\n", glxVisual->visualid );
    printf("\tclass    : %s\n\n", className( glxVisual->class ) );

    glXGetConfig(display, glxVisual, GLX_USE_GL, &tmp);
    printf( "\tSupport GL ( GLX_USE_GL ) : %s\n", TrueFalse(tmp) );

    glXGetConfig(display, glxVisual, GLX_LEVEL, &tmp);
    printf( "\tFramebuffer ( GLX_LEVEL ) : %s\n\n",
      (tmp < ZERO) ? "Underlay" : (tmp == ZERO ? "Normal"  : tmp > ONE ? "Overlay"  : "Popup") );

    glXGetConfig(display, glxVisual, GLX_BUFFER_SIZE, &tmp);
    printf( "\tFramebuffer depth ( GLX_BUFFER_SIZE )     : %d\n", tmp );

    glXGetConfig(display, glxVisual, GLX_DOUBLEBUFFER, &tmp);
    printf( "\tDoublebuffer ( GLX_DOUBLEBUFFER )         : %s\n",
      TrueFalse(tmp) );

    glXGetConfig(display, glxVisual, GLX_DEPTH_SIZE, &tmp);
    printf( "\tDepth buffer depth ( GLX_DEPTH_SIZE )     : %d\n", tmp );

    glXGetConfig(display, glxVisual, GLX_STENCIL_SIZE, &tmp);
    printf( "\tStencil buffer depth ( GLX_STENCIL_SIZE ) : %d\n", tmp );

    glXGetConfig(display, glxVisual, GLX_STEREO, &tmp);
    printf( "\tStereo Buffer ( GLX_STEREO )              : %s\n",
      TrueFalse(tmp) );

    glXGetConfig(display, glxVisual, GLX_AUX_BUFFERS, &tmp);
    printf( "\tAuxillary Buffers ( GLX_AUX_BUFFERS)      : %d\n\n", tmp );

    glXGetConfig(display, glxVisual, GLX_RGBA, &tmp);
    printf( "\tColor mode ( GLX_RGBA )       : %s\n", tmp ? "RGBA" :
    "Color Index" );

    glXGetConfig(display, glxVisual, GLX_RED_SIZE, &tmp);
    printf( "\tRed Bits ( GLX_RED_SIZE )     : %d\n", tmp );

    glXGetConfig(display, glxVisual, GLX_GREEN_SIZE, &tmp);
    printf( "\tGreen Bits ( GLX_GREEN_SIZE ) : %d\n", tmp );

    glXGetConfig(display, glxVisual, GLX_BLUE_SIZE, &tmp);
    printf( "\tBlue Bits ( GLX_BLUE_SIZE )   : %d\n", tmp );

    glXGetConfig(display, glxVisual, GLX_ALPHA_SIZE, &tmp);
    printf( "\tAlpha Bits ( GLX_ALPHA_SIZE ) : %d\n\n", tmp );

    glXGetConfig(display, glxVisual, GLX_ACCUM_RED_SIZE, &tmp);
    printf( "\tRed Accumulation Bits ( GLX_ACCUM_RED_SIZE )     : %d\n", tmp );

    glXGetConfig(display, glxVisual, GLX_ACCUM_GREEN_SIZE, &tmp);
    printf( "\tGreen Accumulation Bits ( GLX_ACCUM_GREEN_SIZE ) : %d\n", tmp );

    glXGetConfig(display, glxVisual, GLX_ACCUM_BLUE_SIZE, &tmp);
    printf( "\tBlue Accumulation Bits ( GLX_ACCUM_BLUE_SIZE )   : %d\n", tmp );

    glXGetConfig(display, glxVisual, GLX_ACCUM_ALPHA_SIZE, &tmp);
    printf( "\tAlpha Accumulation Bits ( GLX_ACCUM_ALPHA_SIZE ) : %d\n\n", tmp );
  }
#endif //TRACE
