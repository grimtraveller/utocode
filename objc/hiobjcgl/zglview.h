#import <Cocoa/Cocoa.h>

@interface ZGlView : NSOpenGLView
{
}

- (id) initWithFrame:(NSRect)frame colorBits:(int)numColorBits
		   depthBits:(int)numDepthBits fullscreen:(BOOL)runFullScreen;

@end
