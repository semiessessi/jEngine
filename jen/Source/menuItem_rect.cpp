/* menuItem class by Semi Essessi
 *
 * Base menu item
 *
 */

#include "menuItem.h"

#include "menuCom.h"
#include "menuScriptRoutine.h"
#include "menuSystem.h"
#include "menuVar.h"

#include "glSystem.h"
#include "glTextRender.h"
#include "jEngine.h"
#include "jMath.h"
#include "log.h"

#include <malloc.h>
#include <wchar.h>

void menuItem::renNoneNoneRect(menuItem* p)
{
    glSystem::glFuncs* gl = p->gl;

	gl->ClearDepth(0.0);
	gl->Clear(GL_DEPTH_BUFFER_BIT);
	
	glColor4fv(p->getFGColor());

	// render clipping rectangle
	float x1 = 0;
	float y1 = 0;
	float x2 = p->getXSize();
	float y2 = p->getYSize();

	if(p->getParent())
	{
		x2 = max(0, min(x2, p->getParent()->getXSize() - p->getXPos()));
		y2 = max(0, min(y2, p->getParent()->getYSize() - p->getYPos()));
	}
	
	gl->DepthFunc(GL_ALWAYS);
	gl->Enable(GL_BLEND);	
	gl->BlendFunc(GL_ZERO, GL_ONE);

	gl->Begin(GL_QUADS);
		gl->Vertex(x1, y1, CLIP_Z);
		gl->Vertex(x2, y1, CLIP_Z);
		gl->Vertex(x2, y2, CLIP_Z);
		gl->Vertex(x1, y2, CLIP_Z);
	gl->End();

	glDisable(GL_BLEND);

	gl->DepthFunc(GL_LESS);

	// render contents
	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renSingleNoneRect(menuItem* p)
{
    glSystem::glFuncs* gl = p->gl;

	gl->ClearDepth(0.0);
	gl->Clear(GL_DEPTH_BUFFER_BIT);
	
	glColor4fv(p->getFGColor());

	// render first clipping rectangle
	float x1 = -2;
	float y1 = -2;
	float x2 = p->getXSize()+2;
	float y2 = p->getYSize()+2;

	if(p->getParent())
	{
		x2 = max(-2, min(x2, p->getParent()->getXSize() - p->getXPos() + 2));
		y2 = max(-2, min(y2, p->getParent()->getYSize() - p->getYPos() + 2));
	}
	
	gl->DepthFunc(GL_ALWAYS);
	gl->Enable(GL_BLEND);	
	gl->BlendFunc(GL_ZERO, GL_ONE);

	gl->Begin(GL_QUADS);
		gl->Vertex(x1, y1, CLIP_Z);
		gl->Vertex(x2, y1, CLIP_Z);
		gl->Vertex(x2, y2, CLIP_Z);
		gl->Vertex(x1, y2, CLIP_Z);
	gl->End();

	gl->Disable(GL_BLEND);

	gl->DepthFunc(GL_LESS);

	float x3 = -1;
	float y3 = -1;
	float x4 = p->getXSize()+1;
	float y4 = p->getYSize()+1;
	
	// render border
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
	gl->End();

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// render smaller clipping rectangle
	gl->DepthFunc(GL_ALWAYS);
	gl->Enable(GL_BLEND);	
	gl->BlendFunc(GL_ZERO, GL_ONE);

	gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	gl->End();

	x1 += 2;
	y1 += 2;
	x2 -= 2;
	y2 -= 2;

	gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	gl->End();

	glDisable(GL_BLEND);

	gl->DepthFunc(GL_LESS);

	// render contents
	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renDoubleNoneRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glColor4fv(p->getFGColor());

	// render first clipping rectangle
	float x1 = -7;
	float y1 = -7;
	float x2 = p->getXSize()+7;
	float y2 = p->getYSize()+7;

	if(p->getParent())
	{
		x2 = max(-2, min(x2, p->getParent()->getXSize() - p->getXPos() + 2));
		y2 = max(-2, min(y2, p->getParent()->getYSize() - p->getYPos() + 2));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = -1;
	float y3 = -1;
	float x4 = p->getXSize()+1;
	float y4 = p->getYSize()+1;
	
	// render border
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
	p->gl->End();

	x3 -= 5;
	y3 -= 5;
	x4 += 5;
	y4 += 5;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
	p->gl->End();

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += 7;
	y1 += 7;
	x2 -= 7;
	y2 -= 7;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	// render contents
	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelInNoneRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -BEVEL_WIDTH;
	float y1 = -BEVEL_WIDTH;
	float x2 = p->getXSize()+BEVEL_WIDTH;
	float y2 = p->getYSize()+BEVEL_WIDTH;

	if(p->getParent())
	{
		x2 = max(-BEVEL_WIDTH, min(x2, p->getParent()->getXSize() - p->getXPos() + BEVEL_WIDTH));
		y2 = max(-BEVEL_WIDTH, min(y2, p->getParent()->getYSize() - p->getYPos() + BEVEL_WIDTH));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;
	
	glColor4fv(darkCol);

	// render border
	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(lightCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += BEVEL_WIDTH;
	y1 += BEVEL_WIDTH;
	x2 -= BEVEL_WIDTH;
	y2 -= BEVEL_WIDTH;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	// render contents
	glColor4fv(col);

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelOutNoneRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -BEVEL_WIDTH;
	float y1 = -BEVEL_WIDTH;
	float x2 = p->getXSize()+BEVEL_WIDTH;
	float y2 = p->getYSize()+BEVEL_WIDTH;

	if(p->getParent())
	{
		x2 = max(-BEVEL_WIDTH, min(x2, p->getParent()->getXSize() - p->getXPos() + BEVEL_WIDTH));
		y2 = max(-BEVEL_WIDTH, min(y2, p->getParent()->getYSize() - p->getYPos() + BEVEL_WIDTH));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;
	
	glColor4fv(lightCol);

	// render border
	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(darkCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += BEVEL_WIDTH;
	y1 += BEVEL_WIDTH;
	x2 -= BEVEL_WIDTH;
	y2 -= BEVEL_WIDTH;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	// render contents
	glColor4fv(col);

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelBothInNoneRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -(BEVEL_WIDTH*3+1);
	float y1 = -(BEVEL_WIDTH*3+1);
	float x2 = p->getXSize()+(BEVEL_WIDTH*3+1);
	float y2 = p->getYSize()+(BEVEL_WIDTH*3+1);

	if(p->getParent())
	{
		x2 = max(-(BEVEL_WIDTH*3+1), min(x2, p->getParent()->getXSize() - p->getXPos() + (BEVEL_WIDTH*3+1)));
		y2 = max(-(BEVEL_WIDTH*3+1), min(y2, p->getParent()->getYSize() - p->getYPos() + (BEVEL_WIDTH*3+1)));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;

	// render border
	glColor4fv(lightCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(darkCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(col);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(darkCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(lightCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += (BEVEL_WIDTH*3+1);
	y1 += (BEVEL_WIDTH*3+1);
	x2 -= (BEVEL_WIDTH*3+1);
	y2 -= (BEVEL_WIDTH*3+1);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	// render contents
	glColor4fv(col);

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelBothOutNoneRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -(BEVEL_WIDTH*3+1);
	float y1 = -(BEVEL_WIDTH*3+1);
	float x2 = p->getXSize()+(BEVEL_WIDTH*3+1);
	float y2 = p->getYSize()+(BEVEL_WIDTH*3+1);

	if(p->getParent())
	{
		x2 = max(-(BEVEL_WIDTH*3+1), min(x2, p->getParent()->getXSize() - p->getXPos() + (BEVEL_WIDTH*3+1)));
		y2 = max(-(BEVEL_WIDTH*3+1), min(y2, p->getParent()->getYSize() - p->getYPos() + (BEVEL_WIDTH*3+1)));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;
	
	// render border
	glColor4fv(darkCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(lightCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(col);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(lightCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(darkCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += (BEVEL_WIDTH*3+1);
	y1 += (BEVEL_WIDTH*3+1);
	x2 -= (BEVEL_WIDTH*3+1);
	y2 -= (BEVEL_WIDTH*3+1);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	// render contents
	glColor4fv(col);

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renNonePlainRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render clipping rectangle
	float x1 = 0;
	float y1 = 0;
	float x2 = p->getXSize();
	float y2 = p->getYSize();

	if(p->getParent())
	{
		x2 = max(0, min(x2, p->getParent()->getXSize() - p->getXPos()));
		y2 = max(0, min(y2, p->getParent()->getYSize() - p->getYPos()));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	// render background
	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	glColor4fv(p->getFGColor());

	// render contents
	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renSinglePlainRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	glColor4fv(p->getFGColor());

	// render first clipping rectangle
	float x1 = -2;
	float y1 = -2;
	float x2 = p->getXSize()+2;
	float y2 = p->getYSize()+2;

	if(p->getParent())
	{
		x2 = max(-2, min(x2, p->getParent()->getXSize() - p->getXPos() + 2));
		y2 = max(-2, min(y2, p->getParent()->getYSize() - p->getYPos() + 2));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = -1;
	float y3 = -1;
	float x4 = p->getXSize()+1;
	float y4 = p->getYSize()+1;
	
	// render border
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
	p->gl->End();

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += 2;
	y1 += 2;
	x2 -= 2;
	y2 -= 2;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renDoublePlainRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	glColor4fv(p->getFGColor());

	// render first clipping rectangle
	float x1 = -7;
	float y1 = -7;
	float x2 = p->getXSize()+7;
	float y2 = p->getYSize()+7;

	if(p->getParent())
	{
		x2 = max(-2, min(x2, p->getParent()->getXSize() - p->getXPos() + 2));
		y2 = max(-2, min(y2, p->getParent()->getYSize() - p->getYPos() + 2));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = -1;
	float y3 = -1;
	float x4 = p->getXSize()+1;
	float y4 = p->getYSize()+1;
	
	// render border
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
	p->gl->End();

	x3 -= 5;
	y3 -= 5;
	x4 += 5;
	y4 += 5;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
	p->gl->End();

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += 7;
	y1 += 7;
	x2 -= 7;
	y2 -= 7;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelInPlainRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -BEVEL_WIDTH;
	float y1 = -BEVEL_WIDTH;
	float x2 = p->getXSize()+BEVEL_WIDTH;
	float y2 = p->getYSize()+BEVEL_WIDTH;

	if(p->getParent())
	{
		x2 = max(-BEVEL_WIDTH, min(x2, p->getParent()->getXSize() - p->getXPos() + BEVEL_WIDTH));
		y2 = max(-BEVEL_WIDTH, min(y2, p->getParent()->getYSize() - p->getYPos() + BEVEL_WIDTH));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;
	
	glColor4fv(darkCol);

	// render border
	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(lightCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += BEVEL_WIDTH;
	y1 += BEVEL_WIDTH;
	x2 -= BEVEL_WIDTH;
	y2 -= BEVEL_WIDTH;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelOutPlainRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -BEVEL_WIDTH;
	float y1 = -BEVEL_WIDTH;
	float x2 = p->getXSize()+BEVEL_WIDTH;
	float y2 = p->getYSize()+BEVEL_WIDTH;

	if(p->getParent())
	{
		x2 = max(-BEVEL_WIDTH, min(x2, p->getParent()->getXSize() - p->getXPos() + BEVEL_WIDTH));
		y2 = max(-BEVEL_WIDTH, min(y2, p->getParent()->getYSize() - p->getYPos() + BEVEL_WIDTH));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;
	
	glColor4fv(lightCol);

	// render border
	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(darkCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += BEVEL_WIDTH;
	y1 += BEVEL_WIDTH;
	x2 -= BEVEL_WIDTH;
	y2 -= BEVEL_WIDTH;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelBothInPlainRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -(BEVEL_WIDTH*3+1);
	float y1 = -(BEVEL_WIDTH*3+1);
	float x2 = p->getXSize()+(BEVEL_WIDTH*3+1);
	float y2 = p->getYSize()+(BEVEL_WIDTH*3+1);

	if(p->getParent())
	{
		x2 = max(-(BEVEL_WIDTH*3+1), min(x2, p->getParent()->getXSize() - p->getXPos() + (BEVEL_WIDTH*3+1)));
		y2 = max(-(BEVEL_WIDTH*3+1), min(y2, p->getParent()->getYSize() - p->getYPos() + (BEVEL_WIDTH*3+1)));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;

	// render border
	glColor4fv(lightCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(darkCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(col);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(darkCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(lightCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += (BEVEL_WIDTH*3+1);
	y1 += (BEVEL_WIDTH*3+1);
	x2 -= (BEVEL_WIDTH*3+1);
	y2 -= (BEVEL_WIDTH*3+1);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelBothOutPlainRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -(BEVEL_WIDTH*3+1);
	float y1 = -(BEVEL_WIDTH*3+1);
	float x2 = p->getXSize()+(BEVEL_WIDTH*3+1);
	float y2 = p->getYSize()+(BEVEL_WIDTH*3+1);

	if(p->getParent())
	{
		x2 = max(-(BEVEL_WIDTH*3+1), min(x2, p->getParent()->getXSize() - p->getXPos() + (BEVEL_WIDTH*3+1)));
		y2 = max(-(BEVEL_WIDTH*3+1), min(y2, p->getParent()->getYSize() - p->getYPos() + (BEVEL_WIDTH*3+1)));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;
	
	// render border
	glColor4fv(darkCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(lightCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(col);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(lightCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(darkCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += (BEVEL_WIDTH*3+1);
	y1 += (BEVEL_WIDTH*3+1);
	x2 -= (BEVEL_WIDTH*3+1);
	y2 -= (BEVEL_WIDTH*3+1);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renNoneDitherRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render clipping rectangle
	float x1 = 0;
	float y1 = 0;
	float x2 = p->getXSize();
	float y2 = p->getYSize();

	if(p->getParent())
	{
		x2 = max(0, min(x2, p->getParent()->getXSize() - p->getXPos()));
		y2 = max(0, min(y2, p->getParent()->getYSize() - p->getYPos()));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	// render background
	p->gl->Enable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stip50);

	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	glColor4fv(p->getFGColor());

	glDisable(GL_POLYGON_STIPPLE);

	// render contents
	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renSingleDitherRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	glColor4fv(p->getFGColor());

	// render first clipping rectangle
	float x1 = -2;
	float y1 = -2;
	float x2 = p->getXSize()+2;
	float y2 = p->getYSize()+2;

	if(p->getParent())
	{
		x2 = max(-2, min(x2, p->getParent()->getXSize() - p->getXPos() + 2));
		y2 = max(-2, min(y2, p->getParent()->getYSize() - p->getYPos() + 2));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = -1;
	float y3 = -1;
	float x4 = p->getXSize()+1;
	float y4 = p->getYSize()+1;
	
	// render border
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
	p->gl->End();

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += 2;
	y1 += 2;
	x2 -= 2;
	y2 -= 2;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	p->gl->Enable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stip50);

	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	glDisable(GL_POLYGON_STIPPLE);

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renDoubleDitherRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	// render first clipping rectangle
	float x1 = -7;
	float y1 = -7;
	float x2 = p->getXSize()+7;
	float y2 = p->getYSize()+7;

	if(p->getParent())
	{
		x2 = max(-2, min(x2, p->getParent()->getXSize() - p->getXPos() + 2));
		y2 = max(-2, min(y2, p->getParent()->getYSize() - p->getYPos() + 2));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = -1;
	float y3 = -1;
	float x4 = p->getXSize()+1;
	float y4 = p->getYSize()+1;
	
	// render border
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
	p->gl->End();

	x3 -= 5;
	y3 -= 5;
	x4 += 5;
	y4 += 5;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
	p->gl->End();

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += 7;
	y1 += 7;
	x2 -= 7;
	y2 -= 7;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	p->gl->Enable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stip50);

	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	glDisable(GL_POLYGON_STIPPLE);

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelInDitherRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	glColor4fv(p->getFGColor());
	
	// render first clipping rectangle
	float x1 = -BEVEL_WIDTH;
	float y1 = -BEVEL_WIDTH;
	float x2 = p->getXSize()+BEVEL_WIDTH;
	float y2 = p->getYSize()+BEVEL_WIDTH;

	if(p->getParent())
	{
		x2 = max(-BEVEL_WIDTH, min(x2, p->getParent()->getXSize() - p->getXPos() + BEVEL_WIDTH));
		y2 = max(-BEVEL_WIDTH, min(y2, p->getParent()->getYSize() - p->getYPos() + BEVEL_WIDTH));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;
	
	glColor4fv(darkCol);

	// render border
	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(lightCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += BEVEL_WIDTH;
	y1 += BEVEL_WIDTH;
	x2 -= BEVEL_WIDTH;
	y2 -= BEVEL_WIDTH;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	p->gl->Enable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stip50);

	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	glDisable(GL_POLYGON_STIPPLE);

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelOutDitherRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -BEVEL_WIDTH;
	float y1 = -BEVEL_WIDTH;
	float x2 = p->getXSize()+BEVEL_WIDTH;
	float y2 = p->getYSize()+BEVEL_WIDTH;

	if(p->getParent())
	{
		x2 = max(-BEVEL_WIDTH, min(x2, p->getParent()->getXSize() - p->getXPos() + BEVEL_WIDTH));
		y2 = max(-BEVEL_WIDTH, min(y2, p->getParent()->getYSize() - p->getYPos() + BEVEL_WIDTH));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;
	
	glColor4fv(lightCol);

	// render border
	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(darkCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += BEVEL_WIDTH;
	y1 += BEVEL_WIDTH;
	x2 -= BEVEL_WIDTH;
	y2 -= BEVEL_WIDTH;

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	p->gl->Enable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stip50);

	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	glDisable(GL_POLYGON_STIPPLE);

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelBothInDitherRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -(BEVEL_WIDTH*3+1);
	float y1 = -(BEVEL_WIDTH*3+1);
	float x2 = p->getXSize()+(BEVEL_WIDTH*3+1);
	float y2 = p->getYSize()+(BEVEL_WIDTH*3+1);

	if(p->getParent())
	{
		x2 = max(-(BEVEL_WIDTH*3+1), min(x2, p->getParent()->getXSize() - p->getXPos() + (BEVEL_WIDTH*3+1)));
		y2 = max(-(BEVEL_WIDTH*3+1), min(y2, p->getParent()->getYSize() - p->getYPos() + (BEVEL_WIDTH*3+1)));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;

	// render border
	glColor4fv(lightCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(darkCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(col);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(darkCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(lightCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += (BEVEL_WIDTH*3+1);
	y1 += (BEVEL_WIDTH*3+1);
	x2 -= (BEVEL_WIDTH*3+1);
	y2 -= (BEVEL_WIDTH*3+1);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	p->gl->Enable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stip50);

	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	glDisable(GL_POLYGON_STIPPLE);

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}

void menuItem::renBevelBothOutDitherRect(menuItem* p)
{
	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// render first clipping rectangle
	float x1 = -(BEVEL_WIDTH*3+1);
	float y1 = -(BEVEL_WIDTH*3+1);
	float x2 = p->getXSize()+(BEVEL_WIDTH*3+1);
	float y2 = p->getYSize()+(BEVEL_WIDTH*3+1);

	if(p->getParent())
	{
		x2 = max(-(BEVEL_WIDTH*3+1), min(x2, p->getParent()->getXSize() - p->getXPos() + (BEVEL_WIDTH*3+1)));
		y2 = max(-(BEVEL_WIDTH*3+1), min(y2, p->getParent()->getYSize() - p->getYPos() + (BEVEL_WIDTH*3+1)));
	}
	
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LESS);

	float x3 = 0;
	float y3 = 0;
	float x4 = p->getXSize();
	float y4 = p->getYSize();

	// TODO:: optimise this away
	float* col = p->getBGColor();
	float lightCol[4];
	float darkCol[4];

	memcpy(lightCol,col,sizeof(float)*4);
	lightCol[0]++;
	lightCol[1]++;
	lightCol[2]++;
	lightCol[3]++;
	lightCol[0] *= 0.5f;
	lightCol[1] *= 0.5f;
	lightCol[2] *= 0.5f;
	lightCol[3] *= 0.5f;

	memcpy(darkCol,col,sizeof(float)*4);
	darkCol[0] *= 0.5f;
	darkCol[1] *= 0.5f;
	darkCol[2] *= 0.5f;
	darkCol[3] *= 0.5f;
	
	// render border
	glColor4fv(darkCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(lightCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(col);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	x3 -= BEVEL_WIDTH;
	x4 += BEVEL_WIDTH;
	y3 -= BEVEL_WIDTH;
	y4 += BEVEL_WIDTH;

	glColor4fv(lightCol);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3, y3, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();
	
	glColor4fv(darkCol);
	
	p->gl->Begin(GL_QUADS);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x3, y4, MENUITEM_Z);
		glVertex3f(x3-BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);

		glVertex3f(x4, y3, MENUITEM_Z);
		glVertex3f(x4, y4, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y4+BEVEL_WIDTH, MENUITEM_Z);
		glVertex3f(x4+BEVEL_WIDTH, y3-BEVEL_WIDTH, MENUITEM_Z);
	p->gl->End();

	// render smaller clipping rectangle
	p->gl->DepthFunc(GL_ALWAYS);
	p->gl->Enable(GL_BLEND);	
	p->gl->BlendFunc(GL_ZERO, GL_ONE);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y2, 0.0f);
	p->gl->End();

	x1 += (BEVEL_WIDTH*3+1);
	y1 += (BEVEL_WIDTH*3+1);
	x2 -= (BEVEL_WIDTH*3+1);
	y2 -= (BEVEL_WIDTH*3+1);

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, CLIP_Z);
		glVertex3f(x2, y1, CLIP_Z);
		glVertex3f(x2, y2, CLIP_Z);
		glVertex3f(x1, y2, CLIP_Z);
	p->gl->End();

	glDisable(GL_BLEND);

	p->gl->DepthFunc(GL_LEQUAL);

	// render background
	p->gl->Enable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stip50);

	glColor4fv(p->getBGColor());

	p->gl->Begin(GL_QUADS);
		glVertex3f(x1, y1, MENUITEM_Z);
		glVertex3f(x2, y1, MENUITEM_Z);
		glVertex3f(x2, y2, MENUITEM_Z);
		glVertex3f(x1, y2, MENUITEM_Z);
	p->gl->End();

	glDisable(GL_POLYGON_STIPPLE);

	// render contents
	glColor4fv(p->getFGColor());

	if(p->getTextRender())
	{
		p->getTextRender()->setColor(p->getFGColor()[0],p->getFGColor()[1],p->getFGColor()[2],p->getFGColor()[3]);
		p->getTextRender()->setPosition(p->getAbsoluteXPos()*MENU_SCALE, p->getAbsoluteYPos()*MENU_SCALE);
		p->getTextRender()->printf(p->getText());
	}
}