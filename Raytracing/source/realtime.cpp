////////////////////////////////////////////////////////////////////////////////
// Temporary code.  Remove this from your raytracer.  This displays
// the contents of a scene file in realtime in a GLUT window.
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <fstream>
#include <vector>

#include "geom.h"
#include "raytrace.h"
#include "realtime.h"

// Stupid C++ needs callbacks to be static functions.
static Realtime* globalRealtime = nullptr;
void CBDrawScene()  { globalRealtime->DrawScene(); } 
void CBReshapeWindow(int w, int h)  { globalRealtime->ReshapeWindow(w,h); }
void CBKeyboardDown(unsigned char key, int x, int y)  { globalRealtime->KeyboardDown(key, x, y); }
void CBKeyboardUp(unsigned char key, int x, int y)  { globalRealtime->KeyboardUp(key, x, y); } 
void CBMouseButton(int button, int state, int x, int y)  { globalRealtime->MouseButton(button, state, x, y); } 
void CBMouseMotion(int x, int y)  { globalRealtime->MouseMotion(x,y); }
void CBAnimate(int value)
{
    glutTimerFunc(30, CBAnimate, 1);
    // atime = 360.0*glutGet(GLUT_ELAPSED_TIME)/12000;
    glutPostRedisplay();
}

unsigned int MakeVAO(MeshData* meshdata)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    
    if (meshdata && meshdata->vertices.size() == 0) {
        std::cerr << "Missing meshdata->vertices in MakeVAO\n";
        exit(-1); }
    if (meshdata && meshdata->triangles.size() == 0) {
        std::cerr << "Missing meshdata->triangles in MakeVAO\n";
        exit(-1); }
    
    GLuint Pbuff;
    glGenBuffers(1, &Pbuff);
    glBindBuffer(GL_ARRAY_BUFFER, Pbuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*11*meshdata->vertices.size(), 
                 &(meshdata->vertices[0].pnt.x()), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(3*sizeof(float)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(6*sizeof(float)));
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(8*sizeof(float)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    GLuint Ibuff;
    glGenBuffers(1, &Ibuff);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*3*meshdata->triangles.size(),
                 &(meshdata->triangles[0].x()), GL_STATIC_DRAW);
    
    glBindVertexArray(0);

    return vao;
}

MeshData* SphMesh()
{
    MeshData* meshdata = new MeshData();
    unsigned int n = 20;
    float d = 2.0f*PI/float(n*2);
    for (unsigned int i=0;  i<=n*2;  i++) {
        float s = i*2.0f*PI/float(n*2);
        for (unsigned int j=0;  j<=n;  j++) {
            float t = j*PI/float(n);
            float x = cos(s)*sin(t);
            float y = sin(s)*sin(t);
            float z = cos(t);
            meshdata->vertices.push_back(VertexData(Vector3f(x,y,z),
                                                    Vector3f(x,y,z),
                                                    Vector2f(s/(2*PI), t/PI),
                                                    Vector3f(sin(s), cos(s), 0.0)));
            if (i>0 && j>0) {
                meshdata->triangles.push_back(TriData((i-1)*(n+1) + (j-1), 
                                                      (i-1)*(n+1) + (j  ), 
                                                      (i  )*(n+1) + (j  )));
                meshdata->triangles.push_back(TriData((i-1)*(n+1) + (j-1),
                                                      (i  )*(n+1) + (j  ),
                                                      (i  )*(n+1) + (j-1))); } } }
    return meshdata;
}

MeshData* BoxMesh()
{
    Matrix4f face[6] = {
        Matrix4f::Identity(),
        rotate(180.0f*Radians, Vector3f(1.0f, 0.0f, 0.0f)),
        rotate( 90.0f*Radians, Vector3f(1.0f, 0.0f, 0.0f)),
        rotate(-90.0f*Radians, Vector3f(1.0f, 0.0f, 0.0f)),
        rotate( 90.0f*Radians, Vector3f(0.0f, 1.0f, 0.0f)),
        rotate(-90.0f*Radians, Vector3f(0.0f, 1.0f, 0.0f))};
       
    Matrix4f half = translate(Vector3f(0.5f, 0.5f, 0.5f))*scale(Vector3f(0.5f, 0.5f, 0.5f));
    MeshData* meshdata = new MeshData();
    for (unsigned int f=0;  f<6;  f++) {
        Matrix4f m4 = half*face[f];
        Matrix3f m3 = m4.block<3,3>(0,0);
        for (unsigned int i=0;  i<2;  i++) {
            for (unsigned int j=0;  j<2;  j++) {
              Vector4f p = m4*Vector4f(float(2*i)-1.0f, float(2*j)-1.0f, 1.0f, 1.0f);
              Vector3f tnrm = m3*Vector3f(0.0f, 0.0f, 1.0f);
              Vector3f ttan = m3*Vector3f(1.0, 0.0, 0.0);
              meshdata->vertices.push_back(VertexData(Vector3f(p[0], p[1], p[2]),
                                                      Vector3f(tnrm[0], tnrm[1], tnrm[2]),
                                                      Vector2f(float(i), float(j)),
                                                      Vector3f(ttan[0], ttan[1], ttan[2])));
              meshdata->triangles.push_back(TriData(4*f+0, 4*f+1, 4*f+3));
              meshdata->triangles.push_back(TriData(4*f+0, 4*f+3, 4*f+2)); } } }
    return meshdata;
}

MeshData* CylMesh()
{
    MeshData* meshdata = new MeshData();
    unsigned int n = 20;
    float d = 2.0f*PI/float(n*2);
    for (unsigned int i=0;  i<=n;  i++) {
        float s = i*2.0f*PI/float(n);
        float x = cos(s);
        float y = sin(s);
        
        meshdata->vertices.push_back(VertexData(Vector3f(x, y, 0.0f),
                                                Vector3f(x, y, 0.0f),
                                                Vector2f(s/(2*PI), 0.0f),
                                                Vector3f(-sin(s), cos(s), 0.0f)));

        meshdata->vertices.push_back(VertexData(Vector3f(x, y, 1.0f),
                                                Vector3f(x, y, 0.0f),
                                                Vector2f(s/(2*PI), 0.0f),
                                                Vector3f(-sin(s), cos(s), 0.0f)));

        if (i>0) {
            meshdata->triangles.push_back(TriData((i-1)*2+1, (i-1)*2, (i  )*2));
            meshdata->triangles.push_back(TriData((i-1)*2+1, (i  )*2, (i  )*2+1)); } }
    return meshdata;
}

////////////////////////////////////////////////////////////////////////
// Shader programming class;  Encapsulates a OpenGL Shader.
////////////////////////////////////////////////////////////////////////
void ShaderProgram::CreateShader(const std::string fname, const GLenum type)
{
    // Read a file into a string
    std::ifstream f;
    f.open(fname, std::ios_base::binary); // Open
    f.seekg(0, std::ios_base::end);       // Position at end
    int length = f.tellg();               // to get the length
    
    char* src = new char [length+1];  // Create buffer of needed length
    f.seekg (0, std::ios_base::beg);      // Position at beginning
    f.read (src, length);             //   to read complete file
    f.close();                            // Close
    
    src[length] = char(0);            // Finish with a NULL
    
    // Create a shader, attach, send it the source, and compile it.
    int shader = glCreateShader(type);
    glAttachShader(program, shader);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    // Get the compilation status
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    
    // If compilation status is not OK, get and print the log message.
    if (status != 1) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, buffer);
        printf("Compile log(%s):\n%s\n", type==GL_VERTEX_SHADER?"Vertex":"Fragment", buffer);
        delete buffer;
    }
}

void ShaderProgram::LinkProgram()
{
    // Link program and check the status
    glLinkProgram(program);
    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    
    // If link failed, get and print log
    if (status != 1) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* buffer = new char[length];
        glGetProgramInfoLog(program, length, nullptr, buffer);
        printf("Link log:\n%s\n", buffer);
        delete buffer;
    }
}


void applyMaterial(Material* mat, const unsigned int program)
{
    int loc = glGetUniformLocation(program, "Kd");
    glUniform3fv(loc, 1, &mat->Kd[0]);
    
    loc = glGetUniformLocation(program, "Ks");
    glUniform3fv(loc, 1, &mat->Ks[0]);
    
    loc = glGetUniformLocation(program, "alpha");
    glUniform1f(loc, mat->alpha);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat->texid);
    loc = glGetUniformLocation(program, "tex");
    glUniform1i(loc, 0);
    
    loc = glGetUniformLocation(program, "emitter");
    glUniform1i(loc, 0);
}

////////////////////////////////////////////////////////////////////////
// Light: encapsulates a light and communiction with a shader.
////////////////////////////////////////////////////////////////////////
void applyLight(Material* mat, const unsigned int program)
{
    Vector3f Z;
    
    int loc = glGetUniformLocation(program, "Kd");
    glUniform3fv(loc, 1, &mat->Kd[0]);
    
    loc = glGetUniformLocation(program, "emitter");
    glUniform1i(loc, 1);
}

////////////////////////////////////////////////////////////////////////
// Obj: encapsulates objects to be drawn; uses OpenGL's VAOs
////////////////////////////////////////////////////////////////////////
Obj::Obj(MeshData* m, const Matrix4f& tr, Material* b)
    : meshdata(m), modelTR(tr), material(b)
{
    Vector4f sum(0,0,0,0);
    //for (int i=0;  i<meshdata->vertices.size();  i++)
    //    sum += modelTR*Vector4f(v.pnt[0], v.pnt[1], v.pnt[2], 1.0);
    for (auto v : meshdata->vertices) // C++11 for loop
        sum += modelTR*Vector4f(v.pnt[0], v.pnt[1], v.pnt[2], 1.0);

    center = (sum/meshdata->vertices.size()).block<3,1>(0,0); 
    
    std::cout << "center: " << center.transpose() << std::endl;
    
    vao = MakeVAO(meshdata);
}

void Obj::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3*meshdata->triangles.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////
// Realtime handles all realtime drawing/interaction
////////////////////////////////////////////////////////////////////////

// Constructor for Realtime.  Initializes OpenGL, GLUT,as well as the
// data elements of the class.
Realtime::Realtime()
{   
    // Initialize the OpenGL bindings
    glbinding::Binding::initialize(false);

    globalRealtime = this;
    // Initialize GLUT
    int argc = 0;
    char* argv;
    
    glutInit(&argc, &argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitContextVersion (3, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitWindowSize(200, 200);
    glutCreateWindow("Class Framework");
    glutSetOption((GLenum)GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glutIgnoreKeyRepeat(true);
    glutDisplayFunc(&CBDrawScene);
    glutReshapeFunc(&CBReshapeWindow);
    glutKeyboardFunc(&CBKeyboardDown);
    glutKeyboardUpFunc(&CBKeyboardUp);
    glutMouseFunc(&CBMouseButton);
    glutMotionFunc(&CBMouseMotion); 
    glutTimerFunc(30, CBAnimate, 1);

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("Rendered by: %s\n", glGetString(GL_RENDERER));
    fflush(stdout);
    
    // Create the shader program
    lighting.CreateProgram();
    lighting.CreateShader("realtime.vert", GL_VERTEX_SHADER);
    lighting.CreateShader("realtime.frag", GL_FRAGMENT_SHADER);

    glBindAttribLocation(lighting.program, 0, "vertex");
    glBindAttribLocation(lighting.program, 1, "vertexNormal");
    glBindAttribLocation(lighting.program, 2, "vertexTexture");
    glBindAttribLocation(lighting.program, 3, "vertexTangent");
    lighting.LinkProgram();

    // Several generic meshes which can be transfofrmed to *any* sphere, box, or cylinder.
    sphMesh = SphMesh();
    boxMesh = BoxMesh();
    cylMesh = CylMesh();

    // Initialize various member attributes
    //materials.push_back(new Material());

    nav = false;
    spin = 0.0f;
    tilt = 90.0f;
    speed = 0.05f;
    front = 0.1f;
    back = 1000.0f;

    shifted = false;
    leftDown = false;
    middleDown = false;
    rightDown = false;
    motionkey = 0;

    ambient = Vector3f(0.2, 0.2, 0.2);    
}

// This function enters the event loop.
void Realtime::run()
{
    for (int i=0;  i<lights.size();  i++) {
        lightPosn[i] = lights[i]->Center();
        float d = lightPosn[i].norm();
        lightEmit[i] = lights[i]->material->Kd*lights[i]->area/(d*d)/3.14;
        printf("light %d: (%f %f %f) (%f %f %f) %f\n", i,
               lightEmit[i][0], lightEmit[i][1], lightEmit[i][2],
               lightPosn[i][0], lightPosn[i][1], lightPosn[i][2],
               lights[i]->area); }
    
    cDist = eye.norm();
    glutReshapeWindow(width, height);
    glutMainLoop();
}
// Called when the scene needs to be redrawn.
void Realtime::DrawScene()
{
    Vector3f viewDir = ViewDirection();
    Vector2f dir2 = Vector2f(viewDir.x(), viewDir.y()).normalized();
    if (motionkey == 'w')
        eye += speed*Vector3f(dir2.x(), dir2.y(), 0.0);
    if (motionkey == 's')
        eye -= speed*Vector3f(dir2.x(), dir2.y(), 0.0);
    if (motionkey == 'd')
        eye += speed*Vector3f(dir2.y(), -dir2.x(), 0.0);
    if (motionkey == 'a')
        eye -= speed*Vector3f(dir2.y(), -dir2.x(), 0.0);
    if (motionkey == 'e')
        eye -= speed*Vector3f(0.0f, 0.0f, -1.0f);
    if (motionkey == 'c')
        eye -= speed*Vector3f(0.0f, 0.0f, 1.0f);
    
    int loc;

    glClearColor(0.3,0.3, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    Matrix4f WorldView;
    Matrix4f R = toMat4(ViewQuaternion().conjugate());
    WorldView = R*translate(-eye);

    float rx = (ry*width)/height;
    Matrix4f WorldProj = frustum(-front*rx, front*rx, -front*ry, front*ry, front, back);

    lighting.Use();

    loc = glGetUniformLocation(lighting.program, "WorldProj");
    glUniformMatrix4fv(loc, 1, GL_FALSE, WorldProj.data());

    loc = glGetUniformLocation(lighting.program, "WorldView");
    glUniformMatrix4fv(loc, 1, GL_FALSE, WorldView.data());

    loc = glGetUniformLocation(lighting.program, "ambient");
    glUniform3fv(loc, 1, &ambient[0]);

    loc = glGetUniformLocation(lighting.program, "eyePos");
    glUniform3fv(loc, 1, &eye[0]);

    loc = glGetUniformLocation(lighting.program, "lightNum");
    glUniform1i(loc, lights.size());

    loc = glGetUniformLocation(lighting.program, "lightPosn");
    glUniform3fv(loc, 8, &lightPosn[0][0]);

    loc = glGetUniformLocation(lighting.program, "lightEmit");
    glUniform3fv(loc, 8, &lightEmit[0][0]);
        
    // for (unsigned int i=0;  i<objs.size();  i++) {
    //     Material* material = objs[i]->material;
    //     Matrix4f& modelTR = objs[i]->modelTR;
    //     ...
    //     objs[i]->draw();
    for (auto obj : objs) {     // C++11 loop
        Material* material = obj->material;
        Matrix4f& modelTR = obj->modelTR;
        Matrix3f normalTR = modelTR.inverse().block<3,3>(0,0);

        loc = glGetUniformLocation(lighting.program, "ModelTr");
        glUniformMatrix4fv(loc, 1, GL_FALSE, modelTR.data());

        loc = glGetUniformLocation(lighting.program, "normalTR");
        glUniformMatrix3fv(loc, 1, GL_FALSE, normalTR.data());

        if (!material) {
            std::cerr << "No material associated with object\n";
            exit(-1); }
        if (material->isLight())
            applyLight(material, lighting.program);
        else
            applyMaterial(material, lighting.program);
        obj->draw(); }

    lighting.Unuse();
    glutSwapBuffers();
}

// Called by GLUT when the window size is changed.
void Realtime::ReshapeWindow(int w, int h)
{
    if (w && h)
        glViewport(0, 0, w, h);
    width = w;
    height = h;

    // Force a redraw
    glutPostRedisplay();
}

// Called by GLUT for keyboard actions.
void Realtime::KeyboardDown(unsigned char key, int x, int y)
{
    printf("key: %c\n", key);
    switch(key) {
    case 9:
        nav = !nav;
        break;

    case 'v': {
        Quaternionf q = ViewQuaternion();
        printf("camera  %g %g %g   %g   q %g %g %g %g\n",
               eye[0], eye[1], eye[2], ry,  q.w(), q.x(), q.y(), q.z());
        printf("screen %d %d\n", width, height);
        fflush(stdout); }
        break;

    case 'w': case 's': case 'a': case 'd': case 'e': case 'c':
        motionkey = key;
        break;
        
    case 27:                    // Escape key
    case 'q':
        glutLeaveMainLoop();
        break;
    }
}

void Realtime::KeyboardUp(unsigned char key, int x, int y)
{
    motionkey = 0;
    fflush(stdout);
}

// Called by GLut when a mouse button changes state.
void Realtime::MouseButton(int button, int state, int x, int y)
{        
    // Record the position of the mouse click.
    mouseX = x;
    mouseY = y;

    // Test if the SHIFT keey was down for this mouse click
    shifted = glutGetModifiers() && GLUT_ACTIVE_SHIFT;

    // Ignore high order bits, set by some (stupid) GLUT implementation.
    button = button%8;

    if (button == GLUT_LEFT_BUTTON) {
        leftDown = (state == GLUT_DOWN); }

    else if (button == GLUT_MIDDLE_BUTTON) {
        middleDown = (state == GLUT_DOWN); }

    else if (button == GLUT_RIGHT_BUTTON) {
        rightDown = (state == GLUT_DOWN); }

    else if (button == 3) {
        Vector3f C = eye + cDist*ViewDirection();
        cDist = pow(cDist, 1.0f/1.02f); 
        eye = C - cDist*ViewDirection(); }

    else if (button == 4) {
        Vector3f C = eye + cDist*ViewDirection();
        cDist = pow(cDist, 1.02f);
        eye = C - cDist*ViewDirection(); }
    
    // Force a redraw
    glutPostRedisplay();
    fflush(stdout);
}

void Realtime::MouseMotion(int x, int y)
{
    // Calculate the change in the mouse position
    int dx = x-mouseX;
    int dy = y-mouseY;

    if (leftDown) {        // Rotate light position
        if (nav) {
            spin += dx/2.0f;
            tilt += dy/2.0f; }
        else {
            Vector3f C = eye + cDist*ViewDirection();
            spin += dx/2.0f;
            tilt += dy/2.0f;
            eye = C - cDist*ViewDirection(); } }

    else if (middleDown) { }

    // Record this position
    mouseX = x;
    mouseY = y;

    // Force a redraw
    glutPostRedisplay();
}


void Realtime::sphere(const Vector3f center, const float r, Material* mat)
{
    Matrix4f m = translate(center) * scale(Vector3f(r,r,r));
    Vector3f rrr(r,r,r);
    Obj* obj = new Obj(sphMesh, m, mat);
    obj->area = 4*PI*r*r;
    objs.push_back(obj);
    if (mat->isLight())
        lights.push_back(obj);
}

void Realtime::box(const Vector3f base, const Vector3f diag, Material* mat)
{
    Matrix4f m = translate(base) * scale(Vector3f(diag[0],diag[1],diag[2]));
    Obj* obj = new Obj(boxMesh, m, mat);
    objs.push_back(obj);
    if (mat->isLight())
        lights.push_back(obj);
}


void Realtime::cylinder(const Vector3f base, const Vector3f axis, const float radius, Material* mat)
{
    Vector3f Z(0.0f, 0.0f, 1.0f);
    Vector3f C = axis.normalized();
    Vector3f B = C.cross(Z);
    if (B.norm() <1e-8)
        B = Vector3f(0,1,0);
    else
        B = B.normalized();
    Vector3f A = B.cross(C).normalized();
    Matrix4f R;
    R<< A(0),B(0),C(0),0.0f,    // Row-wise text (although init is col-wise)
        A(1),B(1),C(1),0.0f,
        A(2),B(2),C(2),0.0f,
        0.0f, 0.0f, 0.0f, 1.0;

    Matrix4f m = translate(base)*R*scale(Vector3f(radius,radius,axis.norm()));
    Vector3f rrr(radius,radius,radius);
    Obj* obj = new Obj(cylMesh, m, mat);
    objs.push_back(obj);
    if (mat->isLight())
        lights.push_back(obj);
}
