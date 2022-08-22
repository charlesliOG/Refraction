#ifndef LASERWIDGET_H
#define LASERWIDGET_H

#include <QLabel>
#include <QSpinBox>
#include <QWidget>
#include <QPushButton>

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

// Represents the number of pixels per meter to draw box2d objects at.
const int B2D_DRAW_SCALE = 20;

const int MAX_REFLECTIONS = 64;

const float32 IOF_VACCUM = 1.0f;
const float32 IOF_AIR = 1.0003f;
const float32 IOF_WATER = 1.333f;
const float32 IOF_GLASS = 1.52f;
const float32 IOF_DIAMOND = 2.417f;

struct LightRay {
    /* Start position in world pos */
    b2Vec2 pos;
    /* Direction vector (NOT normalized) */
    b2Vec2 direction;
    /* Index of refraction of the medium this light ray is travelling through */
    float32 iof;

    /**
     * @brief paint Draws the light ray onto the screen
     * @param painter QPainter pointer
     * @param fraction Length of the light ray as a multiple of the length of the direction vector (given by b2d raycast)
     */
    void paint(QPainter* painter, float32 fraction);
};

class Object {
public:
    Object(b2World* world, b2PolygonShape* poly, float32 x, float32 y, float32 angle);
    b2Body* body;
    b2Fixture* fixture;

    /**
     * @brief paint Draws the object onto the screen at the position on the screen
     * @param painter
     */
    virtual void paint(QPainter* painter);

    /**
     * @brief getLightRayResponse Defines the behavior when this object is hit by a light ray.
     * ray.direction will be set to the direction of the new ray to be emitted.
     * @param ray Light ray that hit this object. ray.pos is set to the intersection point.
     * @param output Box2D Ray cast output
     * @return True if another light ray should be cast.
     */
    virtual bool getLightRayResponse(LightRay& ray, b2RayCastOutput& output);

protected:
    QColor fillColor;
    QColor strokeColor;
    int strokeWeight;
};

class Mirror : public Object {
public:
    Mirror(b2World* world, b2PolygonShape* poly, float32 x, float32 y, float32 angle);

    /* Mirror response: Reflect the ray using the law of reflection */
    bool getLightRayResponse(LightRay& ray, b2RayCastOutput& output);

    /**
     * @brief getReflectionVector Get a direction vector for a reflected ray.
     * Result is not normalized.
     * @param incident Vector for the incoming light ray
     * @param normal Vector normal to the reflection surface (MUST BE NORMALIZED)
     * @return Vector for the reflected light ray
     */
    b2Vec2 getReflectionVector(b2Vec2& incident, b2Vec2& normal);
};

class Medium : public Object {
public:
    // index of refraction of this medium
    float32 iof;

    Medium(b2World* world, b2PolygonShape* poly, float32 x, float32 y, float32 angle, float32 iof, QColor fill = QColorConstants::Transparent);

    /* Medium response: Refract the ray if less than the critical angle, otherwise reflect */
    bool getLightRayResponse(LightRay& ray, b2RayCastOutput& output);

    /**
     * @brief getRefractionVector Get a direction vector for a refracted ray.
     * Result is not normalized.
     * @param incident Vector for the incoming light ray
     * @param normal Vector normal to the refraction surface (MUST BE NORMALIZED)
     * @param n1 Index of refraction of the 1st medium. 1.0 minimum
     * @param n2 Index of refraction of the 2nd medium. 1.0 minimum
     * @param tir Will be set to true if the ray underwent total internal reflection instead of refraction
     * @return Vector for the refracted light ray
     */
    b2Vec2 getRefractionVector(b2Vec2& incident, b2Vec2& normal, float32 n1, float32 n2, bool& tir);
};


class LaserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LaserWidget(QWidget *parent = nullptr);
    QPushButton *infoButton;
    ~LaserWidget();

signals:

private:
    QLabel* iofLabel;
    QDoubleSpinBox* iofSpinBox;
    Medium* editableMedium; // pointer to the medium that the spinbox edits

    b2World world;
    QVector<Object*> objects;
    LightRay laser; // laser origin set by the mouse

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);

    /**
     * @brief setMediumIoF Sets the IoF of the medium saved in editableMedium
     * @param iof index of refraction
     */
    void setMediumIoF(float32 iof);

    /**
     * @brief castLightRay Cast and draw a ray of light. If the ray hits an object, recursively cast another ray in the appropriate direction.
     * @param painter QPainter pointer
     * @param ray LightRay to cast
     * @param prevObjIndex Index of the object the light ray last interacted with. Used to prevent rays from hitting the same object twice.
     * @param depth Current recursion depth
     */
    void castLightRay(QPainter* painter, LightRay ray, int prevObjIndex, int depth);

    /**
     * @brief getIoFAtPos Gets the index of refraction of the medium at a given position.
     * If multiple mediums overlap, gets the IoF of the first one in the objects list.
     * @param pos Point in world pos
     * @return Index of refraction
     */
    float32 getIoFAtPos(b2Vec2& pos);

    /**
     * @brief screenToWorldPos Convert a position on the screen in pixels to a position in the box2d world
     * in meters. (0, 0) is the center of the screen.
     * @param x Screen x pos
     * @param y Screen y pos
     * @return Vec2 world pos
     */
    b2Vec2 screenToWorldPos(float32 x, float32 y);
};

#endif // LASERWIDGET_H
