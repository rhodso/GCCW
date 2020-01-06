#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H

#include "gameobject.h"

class collisionObject : public gameObject
{
public:
    //Constructor
    collisionObject();

    //Destructor
    virtual ~collisionObject();

    //Getters and setters
    dGeomID getGeom();
    dBodyID getBody();
    dMass getMass();
    bool getDebugDraw();

    void setGeom(dGeomID _geom);
    void setBody(dBody _body);
    void setMass(dMass _mass);
    void setDebugDraw(bool _debugDraw);

protected:
    dGeomID geom;
    dBodyID body;
    dMass mass;


};

#endif // COLLISIONOBJECT_H
