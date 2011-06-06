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
#ifndef __NSViewWrapper_h__
#define __NSViewWrapper_h__

#include <Carbon/Carbon.h>

#ifdef __cplusplus
extern "C" {
#endif

	Boolean		NSViewIsHiddenOrHasHiddenAncestor( void* theView );
	void		NSViewFrame(void* theView, CGRect *outRect);
	void		NSViewBounds(void* theView, CGRect *outRect);
	void		NSViewSetHidden(void* theView, Boolean flag);
	void		NSViewSetNeedsDisplayInRect(void*  myView, CGRect *rect); 
	void		NSViewSetNeedsDisplay(void*  myView); 
	void*		NSViewSuperview(void* myView);
	CGFloat		NSViewDPIResolution(void* myView);
	CGLayerRef	CIContextCreateCGLayer(void* myView, void* glContext, void* pixelFormat, CGSize size) ;
	
	Boolean		NSViewLockFocus(void* myView);
	void		NSViewUnlockFocus(void* myView);

#ifdef __cplusplus
}
#endif

#endif //__NSViewWrapper_h__