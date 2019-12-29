//
//  OpenGLView.m
//  HelloOpenGL
//
//  Created by Randy McLain on 2/15/17.
//  Copyright © 2017 Randy McLain. All rights reserved.
//

#import "OpenGLView.h"
#import "CC3GLMatrix.h"

@interface OpenGLView () {
    
    
    
}





@end





@implementation OpenGLView

#pragma mark - Shape Object data
typedef struct { // resource for all of our pre-vertex information
    float Position[3];
    float Color[4];
} Vertex;

//const Vertex Vertices[] = { // array of information for each vertex
//    {{1,-1,0}, {1,0,0,1}},
//    {{1,1,0}, {0,1,0,1}},
//    {{-1,1,0}, {0,0,1,1}},
//    {{-1,-1,0}, {0,0,0,1}}
//};

// Vertices post projection
//const Vertex Vertices[] = {
//    {{1, -1, -7}, {1, 0, 0, 1}},
//    {{1, 1, -7}, {0, 1, 0, 1}},
//    {{-1, 1, -7}, {0, 0, 1, 1}},
//    {{-1, -1, -7}, {0, 0, 0, 1}}
//};

// Vertices adding rotation and translation
//const Vertex Vertices[] = {
//    {{1, -1, 0}, {1, 0, 0, 1}},
//    {{1, 1, 0}, {0, 1, 0, 1}},
//    {{-1, 1, 0}, {0, 0, 1, 1}},
//    {{-1, -1, 0}, {0, 0, 0, 1}}
//};

// 3D Vertice
const Vertex Vertices[] = {
    {{1, -1, 0}, {1, 0, 0, 1}},
    {{1, 1, 0}, {1, 0, 0, 1}},
    {{-1, 1, 0}, {0, 1, 0, 1}},
    {{-1, -1, 0}, {0, 1, 0, 1}},
    {{1, -1, -1}, {1, 0, 0, 1}},
    {{1, 1, -1}, {1, 0, 0, 1}},
    {{-1, 1, -1}, {0, 1, 0, 1}},
    {{-1, -1, -1}, {0, 1, 0, 1}}
};

// pre 3d Indice
//const GLubyte Indices[] = { // array of list or triangles to create.
//    0,1,2,
//    2,3,0
//};


const GLubyte Indices[] = {
    // Front
    0, 1, 2,
    2, 3, 0,
    // Back
    4, 6, 5,
    4, 7, 6,
    // Left
    2, 7, 3,
    7, 6, 2,
    // Right
    0, 4, 1,
    4, 1, 5,
    // Top
    6, 2, 1,
    1, 6, 5,
    // Bottom
    0, 3, 7,
    0, 7, 4
};

static NSString *const SIMPLE_VERTEX = @"SimpleVertex";
static NSString *const SIMPLE_FRAGMENT = @"SimpleFragment";
static NSString *const POSITION = @"Position";


-(instancetype) initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self setupLayer];
        [self setupContext];
        [self setupDepthBuffer];
        [self setupRenderBuffer];
        [self setupFrameBuffer];
        [self compileShaders];
        [self setupVBOs];
        //[self render];
        [self setupDisplayLink];
    }
    return self;
}

-(void) dealloc {
  //  [_context release];
    _context = nil;
  //  [super dealloc];
}

#pragma mark - Shader Resources
-(GLuint)compileShader:(NSString *)shaderName withType:(GLenum)shaderType {
    NSString * shaderPath = [[NSBundle mainBundle] pathForResource:shaderName ofType:@"glsl"];
    NSError * error;
    NSString * shaderString = [NSString stringWithContentsOfFile:shaderPath
                                                        encoding:NSUTF8StringEncoding
                                                           error:&error];
    
    if (!shaderString) {
        NSLog(@"Error loading shader: %@", error.localizedDescription);
        exit(1);
    }
    
    GLuint shaderHandle = glCreateShader(shaderType); // make a shader object of shader type from method parameter
    
    const char * shaderStringUTF8 = [shaderString UTF8String];
    const int shaderStringLength = (int)[shaderString length];
    //give OpenGL the source code for this shader
    glShaderSource(shaderHandle, 1, &shaderStringUTF8, &shaderStringLength);
    
    // compiles shader at run time.
    glCompileShader(shaderHandle);
    
    // record the result if the compilation fails.
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
        
    }
    return shaderHandle;
}

-(void) compileShaders {
    GLuint vertexShader = [self compileShader:SIMPLE_VERTEX withType:GL_VERTEX_SHADER];
    GLuint fragmentShader = [self compileShader:SIMPLE_FRAGMENT withType:GL_FRAGMENT_SHADER];
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
    }
    
    glUseProgram(programHandle);
    
    _positionSlot = glGetAttribLocation(programHandle, "Position");
    _colorSlot = glGetAttribLocation(programHandle, "SourceColor");
    glEnableVertexAttribArray(_positionSlot);
    glEnableVertexAttribArray(_colorSlot);
    
    _projectionUniform = glGetUniformLocation(programHandle, "Projection"); // get the handle for setting the Projection input variable in the vertex shader.
    
    _modelViewUniform = glGetUniformLocation(programHandle, "Modelview");
    
}

#pragma mark - OpenGL setup layer, context, and buffers.
/*
 To set up a view to display OpenGL content, you need to set it’s default layer to a special kind of layer called a CAEAGLLayer.
 */
+(Class)layerClass {
    return [CAEAGLLayer class];
}

/*
 To set up a view to display OpenGL content, you need to set it’s default layer to a special kind of layer called a CAEAGLLayer.
 */
- (void)setupLayer {
    _eaglLayer = (CAEAGLLayer*) self.layer;
    _eaglLayer.opaque = YES;
}


/*
 An EAGLContext manages all of the information iOS needs to draw with OpenGL. It’s similar to how you need a Core Graphics context to do anything with Core Graphics.
 */
-(void) setupContext {
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2; // specify version for OpenGL
    _context =[[EAGLContext alloc] initWithAPI:api];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context!");
        exit(1);
    }
    
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context!");
        exit(1);
    }
}

/*
 The next step to use OpenGL is to create a render buffer, which is an OpenGL object that stores the rendered image to present to the screen.
 */
-(void) setupRenderBuffer {
    glGenBuffers(1, &_colorRenderBuffer); //unique integer for the the render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer); //whenever I refer to GL_RENDERBUFFER, I really mean _colorRenderBuffer
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer]; //allocate some storage for the render buffer
}

/*
 A frame buffer is an OpenGL object that contains a render buffer, and some other buffers...
 */
-(void) setupFrameBuffer {
    GLuint framebuffer; // same as creating a renderbuffer
    glGenFramebuffers(1, &framebuffer); // same as creating a renderbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // same as creating a renderbuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer); // attach render buffer to frame buffer's GL_COLOR_ATTACHMENT0 enum slot.
    
    // associate the new depth buffer with the existing render buffer.
    glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
    
    
}

/*
 let’s just clear the entire screen to a particular color a quick way to present something within the view.
 */
-(void) render: (CADisplayLink *)displayLink {
    glClearColor(0, 104.0/255.0, 55.0/255.0, 1.0);
    // removed when we added 3D depth -> glClear(GL_COLOR_BUFFER_BIT); // actually clear the current render and frame buffer objects.
    
    // clear and check depth each update.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //[_context presentRenderbuffer:GL_RENDERBUFFER]; // present to the new UI layer.
    

    
    //use the math library to create a projection matrix
    CC3GLMatrix * projection = [CC3GLMatrix matrix];
    float h = 4.0f * self.frame.size.height / self.frame.size.width;
    // function that lets you specify the coordinates to use for the left/right and top/bottom for the planes, and the near and far z-coordinates.
    [projection populateFromFrustumLeft:-2
                               andRight:2
                              andBottom:-h/2
                                 andTop:h/2
                                andNear:4
                                 andFar:10];

    glUniformMatrix4fv(_projectionUniform, 1, 0, projection.glMatrix);
    
    CC3GLMatrix *modelView = [CC3GLMatrix matrix];
    [modelView populateFromTranslation:CC3VectorMake(sin(CACurrentMediaTime()), 0, -7)];
    
    _currentRotation += displayLink.duration * 90;
    [modelView rotateBy:CC3VectorMake(_currentRotation, _currentRotation, 0)];
    
    glUniformMatrix4fv(_modelViewUniform, 1, 0, modelView.glMatrix);
    
    glViewport(0, 0, self.frame.size.width, self.frame.size.height); // sets the portion of the UIView for rendering

    /* feed the correct values to the two input variables for the vertex shader – the Position and SourceColor attributes
     attr name,
     valueType,
     *FLASE*,
     size of data structure containing per-vertex data,
     position for where this data is located.
     */
    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)(sizeof(float) * 3));
    
    // calls every vertex shader and fragment shader passed in...
    /*
     manner of drawing the vertices,
     count of vertices to render (dividing the whole array in bytes by the size of the first element within the array,
     is the data type of each individual index in the Indices array,
     From the documentation, it appears that the final parameter should be a pointer to the indices. But since we’re using VBOs it’s a special case – it will use the indices array we already passed to OpenGL-land in the GL_ELEMENT_ARRAY_BUFFER
     */
    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_BYTE, 0);
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

#pragma mark - Vertex Buffers 

/*
 Setup Vertex Object Buffers - object repo for vertex data.
 */
-(void) setupVBOs {
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

}

#pragma mark - Setup Display Link 

-(void) setupDisplayLink {
    CADisplayLink * displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

#pragma mark - Depth Renderer 
-(void) setupDepthBuffer {
    glGenRenderbuffers(1, &_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, self.frame.size.width, self.frame.size.height);
}





@end
