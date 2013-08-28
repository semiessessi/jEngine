/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "sceneBlack.h"

sceneBlack::sceneBlack()
{
    name = new wchar_t[wcslen(L"black") + 1];
    name = wcscpy(name, L"black");
}

void sceneBlack::onRender()
{
    gl->Color(0.0, 0.0, 0.0, blendFactor);
    gl->Begin(GL_QUADS);
        gl->Vertex(1,1);
        gl->Vertex(1,-1);
        gl->Vertex(-1,-1);
        gl->Vertex(-1,1);
    gl->End();

    gl->Disable(GL_BLEND);
}