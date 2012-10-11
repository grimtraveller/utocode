/* All Rights reserved */

#include <AppKit/AppKit.h>
#import "appcontroller.h"
id pool = nil;

void releasePool()
{
	[pool drain];
}

void allocPool()
{
	pool = [[NSAutoreleasePool alloc] init];
	atexit(releasePool);
}


int main(int argc, char **argv)
{
	allocPool();
	[NSApplication sharedApplication];
	id controller = [[AppController alloc] init];
	[controller autorelease];
	[NSApp setDelegate: controller];
	[NSApp run];
	return 0;
}
