//
//  OCGraphicDevice.h
//  OpenCascade
//
//  Created by Emmanuel Valentin on 27/01/09.
//  Copyright 2009 Smooth Softs. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OSX_GraphicDevice.hxx"

@interface OCGraphicDevice : NSObject {
	Handle_OSX_GraphicDevice		graphicDevice;
}

+ (OCGraphicDevice*)	defaultDevice ;

-(const Handle_OSX_GraphicDevice&)		graphicDevice;

@end
