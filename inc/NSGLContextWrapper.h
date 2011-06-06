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

#ifndef __NSGLContextWrapper_h__
#define __NSGLContextWrapper_h__

#include <Carbon/Carbon.h>
#include <OpenGL/gl.h>
#include <OpenGL/CGLTypes.h>

#ifdef __cplusplus
extern "C" {
#endif
	
	void*			NSGLChoosePixelFormat(UInt32* attribs);
	void			NSGLDescribePixelFormat(void* pf, GLint* value, UInt32 attrib, GLint screen);
	void			NSGLDestroyPixelFormat(void* pf);

	void*			NSGLContextCreate( void* pf, void* shareCG );
	void			NSGLSetDrawable(void* CG, void* drawable );
	void*			NSGLGetCurrentDrawable(void* CG);
	void			NSGLClearDrawable(void* CG);
	void			NSGLDestroyContext(void* CG);
	CGLContextObj	NSGLCGLContextObj(void* CG);
	void			NSGLMakeCurrentContext(void* CG);
	void*			NSGLGetCurrentContext();
	void			NSGLMakeCurrent(void* CG, void* drawable);
	void			NSGLFlushBuffer(void* CG);
	void			NSGLUpdate(void* CG);

#ifdef __cplusplus
}
#endif

#endif
