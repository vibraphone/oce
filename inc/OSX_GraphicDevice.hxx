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


#ifndef _OSX_GraphicDevice_HeaderFile
#define _OSX_GraphicDevice_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#include <Standard_DefineHandle.hxx>
#include <Quantity_Length.hxx>
#include <Standard_Integer.hxx>
#include <Aspect_Handle.hxx>
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#include <Aspect_GraphicDevice.hxx>
#include <Handle_Aspect_ColorMap.hxx>
#ifndef _Handle_Aspect_GraphicDriver_HeaderFile
#include <Handle_Aspect_GraphicDriver.hxx>
#endif
#include <Graphic3d_GraphicDriver.hxx>
//#include <Handle_OSX_GraphicDevice.hxx>

class Aspect_GraphicDeviceDefinitionError;
class Aspect_BadAccess;
class OSX_Window;
class Quantity_Color;
class Aspect_ColorMap;
class Aspect_GraphicDriver;

class OSX_GraphicDevice : public Aspect_GraphicDevice {

public:
	Standard_EXPORT OSX_GraphicDevice();

	Standard_EXPORT virtual  void Destroy() ;
	
	Standard_EXPORT ~OSX_GraphicDevice();

	void DisplaySize(Standard_Integer& aWidth,Standard_Integer& aHeight) const;
	Standard_EXPORT virtual		Handle_Aspect_GraphicDriver GraphicDriver() const;

	// Type management
	DEFINE_STANDARD_RTTI (OSX_GraphicDevice)
friend class OSX_Window;



protected:

	Quantity_Length myMWidth;
	Quantity_Length myMHeight;
	Standard_Integer myWidth;
	Standard_Integer myHeight;
	
private:

	//Standard_EXPORT void SetGraphicDriver ();
	//Standard_EXPORT Standard_Boolean ShrIsDefined (Standard_CString& aShr) const;
	void SetGraphicDriver ();
	Standard_Boolean ShrIsDefined (Standard_CString& aShr) const;


	Handle_Graphic3d_GraphicDriver MyGraphicDriver;

};

DEFINE_STANDARD_HANDLE (OSX_GraphicDevice, Aspect_GraphicDevice)

//IMPLEMENT_STANDARD_HANDLE(OSX_GraphicDevice, Aspect_GraphicDevice)
//IMPLEMENT_STANDARD_RTTIEXT(OSX_GraphicDevice, Aspect_GraphicDevice)

#endif
