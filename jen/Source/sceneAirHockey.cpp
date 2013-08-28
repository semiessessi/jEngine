#include "sceneAirHockey.h"

#include "gameSystem.h"
#include "glTextRender.h"
#include "jEngine.h"
#include "inMouseReciever.h"
#include "inSystem.h"

#include "jMath.h"
#include "pngtex.h"

unsigned int ahPlayerScore = 0;
unsigned int ahEnemyScore = 0;
static bool reset = false;
static float startDir = 0.3f;

const float lightPos[] = { 0.5f, 1.0f, 0.1f };
// for lighting without shaders. normalise the position and use it as a direction
const float lightDir[] = { 0.5f/1.122497216f, 1.0f/1.122497216f, 0.1f/1.122497216f };

sceneAirHockey::sceneAirHockey() : gameScene()
{
    name = new wchar_t[wcslen(L"airhockey") + 1];
    name = wcscpy(name, L"airhockey");

    table = Table();
    camera = gameCamera();
    camera.setDistance(3.4f);
    camera.setPitch(40.0f);
    camera.setYaw(0.0f);
    mVector3d pos = mVector3d(0.0f);
    pos[1] = -0.5f;
    camera.setViewPosition(pos);
}

void sceneAirHockey::onInit()
{
    table.initGraphics();
}

void sceneAirHockey::onShutdown()
{
    table.shutdownGraphics();
}

void sceneAirHockey::onRender()
{
    // clear screen that works well with blending
    gl->Color(0.0f, 0.0f, 0.0f, blendFactor);
    gl->Begin(GL_QUADS);
        gl->Vertex(1,1);
        gl->Vertex(1,-1);
        gl->Vertex(-1,-1);
        gl->Vertex(-1,1);
    gl->End();

    // timer
    static float t = 0.0f;
    if((g->isBloomRender() && g->isBloomed()) || !(g->isBloomed()))
    {
        t += gls->getLastDT();
        table.cycle();
    }

    gl->Clear(GL_DEPTH_BUFFER_BIT);

    camera.glTransforms();

    gl->Enable(GL_DEPTH_TEST);
	gl->DepthFunc(GL_LESS);

    // draw table, etc...
    table.render();

    // draw scores
    glTextRender* tr = gls->getDefaultTextRenderer();
    tr->setColor(0.2f,0.2f,1.0f,1.0f);
    tr->setPosition(0.16f*gls->getAspectRatio(), 0.05f);
    tr->printf(L"Player: %d", ahPlayerScore);

    tr->setColor(1.0f,0.2f,0.2f,1.0f);
    tr->setPosition(0.72f*gls->getAspectRatio(), 0.05f);
    tr->printf(L"Enemy: %d", ahEnemyScore);
}

void sceneAirHockey::onSwitch()
{
    jEngine::getPointer()->in->setMouseReciever(table.GetPlayer());
}

void sceneAirHockey::Table::cycle()
{
    if(reset)
    {
        startDir = -startDir;
        puck.SetVelocity(0.2f,startDir);
        puck.SetPosition(0.0f, 0.0f);
        enemy.SetPosition(0.0f,-1.3f);
        player.SetPosition(0.0f,1.3f);
        reset = false;
    }
    puck.cycle();
    player.cycle();
    enemy.tellPuckPosition(puck.GetX(), puck.GetY());
    enemy.tellPuckSpeed(puck.GetSpeed());
    enemy.cycle();

    Bat* hitter = 0;
    // check for collisions between players and puck
    if(player.GetCircle().collidesWith(puck.GetCircle()))
    {
        // player hits puck
        hitter = &player;
    }
    else if(enemy.GetCircle().collidesWith(puck.GetCircle()))
    {
        // player hits puck
        hitter = &enemy;
    }

    if(hitter)
    {
        float dX = puck.GetX() - hitter->GetX();
        float dY = puck.GetY() - hitter->GetY();
        float d = sqrt(dX*dX + dY*dY);
        dX /= d;
        dY /= d;

        // adjust circle position - the *1.05f adds a little extra to make it a bit more stable
        float pX = hitter->GetX() + (BAT_RADIUS + PUCK_RADIUS)*dX*1.05f;
        float pY = hitter->GetY() + (BAT_RADIUS + PUCK_RADIUS)*dY*1.05f;
        puck.SetPosition(pX, pY);

        // reflect existing velocity
        float vX = puck.GetVelocityX();
        float vY = puck.GetVelocityY();
        float dot = vX * dX + vY * dY;
        vX = vX - 2*dX*dot;
        vY = vY - 2*dY*dot;
        puck.SetVelocity(vX, vY);

        // add new velocity
        puck.AddVelocity(hitter->GetSpeed()*dX, hitter->GetSpeed()*dY);
    }
}

void sceneAirHockey::Bat::cycle()
{
    float dt = gls->getLastDT();
    velocity[0] = circle.GetX() - oldPos[0]; oldPos[0] = circle.GetX();
    velocity[1] = circle.GetY() - oldPos[1]; oldPos[1] = circle.GetY();
    velocity[0] /= dt;
    velocity[1] /= dt;
}

void sceneAirHockey::PlayerBat::cycle()
{
    Bat::cycle();
    float dt = gls->getLastDT();
}

void sceneAirHockey::EnemyBat::cycle()
{
    Bat::cycle();
    float dt = gls->getLastDT();
    static float timer = 0.0f;
    float fac = pow(0.92f, 30.0f*dt);
    float targy = 0.0f;
    static float stargy = 0.0f;
    switch(state)
    {
    case AI_BLOCKING:
        // track the puck with slight delay
        targy = min(0.0f,max(puckY - 0.5f, -1.5f));
        // hit puck if it slows down or is in an opportune spot
        if(((puckX > -0.3f) && (puckX < 0.3f) && (puckY < -0.5f) && (puckY > -1.0f)) || (puckS < 0.25f)) state = AI_SHOOTING;
        // wait if the puck is outside of the "danger area"
        if(((puckX < -0.6f) || (puckX > 0.6f) || (puckY > 1.2f)) && (puckS > 0.45f)) state = AI_WAITING;
        circle.SetY(circle.GetY()*fac + targy*(1-fac));
        circle.SetX(circle.GetX()*fac + puckX*(1-fac));
        break;
    case AI_WAITING:
        // move towards the approximate middle in y, but do nothing else
        // start blocking if the puck enters the "danger area"
        if(((puckX > -0.4f) && (puckX < 0.4f) && (puckY < 0.8f)) || (puckS < 0.42f)) state = AI_BLOCKING;
        circle.SetY(circle.GetY()*fac + -1.2f*(1-fac));
        break;
    case AI_SHOOTING:
        // decide direction to shoot if needs be
        if(circle.GetY() > puckY)
        {
            if(stargy == 0.0f) stargy = min(0.0f,max(puckY - 0.5f, -2.0f));
        }
        else if(stargy == 0.0f) stargy = min(0.0f,max(puckY + 0.5f, -2.0f));
        timer += dt;
        fac = pow(0.92f, 30.0f*dt);
        circle.SetY(circle.GetY()*fac + stargy*(1-fac));
        circle.SetX(circle.GetX()*fac + puckX*(1-fac));
        // if we are about to hit, if the puck is on the other side or if too much time has passed, revert to waiting
        if((timer > 1.5f) || (puckY > 0.0f) || (abs(puckY - circle.GetY()) < PUCK_RADIUS + BAT_RADIUS))
        {
            timer = 0.0f;
            stargy = 0.0f;
            state = AI_WAITING;
        }
        
        break;
    };
}

void sceneAirHockey::Puck::cycle()
{
    float dt = gls->getLastDT();

    circle.AddPosition(dt*velocity[0], dt*velocity[1]);

    // collide with table
    if(circle.GetX() > (1.0f - PUCK_RADIUS)) velocity[0] = -velocity[0];
    if(circle.GetX() < (-1.0f + PUCK_RADIUS)) velocity[0] = -velocity[0];
    if(circle.GetY() > (2.0f - PUCK_RADIUS)) velocity[1] = -velocity[1];
    if(circle.GetY() < (-2.0f + PUCK_RADIUS)) velocity[1] = -velocity[1];

    // collide with goal
    if((circle.GetX() < 0.5f) && (circle.GetX() > -0.5f))
    {
        // most of top and bottom edges
        if((circle.GetY() > (1.5f - PUCK_RADIUS)) && (circle.GetY() < (1.6f + PUCK_RADIUS)))
        {
            if(velocity[1] > 0.0f)
            {
                ++ahEnemyScore; // score point for enemy
                reset = true;
            }
            velocity[1] = -velocity[1];
        }

        if((circle.GetY() < (-1.5f + PUCK_RADIUS)) && (circle.GetY() > (-1.6f - PUCK_RADIUS)))
        {
            if(velocity[1] < 0.0f)
            {
                ++ahPlayerScore; // score point for player
                reset = true;
            }
            velocity[1] = -velocity[1];
        }
    }
    else if((abs(circle.GetY()) > 1.5f) && (abs(circle.GetY()) < 1.6f))
    {
        // most of left and right edges
        if(abs(circle.GetX()) < (0.5f + PUCK_RADIUS)) velocity[0] = -velocity[0];
    }
    else
    {
        // edge cases (collide with corners)
        if(GetDistance(-0.5f, 1.5f) < PUCK_RADIUS)
        {
            float dX = -0.5f - circle.GetX();
            float dY = 1.5f - circle.GetY();
            float d = 1 / sqrt(dX*dX + dY*dY);
            dX *= d;
            dY *= d;
            velocity[0] = -dX * GetSpeed();
            velocity[1] = -dY * GetSpeed();
            
        }

        if(GetDistance(-0.5f, 1.6f) < PUCK_RADIUS)
        {
            float dX = -0.5f - circle.GetX();
            float dY = 1.6f - circle.GetY();
            float d = 1 / sqrt(dX*dX + dY*dY);
            dX *= d;
            dY *= d;
            velocity[0] = -dX * GetSpeed();
            velocity[1] = -dY * GetSpeed();
            
        }

        if(GetDistance(0.5f, 1.5f) < PUCK_RADIUS)
        {
            float dX = 0.5f - circle.GetX();
            float dY = 1.5f - circle.GetY();
            float d = 1 / sqrt(dX*dX + dY*dY);
            dX *= d;
            dY *= d;
            velocity[0] = -dX * GetSpeed();
            velocity[1] = -dY * GetSpeed();
            
        }

        if(GetDistance(0.5f, 1.6f) < PUCK_RADIUS)
        {
            float dX = 0.5f - circle.GetX();
            float dY = 1.6f - circle.GetY();
            float d = 1 / sqrt(dX*dX + dY*dY);
            dX *= d;
            dY *= d;
            velocity[0] = -dX * GetSpeed();
            velocity[1] = -dY * GetSpeed();
            
        }
    }


    ValidatePosition();

    float fac = pow(0.98f, 10.0f*dt);

    velocity[0] *= fac;
    velocity[1] *= fac;
}

void sceneAirHockey::Table::initGraphics()
{
    glRenderable::initGraphics();
    puck.initGraphics();
    player.initGraphics();
    enemy.initGraphics();

    // load textures
    png_tex t;

    t.LoadFromFile("graphics\\textures\\table_d.png");

    gl->Enable(GL_TEXTURE_2D);

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.GetWidth(), t.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.imagedata);

    gl->Disable(GL_TEXTURE_2D);

    // setup for game
    puck.SetVelocity(0.2f,startDir);
    enemy.SetPosition(0.0f,-1.3f);
    player.SetPosition(0.0f,1.3f);
}

void sceneAirHockey::Table::shutdownGraphics()
{
    // unload textures
    glDeleteTextures(1, &texture1);

    puck.shutdownGraphics();
    player.shutdownGraphics();
    enemy.shutdownGraphics();
    glRenderable::shutdownGraphics();
}

void sceneAirHockey::Table::render()
{
    gl->Enable(GL_TEXTURE_2D);

    gl->Color(1.0f,1.0f,1.0f);
    // render table
    glBindTexture(GL_TEXTURE_2D, texture1);
    // the following surface all has normal 0, 1, 0
    // so the dot for lighting resolves to just the y component of the light direction
    gl->Color(lightDir[1], lightDir[1], lightDir[1]);

    // render main surface
    gl->Begin(GL_QUADS);

    gl->Normal(0.0f,1.0f,0.0f);

    gl->TexCoord(0.0f,0.0f);
    gl->Vertex(-1.0f,0.0f,2.0f);
    gl->TexCoord(1.0f,0.0f);
    gl->Vertex(1.0f,0.0f,2.0f);
    gl->TexCoord(1.0f,1.0f);
    gl->Vertex(1.0f,0.0f,0.0f);
    gl->TexCoord(0.0f,1.0f);
    gl->Vertex(-1.0f,0.0f,0.0f);
    
    gl->TexCoord(0.0f,0.0f);
    gl->Vertex(-1.0f,0.0f,-2.0f);
    gl->TexCoord(1.0f,0.0f);
    gl->Vertex(1.0f,0.0f,-2.0f);
    gl->TexCoord(1.0f,1.0f);
    gl->Vertex(1.0f,0.0f,0.0f);
    gl->TexCoord(0.0f,1.0f);
    gl->Vertex(-1.0f,0.0f,0.0f);

    gl->End();

    // render goals (two open bottom cuboids)
    glBindTexture(GL_TEXTURE_2D, 0);

    //
    float baseCol[] = { 0.15f, 0.15f, 0.9f };
    // so the dot for lighting resolves to just the y component of the light direction
    gl->Color(baseCol[0]*lightDir[1], baseCol[1]*lightDir[1], baseCol[2]*lightDir[1]);
    gl->Begin(GL_QUADS);

    // upwards surfaces
    gl->Normal(0.0f,1.0f,0.0f);

    gl->Vertex(-0.5f,THICK_DEPTH,1.6f);
    gl->Vertex(0.5f,THICK_DEPTH,1.6f);
    gl->Vertex(0.5f,THICK_DEPTH,1.5f);
    gl->Vertex(-0.5f,THICK_DEPTH,1.5f);

    gl->Vertex(-0.5f,THICK_DEPTH,-1.6f);
    gl->Vertex(0.5f,THICK_DEPTH,-1.6f);
    gl->Vertex(0.5f,THICK_DEPTH,-1.5f);
    gl->Vertex(-0.5f,THICK_DEPTH,-1.5f);

    // front surfaces
    gl->Normal(0.0f,0.0f,1.0f);
    // the following surface all has normal 0, 0, 1
    // so the dot for lighting resolves to just the z component of the light direction
    gl->Color(baseCol[0]*lightDir[2], baseCol[1]*lightDir[2], baseCol[2]*lightDir[2]);
    gl->Vertex(-0.5f,THICK_DEPTH,1.6f);
    gl->Vertex(0.5f,THICK_DEPTH,1.6f);
    gl->Vertex(0.5f,0.0f,1.6f);
    gl->Vertex(-0.5f,0.0f,1.6f);

    gl->Vertex(-0.5f,THICK_DEPTH,-1.5f);
    gl->Vertex(0.5f,THICK_DEPTH,-1.5f);
    gl->Vertex(0.5f,0.0f,-1.5f);
    gl->Vertex(-0.5f,0.0f,-1.5f);

    // left surfaces
    gl->Normal(1.0f,0.0f,0.0f);
    // the following surface all has normal 1, 0, 0
    // so the dot for lighting resolves to just the x component of the light direction
    gl->Color(baseCol[0]*lightDir[0], baseCol[1]*lightDir[0], baseCol[2]*lightDir[0]);
    gl->Vertex(0.5f,THICK_DEPTH,1.6f);
    gl->Vertex(0.5f,THICK_DEPTH,1.5f);
    gl->Vertex(0.5f,0.0f,1.5f);
    gl->Vertex(0.5f,0.0f,1.6f);

    gl->Vertex(0.5f,THICK_DEPTH,-1.6f);
    gl->Vertex(0.5f,THICK_DEPTH,-1.5f);
    gl->Vertex(0.5f,0.0f,-1.5f);
    gl->Vertex(0.5f,0.0f,-1.6f);

    // back surfaces
    gl->Normal(0.0f,0.0f,-1.0f);
    // the following surface all has normal 0, 0, -1
    // so the dot for lighting resolves to just -ve the z component of the light direction
    gl->Color(-baseCol[0]*lightDir[2], -baseCol[1]*lightDir[2], -baseCol[2]*lightDir[2]);
    gl->Vertex(-0.5f,THICK_DEPTH,1.5f);
    gl->Vertex(0.5f,THICK_DEPTH,1.5f);
    gl->Vertex(0.5f,0.0f,1.5f);
    gl->Vertex(-0.5f,0.0f,1.5f);

    gl->Vertex(-0.5f,THICK_DEPTH,-1.6f);
    gl->Vertex(0.5f,THICK_DEPTH,-1.6f);
    gl->Vertex(0.5f,0.0f,-1.6f);
    gl->Vertex(-0.5f,0.0f,-1.6f);

    // right surfaces
    gl->Normal(-1.0f,0.0f,0.0f);
    // the following surface all has normal -1, 0, 0
    // so the dot for lighting resolves to just -ve the x component of the light direction
    gl->Color(-baseCol[0]*lightDir[0], -baseCol[1]*lightDir[0], -baseCol[2]*lightDir[0]);
    gl->Vertex(-0.5f,THICK_DEPTH,1.6f);
    gl->Vertex(-0.5f,THICK_DEPTH,1.5f);
    gl->Vertex(-0.5f,0.0f,1.5f);
    gl->Vertex(-0.5f,0.0f,1.6f);

    gl->Vertex(-0.5f,THICK_DEPTH,-1.6f);
    gl->Vertex(-0.5f,THICK_DEPTH,-1.5f);
    gl->Vertex(-0.5f,0.0f,-1.5f);
    gl->Vertex(-0.5f,0.0f,-1.6f);

    gl->End();

    gl->Disable(GL_TEXTURE_2D);

    puck.render();
    player.render();
    enemy.render();
}

void sceneAirHockey::Puck::render()
{
    gl->Color(0.3f,0.3f,0.8f);
    circle.glRenderCylinder(THIN_DEPTH);
}

void sceneAirHockey::Bat::render()
{
    gl->Color(0.9f,0.2f,0.2f);
    circle.glRenderCylinder(THIN_DEPTH);
}

bool sceneAirHockey::PlayerBat::onMouseMovement(float x, float y, float dx, float dy)
{
    circle.AddPosition(3.0f*dx, 3.0f*dy);
    if(circle.GetX() > (1.0f - BAT_RADIUS)) circle.SetX(1.0f - BAT_RADIUS);
    if(circle.GetX() < (-1.0f + BAT_RADIUS)) circle.SetX(-1.0f + BAT_RADIUS);
    if(circle.GetY() > (2.0f - BAT_RADIUS)) circle.SetY(2.0f - BAT_RADIUS);
    if(circle.GetY() < (0.0f + BAT_RADIUS)) circle.SetY(0.0f + BAT_RADIUS);
    return true;
}

bool sceneAirHockey::Circle::collidesWith(const sceneAirHockey::Circle& c) const
{
    float dx = position[0] - c.position[0];
    float dy = position[1] - c.position[1];
    float r = radius + c.radius;
    if((dx*dx + dy*dy) < r*r) return true;
    return false;
}

void sceneAirHockey::Circle::glRenderCylinder(const float &depth) const
{
    const float o = PI / 20.0f;
    float p = 0;

    float baseCol[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    // get colour for use in directional lighting
    glGetFloatv(GL_CURRENT_COLOR, baseCol);

    gl->Begin(GL_TRIANGLE_FAN);
    gl->Vertex(position[0], depth, position[1]);
    for(unsigned int i = 0; i < 41; ++i)
    {
        // normal is 0, 1, 0
        gl->Normal(0.0f, 1.0f, 0.0f);
        // dot is just y component of light vector
        gl->Color(baseCol[0] * lightDir[1], baseCol[1] * lightDir[1], baseCol[2] * lightDir[1]);
        gl->Vertex(position[0] + radius*cos(p), depth, position[1] + radius*sin(p));
        p += o;
    }
    gl->End();

    p = 0;
    gl->Begin(GL_TRIANGLE_STRIP);
    for(unsigned int i = 0; i < 41; ++i)
    {
        // normal is cos(p), 0, sin(p)
        gl->Normal(cos(p), 0.0f, sin(p));
        // do colour based on directional lighting
        // (no y component in dot since the normal is 0 in y)
        float dot = cos(p)*lightDir[0] + sin(p)*lightDir[2];
        gl->Color(baseCol[0] * dot, baseCol[1] * dot, baseCol[2] * dot);
        gl->Vertex(position[0] + radius*cos(p), depth, position[1] + radius*sin(p));
        gl->Vertex(position[0] + radius*cos(p), 0.0f, position[1] + radius*sin(p));
        p += o;
    }
    gl->End();
}