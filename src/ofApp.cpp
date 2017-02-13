#include "ofMain.h"

class ofApp : public ofBaseApp {
    
    ofShader renderShader, feedbackShader;
    GLuint FeedbackPositionAtt, RenderPositionAtt;
    
    GLuint TransformFeedback = 0;
    
    ofBufferObject TFVbo[2];
    ofVbo TFVao[2];
    
    
    ofEasyCam camera;
    
    //--------------------------------------------------------------
    void setup()
    {

        renderShader.load("shaders/render.vert", "shaders/render.frag");
        
        feedbackShader.setupShaderFromFile(GL_VERTEX_SHADER, "shaders/feedback.vert");
        const GLchar* feedbackVaryings[] = { "o_position" };
        glTransformFeedbackVaryings(feedbackShader.getProgram(), 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
        feedbackShader.linkProgram();
        
//        FeedbackPositionAtt = glGetAttribLocation(feedbackShader.getProgram(), "position");
//        RenderPositionAtt = glGetAttribLocation(renderShader.getProgram(), "position");

        
        ofVec3f pos[3];
        
        pos[0] = ofVec3f(10.5f, -10.5f, 0.0f);
        pos[1] = ofVec3f(-10.5f, -10.5f, 0.0f);
        pos[2] = ofVec3f(0.0f,  10.5f, 0.0f);
        
        
        for(int i = 0; i < 2; i++)
        {
            TFVbo[i].allocate();
            TFVbo[i].bind(GL_ARRAY_BUFFER);
            TFVbo[i].setData(sizeof(ofVec3f) * 3, pos, GL_STATIC_DRAW);
            TFVbo[i].unbind(GL_ARRAY_BUFFER);
            
            TFVao[i].bind();
            TFVao[i].setVertexBuffer(TFVbo[i], 3, sizeof(ofVec3f), 0);
            TFVao[i].unbind();
        }
        
        
        //init TF object------
        glGenTransformFeedbacks(1, &TransformFeedback);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, TFVbo[1].getId());
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        
    }
    
    
    //--------------------------------------------------------------
    void update()
    {
        
        glEnable(GL_RASTERIZER_DISCARD);
        feedbackShader.begin();
        feedbackShader.setUniform1f("u_time", ofGetElapsedTimef());
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, TFVbo[1].getId());
        glBeginTransformFeedback(GL_TRIANGLES);
            TFVao[0].draw(GL_TRIANGLES, 0, 3);
        glEndTransformFeedback();
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        feedbackShader.end();
        glDisable(GL_RASTERIZER_DISCARD);
        
    }
    
    
    //--------------------------------------------------------------
    void draw()
    {
        
        glEnable(GL_DEPTH_TEST);
        
        glClearColor(0.0, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        camera.begin();
        renderShader.begin();
        TFVao[1].bind();
        glDrawTransformFeedback(GL_TRIANGLES, TransformFeedback);
        TFVao[1].unbind();
        renderShader.end();
        camera.end();
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
