//
//  OCNISView.h
//  OpenCascade
//
//  Created by Emmanuel Valentin on 27/01/09.
//  Copyright 2009 Smooth Softs. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <V3d_Viewer.hxx>
#include <NIS_View.hxx>

@interface OCNISView : NSView {
	void*							reserved1;
	BOOL							shouldInitializeOCC;
	Handle_V3d_Viewer				v3dViewer;
	Handle_NIS_View					nisView;
	BOOL							endInit;
}

- (const Handle_V3d_Viewer&)				v3dViewer;
- (const Handle_NIS_View&)					nisView;

- (void)									update;
@end
