#include "ofMain.h"

class ofApp : public ofBaseApp {
    
    ofShader renderShader, feedbackShader;
    GLuint FeedbackPositionAtt, RenderPositionAtt;
    
    GLuint TransformVAO = 0, TransformVBO = 0;
    GLuint FeedbackVAO = 0, FeedbackVBO = 0;
    GLuint TransformFeedback = 0;
    
    ofEasyCam camera;
    
    //--------------------------------------------------------------
    void setup()
    {

        renderShader.load("shaders/render.vert", "shaders/render.frag");
        
        feedbackShader.setupShaderFromFile(GL_VERTEX_SHADER, "shaders/feedback.vert");
        const GLchar* feedbackVaryings[] = { "o_position" };
        glTransformFeedbackVaryings(feedbackShader.getProgram(), 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
        feedbackShader.linkProgram();
        
        FeedbackPositionAtt = glGetAttribLocation(feedbackShader.getProgram(), "position");
        RenderPositionAtt = glGetAttribLocation(renderShader.getProgram(), "position");

        
        GLfloat PositionData[] = {
            50.0f, -50.0f, 0.0f,
            -50.0f, -50.0f, 0.0f,
            0.0f,  50.0f, 0.0f,
        };
        
        
        //init buffers----------
        glGenBuffers(1, &TransformVBO);
        glBindBuffer(GL_ARRAY_BUFFER, TransformVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*9, PositionData, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &FeedbackVBO);
        glBindBuffer(GL_ARRAY_BUFFER, FeedbackVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*9, nullptr, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
        
        
        //init vertex array------
        glGenVertexArrays(1, &TransformVAO);
        glBindVertexArray(TransformVAO);
            glBindBuffer(GL_ARRAY_BUFFER, TransformVBO);
            glVertexAttribPointer(FeedbackPositionAtt, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glEnableVertexAttribArray(FeedbackPositionAtt);
        glBindVertexArray(0);
        
        
        glGenVertexArrays(1, &FeedbackVAO);
        glBindVertexArray(FeedbackVAO);
            glBindBuffer(GL_ARRAY_BUFFER, FeedbackVBO);
            glVertexAttribPointer(RenderPositionAtt, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glEnableVertexAttribArray(RenderPositionAtt);
        glBindVertexArray(0);
        
        
        //init TF object------
        glGenTransformFeedbacks(1, &TransformFeedback);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackVBO);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        
    }
    
    
    //--------------------------------------------------------------
    void update()
    {
        
        glEnable(GL_RASTERIZER_DISCARD);
        feedbackShader.begin();
        glBindVertexArray(TransformVAO);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackVBO);
        glBeginTransformFeedback(GL_TRIANGLES);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);
        glEndTransformFeedback();
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        feedbackShader.end();
        glDisable(GL_RASTERIZER_DISCARD);
        
    }
    
    
    //--------------------------------------------------------------
    void draw()
    {
        
        glEnable(GL_DEPTH_TEST);
        
        glClearColor(0.0, 0.6, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        camera.begin();
        renderShader.begin();
        glBindVertexArray(FeedbackVAO);
        glDrawTransformFeedback(GL_TRIANGLES, TransformFeedback);
        renderShader.end();
        camera.end();
        
        
        swap(TransformVBO, FeedbackVBO);
    }
    
    
    void exit()
    {
        glDeleteVertexArrays(1, &TransformVAO);
        glDeleteBuffers(1, &TransformVBO);
        glDeleteVertexArrays(1, &FeedbackVAO);
        glDeleteBuffers(1, &FeedbackVBO);
        glDeleteTransformFeedbacks(1, &TransformFeedback);

    }
    
    
    //--------------------------------------------------------------
    void keyPressed(int key){
        
    }
    
    //--------------------------------------------------------------
    void keyReleased(int key){
        
    }
    
};

//========================================================================
int main(){
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 0);
    settings.width = 640;
    settings.height = 360;
    settings.resizable = false;
    ofCreateWindow(settings);
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
}
