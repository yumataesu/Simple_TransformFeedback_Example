#include "ofMain.h"

class ofApp : public ofBaseApp {
    
    ofShader renderShader, feedbackShader;
    GLuint TransformFeedback = 0;
    
    ofEasyCam camera;
    
    
    ofBufferObject TransformBuffer, FeedbackBuffer;
    ofVbo TransformVertexBuffer, FeedbackVertexBuffer;
    
    
    static const int NUM = 50000;
    struct Particle
    {
        ofVec3f position;
        ofVec3f accel;
        ofVec3f velocity;
        float lifetime;
        ofVec3f color;
    };
    
    Particle particle[NUM];
    ofTexture texture;
    
    
    //--------------------------------------------------------------
    void setup()
    {

        renderShader.load("shaders/render.vert", "shaders/render.frag");
        
        feedbackShader.setupShaderFromFile(GL_VERTEX_SHADER, "shaders/feedback.vert");
        feedbackShader.setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, "shaders/feedback.geom");
        const GLchar* feedbackVaryings[] = { "o_position", "o_accel", "o_velocity", "o_lifetime", "o_color" };
        glTransformFeedbackVaryings(feedbackShader.getProgram(), 5, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
        feedbackShader.linkProgram();
        
        ofDisableArbTex();
        ofLoadImage(texture, "tex/dot.png");
        ofEnableArbTex();
        
        
        renderShader.begin();
        renderShader.setUniformTexture("tex", texture, 0);
        renderShader.end();
        
        
        
        for(int i = 0; i < NUM; i++)
        {
            particle[i].position = ofVec3f(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500));
            particle[i].accel = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
            particle[i].velocity = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
            particle[i].lifetime = ofRandom(0, 10);
            particle[i].color = ofVec3f(ofRandom(0, 1), ofRandom(0, 1), ofRandom(0, 1));
        }
        
        
        
        //Create and init Vbo
        TransformBuffer.allocate();
        TransformBuffer.bind(GL_ARRAY_BUFFER);
        TransformBuffer.setData(sizeof(Particle) * NUM, particle, GL_STREAM_COPY);
        TransformBuffer.unbind(GL_ARRAY_BUFFER);
        
        FeedbackBuffer.allocate();
        FeedbackBuffer.bind(GL_ARRAY_BUFFER);
        FeedbackBuffer.setData(sizeof(Particle) * NUM, nullptr, GL_STREAM_COPY);
        FeedbackBuffer.unbind(GL_ARRAY_BUFFER);
        
        
        //create and init Vao (interleaved)
        TransformVertexBuffer.bind();
        TransformVertexBuffer.setAttributeBuffer(0, TransformBuffer, 3, sizeof(Particle), 0);
        TransformVertexBuffer.setAttributeBuffer(1, TransformBuffer, 3, sizeof(Particle), 12);
        TransformVertexBuffer.setAttributeBuffer(2, TransformBuffer, 3, sizeof(Particle), 24);
        TransformVertexBuffer.unbind();
        
        
        FeedbackVertexBuffer.bind();
        FeedbackVertexBuffer.setAttributeBuffer(0, TransformBuffer, 3, sizeof(Particle), 0);
        FeedbackVertexBuffer.setAttributeBuffer(1, TransformBuffer, 3, sizeof(Particle), 12);
        FeedbackVertexBuffer.setAttributeBuffer(2, TransformBuffer, 3, sizeof(Particle), 24);
        FeedbackVertexBuffer.unbind();
        
        
        
        //create and init TF
        glGenTransformFeedbacks(1, &TransformFeedback);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackBuffer.getId());
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    }
    
    
    //--------------------------------------------------------------
    void update()
    {
        
        float time = ofGetElapsedTimef();
        //camera.orbit(50 * time, 0, 2000);
        
        glEnable(GL_RASTERIZER_DISCARD);
        feedbackShader.begin();
        feedbackShader.setUniform1f("u_time", ofGetElapsedTimef());
        
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TransformFeedback);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, FeedbackBuffer.getId());
        glBeginTransformFeedback(GL_POINTS);
            TransformVertexBuffer.drawInstanced(GL_POINTS, 0, NUM, 1);
        glEndTransformFeedback();
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        
        feedbackShader.end();
        glDisable(GL_RASTERIZER_DISCARD);

    }
    
    
    //--------------------------------------------------------------
    void draw()
    {
        //glEnable(GL_DEPTH_TEST);
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        

        
        
        camera.begin();
        glEnable(GL_POINT_SPRITE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        
        renderShader.begin();
        FeedbackVertexBuffer.bind();
        glDrawTransformFeedback(GL_POINTS, TransformFeedback);
        FeedbackVertexBuffer.unbind();
        renderShader.end();
//        ofDisablePointSprites();
        camera.end();
        
        std::swap(TransformBuffer, FeedbackBuffer);
    }
    
    
    void exit()
    {
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
