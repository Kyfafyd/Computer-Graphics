varying lowp vec4 DestinationColor; //This is the input variable from the vertex shader

void main(void) { //fragment shader also begins with main
    gl_FragColor = DestinationColor; //This simply sets it to the destination color passed in (interpolated by OpenGL).
   // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
