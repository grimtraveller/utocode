#import "zglview.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>

@implementation ZGlView

- (id) initWithFrame:(NSRect)frame colorBits:(int)numColorBits
		   depthBits:(int)numDepthBits fullscreen:(BOOL)runFullScreen
{
	NSOpenGLPixelFormat *pixelFormat;	
	int colorBits = numColorBits;
	int depthBits = numDepthBits;
	NSOpenGLPixelFormatAttribute pixelAttribs[ 16 ];
	int pixNum = 0;
	pixelAttribs[ pixNum++ ] = NSOpenGLPFADoubleBuffer;
	pixelAttribs[ pixNum++ ] = NSOpenGLPFAAccelerated;
	pixelAttribs[ pixNum++ ] = NSOpenGLPFAColorSize;
	pixelAttribs[ pixNum++ ] = colorBits;
	pixelAttribs[ pixNum++ ] = NSOpenGLPFADepthSize;
	pixelAttribs[ pixNum++ ] = depthBits;	
	pixelAttribs[ pixNum ] = 0;
	pixelFormat = [ [ NSOpenGLPixelFormat alloc ]
                   initWithAttributes:pixelAttribs ];
	self = [ super initWithFrame:frame pixelFormat:pixelFormat ];
	[ pixelFormat release ];
	[ [ self openGLContext ] makeCurrentContext ];
	[ self reshape ];
	glShadeModel( GL_SMOOTH );                // Enable smooth shading
	glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );   // Black background
	glClearDepth( 1.0f );                     // Depth buffer setup
	glEnable( GL_DEPTH_TEST );                // Enable depth testing
	glDepthFunc( GL_LEQUAL );                 // Type of depth test to do
	// Really nice perspective calculations
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	
	
	return self;
}

/*
 * Resize ourself
 */
- (void) reshape
{ 
	NSRect sceneBounds;
	
	[ [ self openGLContext ] update ];
	sceneBounds = [ self bounds ];
	// Reset current viewport
	glViewport( 0, 0, sceneBounds.size.width, sceneBounds.size.height );
	glMatrixMode( GL_PROJECTION );   // Select the projection matrix
	glLoadIdentity();                // and reset it
	// Calculate the aspect ratio of the view
	gluPerspective( 45.0f, sceneBounds.size.width / sceneBounds.size.height,0.1f, 100.0f );
	glMatrixMode( GL_MODELVIEW );    // Select the modelview matrix
	glLoadIdentity();                // and reset it
}

/*
 * Called when the system thinks we need to draw.
 */
- (void) drawRect:(NSRect)rect
{
		// Clear the screen and depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();   // Reset the current modelview matrix

	glTranslatef( -1.5f, 0.0f, -6.0f );   // Left 1.5 units, into screen 6.0
	
	glBegin( GL_TRIANGLES );             // Draw a triangle
	glVertex3f(  0.0f,  1.0f, 0.0f );    // Top
	glVertex3f( -1.0f, -1.0f, 0.0f );    // Bottom left
	glVertex3f(  1.0f, -1.0f, 0.0f );    // Bottom right
	glEnd();                             // Done with the triangle

	glTranslatef( 0.0f, 0.0f, 0.0f );    // Move right 3 units
	
	glBegin( GL_QUADS );                // Draw a quad
	glVertex3f( -1.0f,  1.0f, 0.0f );   // Top left
	glVertex3f(  1.0f,  1.0f, 0.0f );   // Top right
	glVertex3f(  1.0f, -1.0f, 0.0f );   // Bottom right
	glVertex3f( -1.0f, -1.0f, 0.0f );   // Bottom left
	glEnd();                            // Quad is complete
	
	[ [ self openGLContext ] flushBuffer ];
}

@end
