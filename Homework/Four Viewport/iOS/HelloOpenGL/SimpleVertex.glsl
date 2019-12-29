attribute vec4 Position; //attribute keyword declares that this shader is going to be passed in an input variable called Position
attribute vec4 SourceColor; //attribute keyword declares that this shader is going to be passed in an input variable called SourceColor

varying vec4 DestinationColor; //is an output variable that will be passed to the fragment shader and color will be based on colors around it (varying)

uniform mat4 Projection; // passing a constant value for all vertices rather than a varying value... mat4 stands for  4x4 matrix.

uniform mat4 Modelview;

void main(void) { //Every shader begins with a main
    DestinationColor = SourceColor;
    gl_Position = Projection * Modelview * Position; //built in output variable you have to set in the vertex shader called gl_Position equal to the final position of the vertex
    
}
