#include "ofMain.h"

class ofApp : public ofBaseApp {
    
    ofShader renderShader, feedbackShader;
    GLuint TransformFeedback = 0;
    GLuint TransformVbo, TransformVao;
    GLuint FeedbackVbo, FeedbackVao;
    
    ofEasyCam camera;
    
    static const int NUM = 1000000;
    struct Particle
    {
        ofVec3f position;
        ofVec3f accel;
        ofVec3f velocity;
    };
    
    Particle particle[NUM];
    
    //--------------------------------------------------------------
    void setup()
    {

        renderShader.load("shaders/render.vert", "shaders/render.frag");
        
        feedbackShader.setupShaderFromFile(GL_VERTEX_SHADER, "shaders/feedback.vert");
        const GLchar* feedbackVaryings[] = { "o_position", "o_accel", "o_velocity" };
        glTransformFeedbackVaryings(feedbackShader.getProgram(), 3, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
        feedbackShader.linkProgram();

        
        
        for(int i = 0; i < NUM; i++)
        {
            particle[i].position = ofVec3f(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100));
            particle[i].accel = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
            particle[i].velocity = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
        }
        
        
        glGenBuffers(1, &TransformVbo);
        
        glBindBuffer(GL_ARRAY_BUFFER, TransformVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * NUM, particle, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &FeedbackVbo);
        glBindBuffer(GL_ARRAY_BUFFER, FeedbackVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * NUM, nullptr, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
        
        
        glGenVertexArrays(1, &TransformVao);
        
        glBindVertexArray(TransformVao);
        glBindBuffer(GL_ARRAY_BUFFER, TransformVbo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(0 * sizeof(GLfloat)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(6 * sizeof(GLfloat)));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        
        
        glGenVertexArrays(1, &FeedbackVao);
        
        glBindVertexArray(FeedbackVao);
        glBindBuffer(GL_ARRAY_BUFFER, FeedbackVbo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(0 * sizeof(GLfloat)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(6 * sizeof(GLfloat)));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        
        //init TF object------
        glGenTransformFeedbacks(1, &TransformFeedback);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackVbo);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        
    }
    
    
    //--------------------------------------------------------------
    void update()
    {
        
        glEnable(GL_RASTERIZER_DISCARD);
        feedbackShader.begin();
        feedbackShader.setUniform1f("u_time", ofGetElapsedTimef());
        
        glBindVertexArray(TransformVao);
        
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackVbo);
        
        glBeginTransformFeedback(GL_POINTS);
            glDrawArrays(GL_POINTS, 0, NUM);
        glEndTransformFeedback();
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        
        feedbackShader.end();
        glDisable(GL_RASTERIZER_DISCARD);

        
        
    }
    
    
    //--------------------------------------------------------------
    void draw()
    {
        
        glEnable(GL_DEPTH_TEST);
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        camera.begin();
        renderShader.begin();
        glBindVertexArray(FeedbackVao);
        glDrawTransformFeedback(GL_POINTS, TransformFeedback);
        renderShader.end();
        camera.end();
        
        
        std::swap(TransformVbo, FeedbackVbo);
    }
    
    
    void exit()
    {
        glDeleteVertexArrays(1, &TransformVao);
        glDeleteBuffers(1, &TransformVbo);
        glDeleteVertexArrays(1, &FeedbackVao);
        glDeleteBuffers(1, &FeedbackVbo);
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
