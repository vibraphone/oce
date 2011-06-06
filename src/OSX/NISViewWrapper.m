/*
 The content of this file is subject to the Open CASCADE Technology 
 Public License Version 6.2 (the "License"). You may not use the content 
 of this file except in compliance with the License. 
 Please obtain a copy of the License at http://www.opencascade.org and 
 read it completely before using this file. 
 
 The Initial Developer of the Original Code is Emmanuel Valentin, 
 The Original Code is copyright © SmoothSofts, 2009. All rights reserved. 
 
 The software Open CASCADE Technology and all software distributed 
 under the License are distributed on an "AS IS" basis, without warranty of 
 any kind, and the Initial Developer hereby disclaims all such warranties, 
 including without limitation, any warranties of merchantability, fitness 
 for a particular purpose or non-infringement. Please see the License for the 
 specific terms and conditions governing rights and limitations under the License" 
 */

#include "NSViewWrapper.h"
#include <Quartz/Quartz.h>

#define POOL_ON	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init]; 
#define POOL_OFF [pool release];

#define copyNSRectToCGRect(SRC,DEST)	\
	DEST->origin.x = SRC.origin.x;		\
	DEST->origin.y = SRC.origin.y;		\
	DEST->size.width = SRC.size.width;	\
	DEST->size.height = SRC.size.height;

#define copyCGRectToNSRect(SRC,DEST)	\
	DEST.origin.x = SRC->origin.x;		\
	DEST.origin.y = SRC->origin.y;		\
	DEST.size.width = SRC->size.width;	\
	DEST.size.height = SRC->size.height;

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
Boolean	NSViewIsHiddenOrHasHiddenAncestor( void* theView ) {
	POOL_ON
	
	Boolean result = [(NSView*) theView isHiddenOrHasHiddenAncestor];

	POOL_OFF
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
void	NSViewFrame(void* theView, CGRect *outRect) {
	POOL_ON

	NSRect rect = [(NSView*) theView frame];
	copyNSRectToCGRect(rect, outRect);
	
	POOL_OFF
}


//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
void		NSViewBounds(void* theView, CGRect *outRect) {
	POOL_ON
	
	NSRect rect = [(NSView*) theView bounds];
	copyNSRectToCGRect(rect,outRect);
	
	POOL_OFF
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
void		NSViewSetHidden(void* theView, Boolean flag) {
	POOL_ON

	[(NSView*) theView setHidden: flag];
	
	POOL_OFF
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
void		NSViewSetNeedsDisplayInRect(void*  myView, CGRect *rect) {
	NSRect nsrect; 
	copyCGRectToNSRect(rect, nsrect);

	[(NSView*) myView setNeedsDisplayInRect: nsrect ];
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
void		NSViewSetNeedsDisplay(void*  myView){
	[(NSView*) myView setNeedsDisplay: TRUE];
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
void*		NSViewSuperview(void* myView) {
	POOL_ON
	
	void*	parent = [(NSView*) myView superview];
	
	POOL_OFF
	
	return parent;
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
CGFloat		NSViewDPIResolution(void*myView) {
	POOL_ON

	CGFloat result = [[(NSView*) myView window] userSpaceScaleFactor];
	result *= 72;
	
	POOL_OFF
	
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
CGLayerRef	CIContextCreateCGLayer(void* myView, void* glContext, void* pixelFormat, CGSize size) {
	POOL_ON
	
	CIContext* ciContext = [[CIContext contextWithCGLContext: glContext pixelFormat:pixelFormat options: nil] retain];
	CGLayerRef ref =  [ciContext createCGLayerWithSize:size info:nil];
	[(CIContext*) ciContext release];
	
	POOL_OFF
	return ref;
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
Boolean		NSViewLockFocus(void* myView) {
	Boolean result =  [(NSView*)myView lockFocusIfCanDraw];
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------------------
void		NSViewUnlockFocus(void* myView) {
	[(NSView*)myView unlockFocus];
}
