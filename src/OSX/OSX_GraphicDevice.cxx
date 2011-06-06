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

#if (defined(__MACH__) && defined(__APPLE__))
#include <Carbon/Carbon.h>
#include <OSX_GraphicDevice.hxx>
#include <OpenGL_GraphicDriver.hxx>
#include <Aspect_GraphicDevice.hxx>
#include <Aspect_GraphicDeviceDefinitionError.hxx>
#include <OSD_Function.hxx>

#include <stdio.h>
#include <sys/utsname.h>

IMPLEMENT_STANDARD_HANDLE  (OSX_GraphicDevice, Aspect_GraphicDevice)
IMPLEMENT_STANDARD_RTTIEXT (OSX_GraphicDevice, Aspect_GraphicDevice)

OSX_GraphicDevice::OSX_GraphicDevice () {
    CGDirectDisplayID mainDisplay = CGMainDisplayID ();
    CGSize size = CGDisplayScreenSize (mainDisplay);
    myWidth = size.width;
    myHeight = size.height;
    
//  SetGraphicDriver ();
    MyGraphicDriver->Begin ("");
}

OSX_GraphicDevice::~OSX_GraphicDevice() {
    Destroy();
}

void OSX_GraphicDevice::Destroy() {
    MyGraphicDriver->End();

}

void OSX_GraphicDevice :: DisplaySize ( Standard_Integer& aWidth, Standard_Integer& aHeight) const {
    aWidth  = myWidth;
    aHeight = myHeight;
}

Handle(Aspect_GraphicDriver) OSX_GraphicDevice::GraphicDriver () const {
    return MyGraphicDriver;
}

//void OSX_GraphicDevice::SetGraphicDriver () {
//    char *glshr = getenv("CSF_GraphicShr");
//    MyGraphicDriver = new OpenGl_GraphicDriver(glshr);
//}

//Handle(Aspect_GraphicDriver) OSX_GraphicDevice::GraphicDriver () const {
//    static Handle(Aspect_GraphicDriver) foo;
//    cout << "OSX_GraphicDevice::GraphicDriver returns foo\n" << flush;
//    return foo;
//}



Standard_Boolean OSX_GraphicDevice::ShrIsDefined (Standard_CString& aShr) const {
    return Standard_True;
    
}

#endif 
