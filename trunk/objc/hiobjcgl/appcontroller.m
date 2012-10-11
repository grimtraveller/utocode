#import "appcontroller.h"
#import <AppKit/AppKit.h>

@implementation AppController

- (void)applicationWillFinishLaunching:(NSNotification *)aNotification
{
	NSLog(@"%s", __FUNCTION__);
	NSWindow* win = [NSApp mainWindow];
	NSRect wf0 = {{200, 200}, {600, 400}};
	wf0.origin.x = 200;
	wf0.origin.y = 200;
	wf0.size.width = 800;
	wf0.size.height = 600;
	int style = NSTitledWindowMask | NSClosableWindowMask 
		| NSMiniaturizableWindowMask;
	win = [[NSWindow alloc] initWithContentRect: wf0
			  styleMask: style
			  backing: NSBackingStoreBuffered
			  defer: NO];
	[win setDelegate:self];
	_view = [ [ ZGlView alloc ] initWithFrame:[ win frame ]
										  colorBits:16 depthBits:16 fullscreen:FALSE ];
 
	[win setContentView:_view];
	[win orderFrontRegardless];
}

- (void)applicationDidFinishLaunching: (NSNotification *)notification
{
	NSLog(@"%s", __FUNCTION__);
	return;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	NSLog(@"%s", __FUNCTION__);
	return YES;
}

- (void)windowWillClose:(NSNotification*)notification
{
	NSLog(@"%s", __FUNCTION__);
}

- (void)windowShouldClose:(NSNotification*)notification
{
	NSLog(@"%s", __FUNCTION__);
}


- (void)dealloc
{
	NSLog(@"%s", __FUNCTION__);
	[[NSApp mainWindow] release];
	[_view release];
	[super dealloc];
}

@end //implementation AppController
