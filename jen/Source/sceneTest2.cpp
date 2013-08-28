/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "sceneTest2.h"

#include "gameSystem.h"
#include "jMath.h"

sceneTest2::sceneTest2()
{
    name = new wchar_t[wcslen(L"plane") + 1];
    name = wcscpy(name, L"plane");
    model = new jmodel("Graphics\\plane.j3d");
}

sceneTest2::~sceneTest2()
{
    delete model;
}

void sceneTest2::onRender()
{
    // clear screen that works well with blending
    gl->Color(1.0, 1.0, 1.0, blendFactor);
    gl->Begin(GL_QUADS);
        gl->Vertex(1,1);
        gl->Vertex(1,-1);
        gl->Vertex(-1,-1);
        gl->Vertex(-1,1);
    gl->End();

    static float t = 0.0f;
    if((g->isBloomRender() && g->isBloomed()) || !(g->isBloomed()))
    {
        t += gls->getLastDT();
    }

    gl->MatrixMode(GL_PROJECTION);
    gl->LoadIdentity();
    gl->Perspective(60.0f, gls->getAspectRatio());
    gl->MatrixMode(GL_MODELVIEW);
    gl->LoadIdentity();
    gl->Clear(GL_DEPTH_BUFFER_BIT);
	
    gl->Translate(100.0f,0.0f,-350.0f);

    gl->Rotate(210.0f,1.0f,0.0f,0.0f);
	gl->Rotate(30.0f*t,0.0f,1.0f,0.0f);
	
	gl->Enable(GL_DEPTH_TEST);
	gl->DepthFunc(GL_LESS);

    gl->FrontFace(GL_CCW);
	gl->CullFace(GL_BACK);
	gl->Enable(GL_CULL_FACE);

    float lx[3] = { cos(cDEG2RAD((30*t+225))), -0.3f, sin(cDEG2RAD((30*t+225)))};
    float no = 1/sqrt(cDOT(lx,lx));
    lx[0] *= no;
    lx[1] *= no;
    lx[2] *= no;
	
	gl->Begin(GL_TRIANGLES);
		for(int i = 0; i < model->GetNumTris(); i++)
		{
            float b = model->GetNormal(i,0)->x*lx[0] + model->GetNormal(i,0)->y*lx[1] + model->GetNormal(i,0)->z*lx[2];
			gl->Color(b,b,b, blendFactor);
			model->glNormal(i,0);
			model->glVertex(i,0);
			model->glNormal(i,1);
			model->glVertex(i,1);
			model->glNormal(i,2);
			model->glVertex(i,2);
		}
	gl->End();

    gl->Disable(GL_CULL_FACE);
}