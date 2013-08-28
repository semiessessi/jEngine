/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "sceneTest1.h"

sceneTest1::sceneTest1()
{
    name = new wchar_t[wcslen(L"test1") + 1];
    name = wcscpy(name, L"test1");
}

void sceneTest1::onRender()
{
    // clear screen that works well with blending
    gl->Color(0.0, 0.0, 0.0, blendFactor);
    gl->Begin(GL_QUADS);
        gl->Vertex(1,1);
        gl->Vertex(1,-1);
        gl->Vertex(-1,-1);
        gl->Vertex(-1,1);
    gl->End();

    glClear(GL_DEPTH_BUFFER_BIT);

    gl->Color(1.0, 1.0, 1.0, blendFactor);
    gl->Begin(GL_QUADS);
        gl->Vertex(0.5f,0.5f);
        gl->Vertex(0.5f,-0.5f);
        gl->Vertex(-0.5f,-0.5f);
        gl->Vertex(-0.5f,0.5f);
    gl->End();

    gl->Disable(GL_BLEND);
}