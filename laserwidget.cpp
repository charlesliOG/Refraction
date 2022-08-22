#include "laserwidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QMouseEvent>
#include <iostream>
#include <QStyleOption>

#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>


LaserWidget::LaserWidget(QWidget *parent)
    : QWidget{parent}
    , world(b2Vec2(0, 0))
    , objects()
{
    b2PolygonShape box;

    // Water medium
    box.SetAsBox(20, 5);
    editableMedium = new Medium(&world, &box, 0, 10, 0, IOF_WATER, QColor(0, 0, 255, 100));
    objects.append(editableMedium);

    // Vacuum medium
    // (box2d raycasts don't work when the light exits the water box so we need boxes of vacuum
    // surrounding the water so the light knows when to switch IoF)
    // (also, add an offset of 0.01 to the position to avoid issues with the collision overlapping)
    box.SetAsBox(20, 1);
    objects.append(new Medium(&world, &box, 0, 4 + 0.01, 0, IOF_VACCUM));
    objects.append(new Medium(&world, &box, 0, 16 - 0.01, 0, IOF_VACCUM));
    box.SetAsBox(1, 5);
    objects.append(new Medium(&world, &box, -21 + 0.01, 10, 0, IOF_VACCUM));
    objects.append(new Medium(&world, &box, 21 - 0.01, 10, 0, IOF_VACCUM));

    // Mirrors
    box.SetAsBox(2, 1);
    objects.append(new Mirror(&world, &box, 3, -5, 3));
    objects.append(new Mirror(&world, &box, -8, -9, -1));
    objects.append(new Mirror(&world, &box, 9, -12, 2));
    objects.append(new Mirror(&world, &box, -12, -1, 4.3));
    objects.append(new Mirror(&world, &box, 16, -2, 1.45));
    objects.append(new Mirror(&world, &box, 1, -11, 0.4));

    laser.pos = b2Vec2(0, 0);
    laser.direction = b2Vec2(0, 0);
    laser.iof = IOF_VACCUM;

    // set up QLabel, Spin Box, and QButton
    iofSpinBox = new QDoubleSpinBox(this);
    iofSpinBox->setMinimum(1.0);
    iofSpinBox->setSingleStep(0.1);
    iofSpinBox->setValue(IOF_WATER);
    iofSpinBox->move(parent->width() - this->width() - 20, 40);

    iofLabel = new QLabel(this);
    iofLabel->setText("Index of Refraction");
    iofLabel->move(parent->width() - this->width() - 20,20);

    infoButton = new QPushButton(this);
    infoButton->setText("Show info");
    infoButton->move(20, 20);
    infoButton->setStyleSheet("QPushButton{"
                              "background-color: #FFFFFF;"
                              "border-radius: 4px;"
                              "border: 1px solid black;"
                              "font-size: 14px;"
                              "color: #000000;"
                              "margin: 0;"
                              "font-weight: 500;"
                              "min-width: 10px;"
                              "outline: none;"
                              "padding: 4px 8px;}"
                              "QPushButton:hover{"
                              "}"
                              );

    infoButton->setCursor(QCursor(Qt::PointingHandCursor));

    // connect iof spinbox
    connect(iofSpinBox, &QDoubleSpinBox::valueChanged, this, &LaserWidget::setMediumIoF);

    // make widget fill parent container
    this->resize(parent->width(), parent->height());

    // set stylesheet
    this->setStyleSheet("background-color: white;");
}

void LaserWidget::mouseMoveEvent(QMouseEvent* event) {
    b2Vec2 mousePos = screenToWorldPos(event->position().x(), event->position().y());
    laser.direction = (mousePos - laser.pos);
    laser.direction.Normalize();
    update();
}

void LaserWidget::mousePressEvent(QMouseEvent* event) {
    b2Vec2 mousePos = screenToWorldPos(event->position().x(), event->position().y());
    laser.pos = mousePos;
    laser.iof = getIoFAtPos(mousePos);
    update();
}

void LaserWidget::setMediumIoF(float32 iof) {
    editableMedium->iof = iof;
    laser.iof = getIoFAtPos(laser.pos);
    update();
}

void LaserWidget::paintEvent(QPaintEvent *event) {
    // set up to accept style sheet
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // translate the origin to the center of the screen
    painter.translate(width()/2, height()/2);

    for (int i = 0; i < objects.size(); ++i) {
        objects[i]->paint(&painter);
    }

    castLightRay(&painter, laser, -1, 0);
}

void LaserWidget::castLightRay(QPainter* painter, LightRay ray, int prevObjIndex, int depth) {
    b2RayCastInput input;
    b2RayCastOutput output;

    input.p1 = ray.pos;
    input.p2 = ray.pos + ray.direction;
    // make maxFraction big enough to cross the screen
    input.maxFraction = qMax(width(), height()) / B2D_DRAW_SCALE;

    // find the closest object that intersects with the raycast
    b2RayCastOutput minOutput;
    minOutput.fraction = input.maxFraction;
    int closestObjIndex = -1;
    for (int i = 0; i < objects.size(); ++i) {
        if (i != prevObjIndex && objects[i]->fixture->RayCast(&output, input, 0)) {
            if (output.fraction < minOutput.fraction) {
                minOutput = output;
                closestObjIndex = i;
            }
        }
    }

    // draw the ray
    ray.paint(painter, minOutput.fraction);

    if (depth < MAX_REFLECTIONS && closestObjIndex >= 0) {
        // move new ray position to the end of the current ray
        ray.pos = ray.pos + minOutput.fraction * ray.direction;

        // cast the new ray
        if (objects[closestObjIndex]->getLightRayResponse(ray, minOutput)) {
            castLightRay(painter, ray, closestObjIndex, depth + 1);
        }
    }
}

float32 LaserWidget::getIoFAtPos(b2Vec2& pos) {
    for (int i = 0; i < objects.size(); ++i) {
        if (Medium* medium = dynamic_cast<Medium*>(objects[i])) {
            if (medium->fixture->TestPoint(pos))
                return medium->iof;
        }
    }
    return 1.0f;
}

b2Vec2 LaserWidget::screenToWorldPos(float32 x, float32 y) {
    return b2Vec2((x - width()/2) / B2D_DRAW_SCALE,
                  (y - height()/2) / B2D_DRAW_SCALE);
}

LaserWidget::~LaserWidget() {
    qDeleteAll(objects);
}


void LightRay::paint(QPainter* painter, float32 fraction) {
    int scale = B2D_DRAW_SCALE;

    QPen pen;
    pen.setColor(QColorConstants::Red);
    pen.setWidth(2);
    painter->setPen(pen);

    b2Vec2 end = pos + (fraction * direction);
    painter->drawLine(pos.x * scale, pos.y * scale, end.x * scale, end.y * scale);
}

Object::Object(b2World* world, b2PolygonShape* poly, float32 x, float32 y, float32 angle)
    : fillColor(QColorConstants::Transparent)
    , strokeColor(QColorConstants::Black)
    , strokeWeight(1)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(x, y);
    bodyDef.angle = angle;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = poly;

    body = world->CreateBody(&bodyDef);
    fixture = body->CreateFixture(&fixtureDef);
}

void Object::paint(QPainter* painter) {
    bool drawStroke = strokeColor != QColorConstants::Transparent && strokeWeight > 0;
    bool drawFill = fillColor != QColorConstants::Transparent;
    if (!drawStroke && !drawFill) return;

    painter->save(); // push transformation matrix

    // pixels per meter
    int scale = B2D_DRAW_SCALE;

    // translate & rotate the matrix according to the body transform
    b2Vec2 pos = body->GetPosition();
    painter->translate(pos.x * scale, pos.y * scale);
    painter->rotate(qRadiansToDegrees(body->GetAngle()));

    // copy the b2Polygon into a QPolygon
    b2PolygonShape* b2poly = (b2PolygonShape*) fixture->GetShape();
    int vertexCount = b2poly->GetVertexCount();
    QPolygon qpoly;
    b2Vec2 vertex;
    for (int i = 0; i < vertexCount; ++i) {
        vertex = b2poly->GetVertex(i);
        qpoly << QPoint(vertex.x * scale, vertex.y * scale);
    }

    if (drawFill) {
        // Make fill brush
        QBrush fillBrush;
        fillBrush.setColor(fillColor);
        fillBrush.setStyle(Qt::SolidPattern);

        // Draw polygon fill
        QPainterPath fillPath;
        fillPath.addPolygon(qpoly);
        painter->fillPath(fillPath, fillBrush);
    }

    if (drawStroke) {
        // Make stroke pen
        QPen strokePen;
        strokePen.setWidth(strokeWeight);
        strokePen.setColor(strokeColor);
        painter->setPen(strokePen);

        // Draw the polygon stroke
        painter->drawConvexPolygon(qpoly);
    }

    painter->restore(); // pop transformation matrix
}

bool Object::getLightRayResponse(LightRay& ray, b2RayCastOutput& output) {
    return false;
}

Mirror::Mirror(b2World* world, b2PolygonShape* poly, float32 x, float32 y, float32 angle)
    : Object(world, poly, x, y, angle)
{ }

bool Mirror::getLightRayResponse(LightRay& ray, b2RayCastOutput& output) {
    ray.direction = getReflectionVector(ray.direction, output.normal);
    return true;
}

b2Vec2 Mirror::getReflectionVector(b2Vec2& incident, b2Vec2& normal) {
    return incident - (2 * b2Dot(incident, normal)) * normal;
}

Medium::Medium(b2World* world, b2PolygonShape* poly, float32 x, float32 y, float32 angle, float32 iof, QColor fill)
    : Object(world, poly, x, y, angle)
    , iof(iof)
{
    strokeColor = QColorConstants::Transparent;
    fillColor = fill;
}

bool Medium::getLightRayResponse(LightRay& ray, b2RayCastOutput& output) {
    bool tir;
    ray.direction = getRefractionVector(ray.direction, output.normal, ray.iof, iof, tir);
    if (!tir) ray.iof = iof;
    return true;
}

b2Vec2 Medium::getRefractionVector(b2Vec2& incident, b2Vec2& normal, float32 n1, float32 n2, bool& tir) {
    incident.Normalize();
    if (n1 == n2) {
        // don't bother calculating if the mediums are the same
        tir = false;
        return incident;
    }

    const float32 n = n1 / n2;
    const float32 cosI = -b2Dot(normal, incident);
    const float32 sinT2 = n * n * (1 - cosI * cosI);

    if (sinT2 > 1.0f) {
        // total internal reflection
        tir = true;
        return incident - (2 * b2Dot(incident, normal)) * normal;
    } else {
        // refraction
        tir = false;
        const float32 cosT = sqrt(1 - sinT2);
        return n * incident + (n * cosI - cosT) * normal;
    }
}
