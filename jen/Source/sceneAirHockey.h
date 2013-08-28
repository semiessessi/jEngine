/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __SCENEAIRHOCKEY_H
#define __SCENEAIRHOCKEY_H

#define PUCK_RADIUS     0.14f
#define BAT_RADIUS      0.1f
#define THIN_DEPTH      0.02f
#define THICK_DEPTH     0.05f

#include "gameCamera.h"
#include "gameScene.h"
#include "glShader.h"
#include "inMouseReciever.h"

extern unsigned int ahPlayerScore;
extern unsigned int ahEnemyScore;

class sceneAirHockey : public gameScene
{
private:
    gameCamera  camera;

    class Circle : public glRenderable
    {
    private:
        float   radius;
        float   position[2];
    public:
        Circle(const float& r) : radius(r) { position[0] = position[1] = 0.0f; }
        ~Circle() {}

        float GetRadius() const { return radius; }
        float GetX() const { return position[0]; }
        float GetY() const { return position[1]; }

        void SetX(const float& x) { position[0] = x; }
        void SetY(const float& y) { position[1] = y; }

        void AddPosition(const float& x, const float& y) { position[0] += x; position[1] += y; }

        void glRenderCylinder(const float& depth) const;

        bool collidesWith(const Circle& c) const;
    };    

    class Puck : public glRenderable
    {
    private:
        Circle circle;
        float velocity[2];
    public:
        Puck() : circle(PUCK_RADIUS) { velocity[0] = velocity[1] = 0.0f; }

        void cycle();

        void initGraphics() { glRenderable::initGraphics(); circle.initGraphics(); }
        void shutdownGraphics() { circle.shutdownGraphics(); glRenderable::shutdownGraphics(); }
        void render();

        float GetX() const { return circle.GetX(); }
        float GetY() const { return circle.GetY(); }

        void AddVelocity(const float& x, const float& y) { velocity[0] += x; velocity[1] += y; }
        void SetVelocity(const float& x, const float& y) { velocity[0] = x; velocity[1] = y; }
        
        void SetPosition(const float& x, const float& y)
        {
            // force inside table
            circle.SetX(max(-1.0f + PUCK_RADIUS,min(1.0f - PUCK_RADIUS,x)));
            circle.SetY(max(-2.0f + PUCK_RADIUS,min(2.0f - PUCK_RADIUS,y)));

            /*
            // removed: makes a mess
            // force around left/right edges of goal
            if((abs(circle.GetY()) > 1.5f) && (abs(circle.GetY()) < 1.6f))
            {
                if(abs(circle.GetX()) < (0.5f + PUCK_RADIUS)) circle.SetX((0.5f + PUCK_RADIUS)*sgn(circle.GetX()));
            }
            */
        }

        void ValidatePosition()
        {
            SetPosition(circle.GetX(), circle.GetY());
        }

        float GetVelocityX() const { return velocity[0]; }
        float GetVelocityY() const { return velocity[1]; }

        float GetSpeed() const { return sqrt(velocity[0]*velocity[0] + velocity[1]*velocity[1]); }
        float GetDistance(const float& x, const float& y) const
        { 
            float dx = x - circle.GetX();
            float dy = y - circle.GetY();
            return sqrt(dx*dx + dy*dy);
        }

        Circle GetCircle() const { return circle; }
    };

    class Bat : public glRenderable
    {
    protected:
        Circle circle;
        float oldPos[2];
        float velocity[2];
    public:
        Bat() : circle(BAT_RADIUS) { oldPos[0] = oldPos[1] = velocity[0] = velocity[1]; }

        void initGraphics() { glRenderable::initGraphics(); circle.initGraphics(); }
        void shutdownGraphics() { circle.shutdownGraphics(); glRenderable::shutdownGraphics(); }
        void render();

        float GetX() const { return circle.GetX(); }
        float GetY() const { return circle.GetY(); }

        Circle GetCircle() const { return circle; }
        float GetSpeed() const { return sqrt(velocity[0]*velocity[0] + velocity[1]*velocity[1]); }

        void SetPosition(const float& x, const float& y) { circle.SetX(x); circle.SetY(y); }

        virtual void cycle();
    };

    class PlayerBat : public Bat, public inMouseReciever
    {
    public:
        PlayerBat() : Bat() {}
        void cycle();

        bool onMouseMovement(float x, float y, float dx, float dy);
    };

    class EnemyBat : public Bat
    {
    private:
        enum aiState
        {
            AI_WAITING,
            AI_BLOCKING,
            AI_SHOOTING
        } state;

        float puckX;
        float puckY;
        float puckS;
    public:
        EnemyBat() : Bat(), state(AI_WAITING), puckX(0.0f), puckY(0.0f), puckS(0.0f) {}
        void cycle();
        void tellPuckPosition(const float& x, const float& y) { puckX = x; puckY = y; }
        void tellPuckSpeed(const float& speed) { puckS = speed; }
    };

    class Table : public glRenderable
    {
    private:
        Puck puck;
        PlayerBat player;
        EnemyBat enemy;
        unsigned int texture1;
        unsigned int texture2;
        unsigned int cubeMap;
    public:
        Table() : puck(), player(), enemy(), texture1(0), texture2(0), cubeMap(0) {}

        void cycle();

        void initGraphics();
        void shutdownGraphics();

        PlayerBat* GetPlayer() { return &player; }

        void render();
    };

    Table table;
public:
    sceneAirHockey();

    void onInit();
    void onShutdown();
    void onRender();
    void onSwitch();
};

#endif