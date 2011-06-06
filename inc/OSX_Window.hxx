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


#ifndef _OSX_Window_HeaderFile
#define _OSX_Window_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineHandle_HeaderFile
#include <Standard_DefineHandle.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Aspect_Window_HeaderFile
#include <Aspect_Window.hxx>
#endif
//#ifndef _Handle_Xw_GraphicDevice_HeaderFile
//#include <Handle_Xw_GraphicDevice.hxx>
//#endif
#ifndef _Quantity_NameOfColor_HeaderFile
#include <Quantity_NameOfColor.hxx>
#endif
#ifndef _Standard_CString_HeaderFile
#include <Standard_CString.hxx>
#endif
#ifndef _Quantity_Parameter_HeaderFile
#include <Quantity_Parameter.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Aspect_FillMethod_HeaderFile
#include <Aspect_FillMethod.hxx>
#endif
#ifndef _Aspect_GradientFillMethod_HeaderFile
#include <Aspect_GradientFillMethod.hxx>
#endif
#ifndef _Aspect_TypeOfResize_HeaderFile
#include <Aspect_TypeOfResize.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
//#ifndef _Handle_Aspect_PixMap_HeaderFile
//#include <Handle_Aspect_PixMap.hxx>
//#endif
#ifndef _Quantity_Ratio_HeaderFile
#include <Quantity_Ratio.hxx>
#endif


//#include <WNT_TypeOfImage.hxx>
//#include <Handle_WNT_ImageManager.hxx>
//#include <Handle_WNT_Icon.hxx>
//#include <Handle_WNT_GraphicDevice.hxx>
#ifndef _Handle_OSX_Window_HeaderFile
#include <Handle_OSX_Window.hxx>
#endif
#ifndef _OSX_GraphicDevice_HeaderFile
#include <OSX_GraphicDevice.hxx>
#endif

class Aspect_WindowDefinitionError;
class Aspect_WindowError;
class Aspect_Background;
class Quantity_Color;

//! This class defines MacOSX window <br>
//!  Warning: The position and size for the creation of the window <br>
//!	    are defined in Device Screen Unit (DSU) <br>
//!	    floating [0,1] space. <br>

class OSX_Window : public Aspect_Window {

public:
 // Methods PUBLIC
 // 

Standard_EXPORT OSX_Window(const Handle(OSX_GraphicDevice)& aDevice,
                           const Standard_Address           aView,
                           const Standard_Address           aGLContext,
                           const Standard_Address           aPixelFormat);
	
Standard_EXPORT ~OSX_Window(); 
	
//! Destroies the Window and all resourses attached to it. <br>
Standard_EXPORT   void Destroy() ;


//! Modifies the window background. <br>
Standard_EXPORT void SetBackground(const Aspect_Background& Background) ;

//! Modifies the window background. <br>
Standard_EXPORT void SetBackground(const Quantity_NameOfColor BackColor) ;

//! Modifies the window background. <br>
Standard_EXPORT void SetBackground(const Quantity_Color& color);

Standard_EXPORT Standard_Boolean SetBackground(const Standard_CString aName,const Aspect_FillMethod aMethod = Aspect_FM_CENTERED);
	
Standard_EXPORT void SetBackground(const Aspect_Handle aBitmap);

Standard_EXPORT void SetBackground(const Aspect_GradientBackground& ABackground);

//! Sets cursor <aCursor> for ENTIRE WINDOW CLASS to which <br>
//!          the Window belongs. <br>
Standard_EXPORT   void SetCursor(const Aspect_Handle aCursor) const;

//! Activates/Deactivates the Double Buffering capability <br>
//!	    for this window. <br>
//!  Warning: Double Buffering is always DISABLE by default. <br>
//!  Trigger: Raises if BackingStore () isn't allowed for this Window <br>
Standard_EXPORT   void SetDoubleBuffer(const Standard_Boolean DBmode) ;

//!  Flushes all graphics to the screen and Swap the Double <br>
//!	     buffer if Enable <br>
//!  Trigger: Raises if Something is WRONG at Drawing Time. <br>
Standard_EXPORT   void Flush() const;

//!  Opens the window <me>. <br>
Standard_EXPORT   void Map() const;

//! Opens a window <me> according to <aMapMode>. <br>
//!          This method is specific to Windows NT. <br>
//!          <aMapMode> can be one of SW_xxx constants defined <br>
//!          in <windows.h>. See documentation. <br>
Standard_EXPORT   void Map(const Standard_Integer aMapMode) const;

//! Closes the window <me>. <br>
Standard_EXPORT   void Unmap() const;

//! Applies the resizing to the window <me>. <br>
Standard_EXPORT   Aspect_TypeOfResize DoResize() const;

//! Apply the mapping change to the window <me> <br>
//! and returns TRUE if the window is mapped at screen. <br>
Standard_EXPORT   Standard_Boolean DoMapping() const;

//! Clears the Window in the Background color. <br>
Standard_EXPORT   void Clear() const;

//!  Clears the Window Area defined by his center and PIXEL <br>
//!           size in the Background color <br>
//!  Trigger:  Raises if Window is not defined properly <br>
Standard_EXPORT   void ClearArea(const Standard_Integer Xc,const Standard_Integer Yc,const Standard_Integer Width,const Standard_Integer Height) const;

//! Restores The Window from the BackingStored Window <br>
//!	    See BackingStore () method. <br>
Standard_EXPORT   void Restore() const;

//! Restores The Window Area defined by his center <br>
//!	    and PIXEL size from the BackingStored Window <br>
//!	    See BackingStore () method. <br>
Standard_EXPORT   void RestoreArea(const Standard_Integer Xc,const Standard_Integer Yc,const Standard_Integer Width,const Standard_Integer Height) const;

//! Dumps the Window to an XWD,GIF or BMP filei with <br>
//! an optional gamma correction value according to the graphic system. <br>
//! and returns TRUE if the dump occurs normaly. <br>
//!  Trigger: Raises if Window is not defined properly <br>
Standard_EXPORT   Standard_Boolean Dump(const Standard_CString aFilename,const Standard_Real aGammaValue = 1.0) const;

//! Dumps the Window Area defined by his center and PIXEL size <br>
//!	    to an XWD,GIF or BMP file with <br>
//! an optional gamma correction value according to the graphic system <br>
//!          and returns TRUE if the dump occurs normaly. <br>
//!  Trigger: Raises if Window is not defined properly <br>
//!	    or the area is out of the Window. <br>
Standard_EXPORT   Standard_Boolean DumpArea(const Standard_CString aFilename,const Standard_Integer Xc,const Standard_Integer Yc,const Standard_Integer Width,const Standard_Integer Height,const Standard_Real aGammaValue = 1.0) const;

//! dump the full contents of the window to a pixmap. <br>
Standard_EXPORT   virtual  Handle_Aspect_PixMap ToPixMap() const;

//! Loads the XWD file to this Window. <br>
//!          Returns TRUE if the loading occurs normaly. <br>
//!  Warning: Note that the Window is enlarged automatically <br>
//!          when the image size is too large for this window. <br>
//!  Trigger: Raises if Window is not defined properly <br>
Standard_EXPORT   Standard_Boolean Load(const Standard_CString aFilename) const;

//! Loads the XWD file to Window Area defined by his center <br>
//!          and PIXEL size. <br>
//!          Returns TRUE if the loading occurs normaly. <br>
//!  Warning: Note that the Image is zoomed automatically <br>
//!          when the image size is too large for this window area. <br>
//!  Trigger: Raises if Window is not defined properly <br>
//!          or the area is out of the Window. <br>
Standard_EXPORT   Standard_Boolean LoadArea(const Standard_CString aFilename,const Standard_Integer Xc,const Standard_Integer Yc,const Standard_Integer Width,const Standard_Integer Height) const;

//! Sets format of the image file created by Dump or <br>
//!          DumpArea methods. <br>
//Standard_EXPORT   void SetOutputFormat(const WNT_TypeOfImage aFormat) ;

//! Changes variables due to window position. <br>
Standard_EXPORT   void SetPos(const Standard_Integer X,const Standard_Integer Y,const Standard_Integer X1,const Standard_Integer Y1) ;

//! Sets user defined flags in the extra window data area. <br>
Standard_EXPORT   void SetFlags(const Standard_Integer aFlags) ;

//! Reset specified flags in the extra window data area. <br>
Standard_EXPORT   void ResetFlags(const Standard_Integer aFlags) ;

//! Returns the BackingStore capability for this Window. <br>
//!	    If Answer is True Exposure can be recovered by <br>
//!		Restore RestoreArea methods. <br>
//!	    If Answer is False, Application must Redraw the <br>
//!	        exposed area. <br>
Standard_EXPORT   Standard_Boolean BackingStore() const;
//! Returns the DoubleBuffer state. <br>
  Standard_Boolean DoubleBuffer() const;

//! Returns True if the window <me> is opened <br>
//!	        and False if the window is closed. <br>
Standard_EXPORT   Standard_Boolean IsMapped() const;

//! Returns The Window RATIO equal to the physical <br>
//!	    WIDTH/HEIGHT dimensions. <br>
Standard_EXPORT   Quantity_Ratio Ratio() const;

//! Returns The Window POSITION in DSU <br>
Standard_EXPORT   void Position(Quantity_Parameter& X1,Quantity_Parameter& Y1,Quantity_Parameter& X2,Quantity_Parameter& Y2) const;

//! Returns The Window POSITION in PIXEL <br>
Standard_EXPORT   void Position(Standard_Integer& X1,Standard_Integer& Y1,Standard_Integer& X2,Standard_Integer& Y2) const;

//! Returns The Window SIZE in DSU <br>
Standard_EXPORT   void Size(Quantity_Parameter& Width,Quantity_Parameter& Height) const;

//! Returns The Window SIZE in PIXEL <br>
Standard_EXPORT   void Size(Standard_Integer& Width,Standard_Integer& Height) const;

//! Returns The Window SIZE in MM <br>
Standard_EXPORT   void MMSize(Standard_Real& Width,Standard_Real& Height) const;

//! Returns the DSU value depending of the PIXEL value. <br>
Standard_EXPORT   Quantity_Parameter Convert(const Standard_Integer PV) const;

//! Returns the PIXEL value depending of the DSU value. <br>
Standard_EXPORT   Standard_Integer Convert(const Quantity_Parameter DV) const;

//! Returns the DSU position depending of the PIXEL position. <br>
Standard_EXPORT   void Convert(const Standard_Integer PX,const Standard_Integer PY,Quantity_Parameter& DX,Quantity_Parameter& DY) const;

//! Returns the PIXEL position depending of the DSU position. <br>
Standard_EXPORT   void Convert(const Quantity_Parameter DX,const Quantity_Parameter DY,Standard_Integer& PX,Standard_Integer& PY) const;

Standard_EXPORT	void  Draw(CGContextRef context);

Standard_EXPORT	void*	OSXWindow();

Standard_EXPORT	void*	GLContext();

 // Type management
//DEFINE_STANDARD_RTTI(NIS_View)
DEFINE_STANDARD_RTTI(OSX_Window)
	
    CGLayerRef        MyOffscreenLayer;
protected:
    Standard_Integer* command;
    Standard_Address  MyView;
    Standard_Address  MyGLContext;
    Standard_Address  MyCIContext;
    CGLayerRef	      MySavedArea;
    CGColorRef        MyBackColor;
    CGColorSpaceRef   MyColorSpace;

private: 
};

#endif
