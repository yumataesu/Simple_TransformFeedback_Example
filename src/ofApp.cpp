#include "ofMain.h"

class ofApp : public ofBaseApp {
    
    ofShader renderShader, feedbackShader;
    GLuint TransformFeedback = 0;
    GLuint TransformVbo, TransformVao;
    GLuint FeedbackVbo, FeedbackVao;
    
    ofEasyCam camera;
    
    
    ofBufferObject TransformBuffer, FeedbackBuffer;
    ofVbo TransformVertexBuffer, FeedbackVertexBuffer;
    
    
    static const int NUM = 400000;
    struct Particle
    {
        ofVec3f position;
        ofVec3f accel;
        ofVec3f velocity;
        float lifetime;
    };
    
    Particle particle[NUM];
    
    //--------------------------------------------------------------
    void setup()
    {

        renderShader.load("shaders/render.vert", "shaders/render.frag");
        
        feedbackShader.setupShaderFromFile(GL_VERTEX_SHADER, "shaders/feedback.vert");
        feedbackShader.setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, "shaders/feedback.geom");
        const GLchar* feedbackVaryings[] = { "o_position", "o_accel", "o_velocity", "o_lifetime" };
        glTransformFeedbackVaryings(feedbackShader.getProgram(), 4, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
        feedbackShader.linkProgram();

        
        
        for(int i = 0; i < NUM; i++)
        {
            particle[i].position = ofVec3f(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500));
            particle[i].accel = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
            particle[i].velocity = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
            particle[i].lifetime = ofRandom(0, 10);
        }
        
        
        glGenBuffers(1, &TransformVbo);
        
        glBindBuffer(GL_ARRAY_BUFFER, TransformVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * NUM, particle, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &FeedbackVbo);
        glBindBuffer(GL_ARRAY_BUFFER, FeedbackVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * NUM, nullptr, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
//        TransformBuffer.allocate();
//        TransformBuffer.bind(GL_ARRAY_BUFFER);
//        TransformBuffer.setData(sizeof(Particle) * NUM, particle, GL_STREAM_COPY);
//        TransformBuffer.unbind(GL_ARRAY_BUFFER);
//        
//        FeedbackBuffer.allocate();
//        FeedbackBuffer.bind(GL_ARRAY_BUFFER);
//        FeedbackBuffer.setData(sizeof(Particle) * NUM, nullptr, GL_STREAM_COPY);
//        FeedbackBuffer.unbind(GL_ARRAY_BUFFER);
//        
//        
//        
//        TransformVertexBuffer.bind();
//        TransformVertexBuffer.setAttributeBuffer(0, TransformBuffer, 3, sizeof(Particle), 0);
//        TransformVertexBuffer.setAttributeBuffer(1, TransformBuffer, 3, sizeof(Particle), 12);
//        TransformVertexBuffer.setAttributeBuffer(2, TransformBuffer, 3, sizeof(Particle), 24);
//        TransformVertexBuffer.unbind();
//        
//        
//        FeedbackVertexBuffer.bind();
//        FeedbackVertexBuffer.setAttributeBuffer(0, TransformBuffer, 3, sizeof(Particle), 0);
//        FeedbackVertexBuffer.setAttributeBuffer(1, TransformBuffer, 3, sizeof(Particle), 12);
//        FeedbackVertexBuffer.setAttributeBuffer(2, TransformBuffer, 3, sizeof(Particle), 24);
//        FeedbackVertexBuffer.unbind();
        
        
        
        
        glGenVertexArrays(1, &TransformVao);
        
        glBindVertexArray(TransformVao);
        glBindBuffer(GL_ARRAY_BUFFER, TransformVbo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(0 * sizeof(GLfloat)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(6 * sizeof(GLfloat)));
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(9 * sizeof(GLfloat)));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        
        
        glGenVertexArrays(1, &FeedbackVao);
        
        glBindVertexArray(FeedbackVao);
        glBindBuffer(GL_ARRAY_BUFFER, FeedbackVbo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(0 * sizeof(GLfloat)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(6 * sizeof(GLfloat)));
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(9 * sizeof(GLfloat)));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        
        //init TF object------
        glGenTransformFeedbacks(1, &TransformFeedback);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackVbo);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        
        
        
            //init TF object------
//            glGenTransformFeedbacks(1, &TransformFeedback);
//            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
//            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackBuffer.getId());
//            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        
    }
    
    
    //--------------------------------------------------------------
    void update()
    {
        
        float time = ofGetElapsedTimef();
        //camera.orbit(50 * time, 0, 2000);
        
        glEnable(GL_RASTERIZER_DISCARD);
        feedbackShader.begin();
        feedbackShader.setUniform1f("u_time", time);
        
        glBindVertexArray(TransformVao);
        
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackVbo);
        
        glBeginTransformFeedback(GL_POINTS);
            glDrawArraysInstanced(GL_POINTS, 0, NUM, 1);
        glEndTransformFeedback();
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        
        feedbackShader.end();
        glDisable(GL_RASTERIZER_DISCARD);
        
        
//        glEnable(GL_RASTERIZER_DISCARD);
//        feedbackShader.begin();
//        feedbackShader.setUniform1f("u_time", ofGetElapsedTimef());
//        
//        //glBindVertexArray(TransformVao);
//        
//        
//        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
//        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackBuffer.getId());
//        
//        glBeginTransformFeedback(GL_POINTS);
//        //glDrawArraysInstanced(GL_POINTS, 0, NUM, 1);
//        
//        TransformVertexBuffer.drawInstanced(GL_POINTS, 0, NUM, 1);
//        glEndTransformFeedback();
//        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
//        
//        feedbackShader.end();
//        glDisable(GL_RASTERIZER_DISCARD);


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
        //TransformVertexBuffer.bind();
        glBindVertexArray(FeedbackVao);
        glDrawTransformFeedback(GL_POINTS, TransformFeedback);
        //TransformVertexBuffer.unbind();
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
    settings.width = 1280;
    settings.height = 720;
    settings.resizable = false;
    ofCreateWindow(settings);
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
}
