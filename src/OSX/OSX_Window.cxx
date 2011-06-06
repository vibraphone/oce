/*
 The content of this file is subject to the Open CASCADE Technology 
 Public License Version 6.2 (the "License"). You may not use the content 
 of this file except in compliance with the License. 
 Please obtain a copy of the License at http://www.opencascade.org and 
 read it completely before using this file. 
 
 The Initial Developer of the Original Code is Open CASCADE S.A.S., 
 with main offices at 15 bis, rue Ernest Renan 92136, Issy Les Moulineaux, France. 
 The Original Code is copyright © Open CASCADE S.A.S., 2001. All rights reserved. 
 
 Modifications to the Original Code have been made by Emmanuel Valentin. 
 Modifications are copyright © 2009. All rights reserved. 
 
 The software Open CASCADE Technology and all software distributed 
 under the License are distributed on an "AS IS" basis, without warranty of 
 any kind, and the Initial Developer hereby disclaims all such warranties, 
 including without limitation, any warranties of merchantability, fitness 
 for a particular purpose or non-infringement. Please see the License for the 
 specific terms and conditions governing rights and limitations under the License" 
 */
///

#if (defined(__MACH__) && defined(__APPLE__))
#include <Carbon/Carbon.h>
#include <OSX_Window.hxx>
#include <NSViewWrapper.h>

#include <stdio.h>
#include <Aspect_WindowError.hxx>


IMPLEMENT_STANDARD_HANDLE  (OSX_Window, Aspect_Window)
IMPLEMENT_STANDARD_RTTIEXT (OSX_Window, Aspect_Window)
//#include <Handle_OSX_Window.hxx>

enum {
	kOSXWindowClearArea	= 1
};


//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
OSX_Window::OSX_Window(const Handle(OSX_GraphicDevice)& aDevice,
                       const Standard_Address	          aView,
                       const Standard_Address	          aGLContext,
                       const Standard_Address	          aPixelFormat):Aspect_Window(aDevice), MyView(aView), MyGLContext(aGLContext){
	MyBackColor = NULL;
	MyOffscreenLayer = NULL;
	MyColorSpace = CGColorSpaceCreateWithName( kCGColorSpaceGenericRGB );

	Standard_Integer w,h;
	Size(w,h);
	command = (Standard_Integer*)malloc(sizeof(Standard_Integer));
	//MyOffscreenLayer =	CIContextCreateCGLayer(MyView, MyGLContext, aPixelFormat, CGSizeMake(w, h)); 
}  

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::Destroy () {
	if (MyOffscreenLayer)
		CGLayerRelease(MyOffscreenLayer);
	CGColorSpaceRelease(MyColorSpace);
	if (MyBackColor != NULL)
		CGColorRelease(MyBackColor);
	free(command);
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
OSX_Window::~OSX_Window() {
	Destroy();
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void*	OSX_Window::OSXWindow() {
	return MyView;
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void*	OSX_Window::GLContext() {
	return MyGLContext;
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::SetBackground(const Aspect_Background& Background) {
	SetBackground (Background.Color ().Name ());
} 

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::SetBackground( const Quantity_NameOfColor BackColor ) {
	if (MyBackColor != NULL)
		CGColorRelease( MyBackColor );
	
	MyBackground.SetColor( BackColor );
	
	CGFloat components[] = { MyBackground.Color().Red(), MyBackground.Color().Green(), MyBackground.Color().Blue(), 0.0 };
	MyBackColor = CGColorCreate(MyColorSpace, components);
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::SetBackground( const Quantity_Color& color ) {
	if (MyBackColor != NULL)
		CGColorRelease( MyBackColor );

	MyBackground.SetColor( color );
	
	CGFloat components[] = {MyBackground.Color().Red(),	MyBackground.Color().Green(),MyBackground.Color().Blue(),0.0F };

	MyBackColor = CGColorCreate(MyColorSpace, components);
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Standard_Boolean OSX_Window::SetBackground(const Standard_CString aName,const Aspect_FillMethod aMethod) {
	return Standard_False;
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::SetBackground(const Aspect_Handle aBitmap) {

}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::SetBackground(const Aspect_GradientBackground& aBitmap) {

}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::SetCursor ( const Aspect_Handle aCursor ) const {

} 

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::SetDoubleBuffer ( const Standard_Boolean DBmode ) {
	// not supported
} 

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Standard_Boolean OSX_Window::BackingStore() const {
	/*
	 Standard_Boolean retVal = Standard_False;;
	 HDC              hDC;
	 RECT             r;
	 
	 if ( myHPixmap )
	 
	 retVal = Standard_True;
	 
	 else {
	 
	 hDC = GetDC (  ( HWND )myHWindow  );
	 
	 GetClientRect (  ( HWND )myHWindow, &r  );
	 *(  ( Aspect_Handle* )&myHPixmap  ) = CreateCompatibleBitmap ( hDC, r.right, r.bottom );
	 
	 if ( myHPixmap ) {
	 
	 HDC hDCmem;
	 
	 hDCmem = CreateCompatibleDC ( hDC );
	 
	 SELECT_PALETTE( hDCmem );
	 
	 SelectObject ( hDCmem, myHPixmap );
	 FillRect (  hDCmem, &r, ( HBRUSH )MyHBackground  );
	 
	 UNSELECT_PALETTE( hDCmem );
	 
	 DeleteDC ( hDCmem );
	 
	 retVal = Standard_True;
	 
	 }  // end if
	 
	 ReleaseDC (  ( HWND )myHWindow, hDC  );
	 
	 }  // end else
	 */
	return Standard_False;
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Standard_Boolean OSX_Window::DoubleBuffer() const {
	return Standard_False;
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::Flush () const {
	// not supported
} 

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Standard_Boolean OSX_Window::IsMapped () const {
	return !NSViewIsHiddenOrHasHiddenAncestor( MyView );
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: Map () const {
	NSViewSetHidden( MyView, true );
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: Unmap () const {
	NSViewSetHidden( MyView, false );
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Aspect_TypeOfResize OSX_Window :: DoResize () const {
/*	if (CFGetRetainCount(MyOffscreenLayer)>0) {	// make sure off screen layer is resized.
		CGLayerRelease(MyOffscreenLayer);
	}*/

	// mac view has only one way to be resized
	return Aspect_TOR_RIGHT_AND_BOTTOM_BORDER;
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Standard_Boolean OSX_Window :: DoMapping () const {
	// DO nothing on MAC.
  return Standard_True;
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::Clear () const {
	HIRect rect;
	NSViewBounds(MyView, &rect);
	ClearArea( rect.origin.x + (rect.size.width / 2), rect.origin.y + (rect.size.height / 2) ,rect.size.width,rect.size.height);
	NSViewSetNeedsDisplay(MyView); 
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: ClearArea (
                    const Standard_Integer Xc,
                    const Standard_Integer Yc,
                    const Standard_Integer Width,
                    const Standard_Integer Height
                   ) const {
	if (MyOffscreenLayer == NULL)
		return;
	
	CGRect rect;
	
	rect.origin.x = Xc - (Width / 2);
	rect.origin.y = Yc - (Height / 2);
	rect.size.width = Width;
	rect.size.height = Height;

	CGContextRef context = CGLayerGetContext(MyOffscreenLayer);
//	CGContextSaveGState(context);
	CGContextSetRGBFillColor (context, 1, 1 , 0, 1);// 13

//	CGContextSetFillColorWithColor(context, MyBackColor);
//	CGContextSetFillColorWithColor(context, MyBackColor);
	CGContextFillRect (context, rect);
//	CGContextRestoreGState(context);
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window::Restore () const {
	HIRect rect;

	if (CFGetRetainCount(MySavedArea)>0) {
		NSViewBounds(MyView, &rect);
		RestoreArea(rect.origin.x + (rect.size.width / 2), rect.origin.y + (rect.size.height / 2) ,rect.size.width,rect.size.height);
	} 
}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: RestoreArea (
                    const Standard_Integer Xc,
                    const Standard_Integer Yc,
                    const Standard_Integer Width,
                    const Standard_Integer Height
                   ) const {

	if (CFGetRetainCount(MySavedArea)>0 && CFGetRetainCount(MyOffscreenLayer)>0) {
		CGContextRef context = CGLayerGetContext(MyOffscreenLayer);
		CGRect rect;
		rect.origin.x = Xc - (Width / 2);
		rect.origin.y = Yc - (Height / 2);
		rect.size.width = Width;
		rect.size.height = Height;
		CGContextDrawLayerInRect(context, rect, MySavedArea);
		CFRelease(context);
	}
} 

Standard_Boolean OSX_Window::Dump (const Standard_CString aFilename,
                 const Standard_Real aGammaValue ) const {

	HIRect rect;
	NSViewBounds(MyView, &rect);
	
	return DumpArea(aFilename, rect.origin.x + (rect.size.width / 2), rect.origin.y + (rect.size.height / 2) ,rect.size.width,rect.size.height, aGammaValue);
	
}  // end WNT_Window :: Dump
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Standard_Boolean OSX_Window::DumpArea(const Standard_CString aFilename, 
                                      const Standard_Integer Xc , 
                                      const Standard_Integer Yc, 
                                      const Standard_Integer Width, 
                                      const Standard_Integer Height, 
                                      const Standard_Real aGammaValue) const {

/* RECT             r;
 Standard_Integer x, y;

 GetClientRect (  ( HWND )myHWindow, &r  );

 x = Xc - Width  / 2;
 y = Yc - Height / 2;

 return ( myDoubleBuffer ) ?
         myImages -> SaveBuffer ( aFilename, x, y, Width, Height ) :
                 myImages -> Save               ( aFilename, x, y, Width, Height ); */
	return Standard_True;

}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Handle_Aspect_PixMap OSX_Window::ToPixMap() const {
}


//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Standard_Boolean OSX_Window :: Load ( const Standard_CString aFilename ) const {
/*
 RECT r;

 GetClientRect (  ( HWND )myHWindow, &r  );
 
 return LoadArea ( aFilename, r.right / 2, r.bottom / 2, r.right, r.bottom );
*/
	return Standard_False;
}
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Standard_Boolean OSX_Window :: LoadArea (
                                const Standard_CString aFilename,
                                const Standard_Integer Xc,
                                const Standard_Integer Yc,
                                const Standard_Integer Width,
                                const Standard_Integer Height
                               ) const {
/*
 Standard_Integer idx;
 Standard_Boolean status;

 status = (  idx = myImages -> Load ( aFilename )  ) ? Standard_True : Standard_False;

 if ( status ) myImages -> Draw ( idx, Xc, Yc, Width, Height );
*/
 return Standard_False;

}

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Quantity_Ratio OSX_Window :: Ratio () const {

	HIRect rect;
	NSViewBounds(MyView, &rect);
	return ( Quantity_Ratio )(  ( Quantity_Ratio )rect.size.width / ( Quantity_Ratio )rect.size.height );

} 

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: Position (
                    Quantity_Parameter& X1, Quantity_Parameter& Y1,
                    Quantity_Parameter& X2, Quantity_Parameter& Y2
                   ) const {

	void* parentView =  NSViewSuperview(MyView);
	if (parentView != NULL) {
		HIRect parentRect;
		HIRect rect;
		NSViewBounds(parentView, &parentRect);
		NSViewBounds(MyView, &rect);
		X1 = (Quantity_Parameter)rect.origin.x / (Quantity_Parameter)parentRect.size.width;
		X2 = (Quantity_Parameter)(rect.origin.x + rect.size.width) / (Quantity_Parameter)parentRect.size.width ;
		Y1 = (Quantity_Parameter)rect.origin.y / (Quantity_Parameter)parentRect.size.height;
		Y2 = (Quantity_Parameter)(rect.origin.y + rect.size.height) / (Quantity_Parameter)parentRect.size.height;
	} else {
	
	}
}
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: Position (
                    Standard_Integer& X1, Standard_Integer& Y1,
                    Standard_Integer& X2, Standard_Integer& Y2
                   ) const {

	HIRect rect;
	NSViewBounds(MyView, &rect);
	X1 = rect.origin.x;
	X2 = rect.origin.x + rect.size.width;
	Y1 = rect.origin.y;
	Y2 = rect.origin.y + rect.size.height;

}
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: Size (
                    Quantity_Parameter& Width, Quantity_Parameter& Height
                   ) const {

	void* parentView =  NSViewSuperview(MyView);
	if (parentView != NULL) {
		HIRect parentRect;
		HIRect rect;
		NSViewBounds(parentView, &parentRect);
		NSViewBounds(MyView, &rect);

		Width  = (Quantity_Parameter) rect.size.width / (Quantity_Parameter)parentRect.size.width;
		Height = (Quantity_Parameter) rect.size.height / (Quantity_Parameter)parentRect.size.height; 
	}	
} 
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: Size (Standard_Integer& Width, Standard_Integer& Height) const {
	HIRect rect;
	NSViewBounds(MyView, &rect);
	Width  = rect.size.width;
	Height = rect.size.height;
} 

//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: MMSize (Standard_Real& Width, Standard_Real& Height) const {

	HIRect rect;
	NSViewBounds(MyView, &rect);
	CGFloat dpiRes = NSViewDPIResolution(MyView);
	
	Width  = rect.size.width / dpiRes * 25.4F;
	Height = rect.size.height / dpiRes * 25.4F;
}
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Quantity_Parameter OSX_Window :: Convert (const Standard_Integer PV) const {

	int w, h;
	const Handle( OSX_GraphicDevice )& dev = Handle( OSX_GraphicDevice ) :: DownCast ( MyGraphicDevice );
	dev->DisplaySize ( w, h );

	return ( Quantity_Parameter )(  PV / Min ( w, h )  );

}
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
Standard_Integer OSX_Window :: Convert (const Quantity_Parameter DV) const {

	int w, h;
	const Handle( OSX_GraphicDevice )& dev = Handle( OSX_GraphicDevice ) :: DownCast ( MyGraphicDevice );
	dev->DisplaySize ( w, h );
	
	return ( Quantity_Parameter )(  DV * Min ( w, h )  );
}  
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: Convert (const Standard_Integer PX, const Standard_Integer PY,
							Quantity_Parameter& DX, Quantity_Parameter& DY) const {

	int w, h;
	const Handle( OSX_GraphicDevice )& dev = Handle( OSX_GraphicDevice ) :: DownCast ( MyGraphicDevice );
	dev->DisplaySize ( w, h );
	
	DX = ( Quantity_Parameter )PX / ( Quantity_Parameter )w;
	DY = ( Quantity_Parameter )PY / ( Quantity_Parameter )h;

}
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
void OSX_Window :: Convert (
                    const Quantity_Parameter DX, const Quantity_Parameter DY,
                    Standard_Integer& PX, Standard_Integer& PY
                   ) const {

	int w, h;
	const Handle( OSX_GraphicDevice )& dev = Handle( OSX_GraphicDevice ) :: DownCast ( MyGraphicDevice );
	dev->DisplaySize ( w, h );

	PX = ( Standard_Integer )(  DX * ( Quantity_Parameter )w           );
	PY = ( Standard_Integer )(  ( DY ) * ( Quantity_Parameter )h  );

}  // end WNT_Window :: Convert
//--------------------------------------------------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------------------------------------------------
/*void WNT_Window :: SetOutputFormat ( const WNT_TypeOfImage aFormat ) {

 myFormat = aFormat;
 myImages -> SetFormat ( aFormat );

}  // end WNT_Window :: SetOutputFormat
*/	

// -----------------------------------------------------------------------------
//	
// -----------------------------------------------------------------------------
void  OSX_Window::Draw(CGContextRef context) {
/*	if (MyOffscreenLayer) {
		CGContextDrawLayerAtPoint (context,CGPointZero,MyOffscreenLayer);
	}*/
	*command = 0;
}
	
#endif //(defined(__MACH__) && defined(__APPLE__))


/*
 
 
 //! Dumps the Window to a XWD,GIF or BMP file with <br>
 Standard_EXPORT virtual  Standard_Boolean Dump(const Standard_CString aFilename,const Standard_Real aGammaValue = 1.0) const = 0;
 
 //! Dumps the Window Area defined by his center and PIXEL size <br>
 //! to an XWD,GIF or BMP file with <br>
 //! an optional gamma correction value according to the graphic system <br>
 Standard_EXPORT virtual  Standard_Boolean DumpArea(const Standard_CString aFilename,const Standard_Integer Xc,const Standard_Integer Yc,const Standard_Integer Width,const Standard_Integer Height,const Standard_Real aGammaValue = 1.0) const = 0;
 
 //! Loads the XWD file to this Window. <br>
 //! Returns TRUE if the loading occurs normaly. <br>
 //!  Warning: Note that the Window is enlarged automatically <br>
 Standard_EXPORT virtual  Standard_Boolean Load(const Standard_CString aFilename) const = 0;
 
 //! Loads the XWD file to Window Area defined by his center <br>
 Standard_EXPORT virtual  Standard_Boolean LoadArea(const Standard_CString aFilename,const Standard_Integer Xc,const Standard_Integer Yc,const Standard_Integer Width,const Standard_Integer Height) const = 0;
 
 //! Returns the window background. <br>
 Standard_EXPORT   Aspect_Background Background() const;
 
 //! Returns the current image background string <br>
 //! or NULL if nothing is defined. <br>
 Standard_EXPORT   Standard_CString BackgroundImage() const;
 
 //! Returns the current image background fill mode. <br>
 Standard_EXPORT   Aspect_FillMethod BackgroundFillMethod() const;
 //! Returns the background bitmap handle or <br>
 //! 0 when nothing is defined <br>
 Aspect_Handle HBackground() const;
 
 //! Returns the Graphic Device associated to this Window. <br>
 Standard_EXPORT   Handle_Aspect_GraphicDevice GraphicDevice() const;
 
 //! Returns True if the window <me> is opened <br>
 //!	    and False if the window is closed. <br>
 Standard_EXPORT virtual  Standard_Boolean IsMapped() const = 0;
 
 //! Returns The Window RATIO equal to the physical <br>
 //!	    WIDTH/HEIGHT dimensions <br>
 Standard_EXPORT virtual  Quantity_Ratio Ratio() const = 0;
 
 //! Returns The Window POSITION in DSU <br>
 Standard_EXPORT virtual  void Position(Quantity_Parameter& X1,Quantity_Parameter& Y1,Quantity_Parameter& X2,Quantity_Parameter& Y2) const = 0;
 
 //! Returns The Window POSITION in PIXEL <br>
 Standard_EXPORT virtual  void Position(Standard_Integer& X1,Standard_Integer& Y1,Standard_Integer& X2,Standard_Integer& Y2) const = 0;
 
 //! Returns The Window SIZE in DSU <br>
 Standard_EXPORT virtual  void Size(Quantity_Parameter& Width,Quantity_Parameter& Height) const = 0;
 
 //! Returns The Window SIZE in PIXEL <br>
 Standard_EXPORT virtual  void Size(Standard_Integer& Width,Standard_Integer& Height) const = 0;
 
 //! Returns The Window SIZE in MM <br>
 Standard_EXPORT virtual  void MMSize(Standard_Real& Width,Standard_Real& Height) const = 0;
 
 //! Returns the DSU value depending of the PIXEL value. <br>
 Standard_EXPORT virtual  Quantity_Parameter Convert(const Standard_Integer PV) const = 0;
 
 //! Returns the PIXEL value depending of the DSU value. <br>
 Standard_EXPORT virtual  Standard_Integer Convert(const Quantity_Parameter DV) const = 0;
 
 //! Returns the DSU position depending of the PIXEL position. <br>
 Standard_EXPORT virtual  void Convert(const Standard_Integer PX,const Standard_Integer PY,Quantity_Parameter& DX,Quantity_Parameter& DY) const = 0;
 
 //! Returns the PIXEL position depending of the DSU position. <br>
 Standard_EXPORT virtual  void Convert(const Quantity_Parameter DX,const Quantity_Parameter DY,Standard_Integer& PX,Standard_Integer& PY) const = 0;

*/ 

