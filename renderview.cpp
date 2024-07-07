#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QSceneLoader>
#include <Qt3DExtras>
#include <iostream>
#include <assert.h>

#include "renderview.h"
#include "ui_renderview.h"

#define CAMERA_DISTANCE 30.0f
#define CAMERA_HEIGHT   20.0f

const float Eps = 1E-6f;
const float Pi  = 3.14159265f;
const float cos45 = std::cos(M_PI / 4.0f); // Kosinus von 45 Grad
const float half = 0.5f;

RenderView::RenderView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderView),
    root(NULL),
    camera(NULL)
{
    ui->setupUi(this);
    
    node_2g = node_2h = node_2i = node_2j = NULL;
    node_3a = node_3b = NULL;
    nodeTree = nodeCross = NULL;

    // Create 3D render window and replace placeholder widget in UI
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(200, 255, 200));

    QWidget *container = QWidget::createWindowContainer(view);
    container->setGeometry(ui->viewarea->geometry());
    container->setSizePolicy(ui->viewarea->sizePolicy());
    container->setMinimumSize(ui->viewarea->minimumSize());
    
    ui->horizontalLayout->replaceWidget(ui->viewarea, container);
    delete ui->viewarea;
    ui->viewarea = container;
    
    // Create root node and box scene
    root = createScene();

    // add coord cross and tree to scene
    createCoordCrossAndTree(root);
    
    view->setRootEntity(root);
    
    // Parameterize camera
    setCameraAndParams(view->camera());
    

    
    // connect checkbox events to handlers
    QObject::connect(ui->checkBox_2g, &QCheckBox::stateChanged,
                     this, &RenderView::showTrafo2g);
    QObject::connect(ui->checkBox_2h, &QCheckBox::stateChanged,
                     this, &RenderView::showTrafo2h);
    QObject::connect(ui->checkBox_2i, &QCheckBox::stateChanged,
                     this, &RenderView::showTrafo2i);
    QObject::connect(ui->checkBox_2j, &QCheckBox::stateChanged,
                     this, &RenderView::showTrafo2j);

    QObject::connect(ui->checkBox_3a, &QCheckBox::stateChanged,
                     this, &RenderView::showTrafo3a);
    QObject::connect(ui->checkBox_3b, &QCheckBox::stateChanged,
                     this, &RenderView::showTrafo3b);

    QObject::connect(ui->checkBox_3c1, &QCheckBox::stateChanged,
                     this, &RenderView::showTrafo3c1);
    QObject::connect(ui->checkBox_3c2, &QCheckBox::stateChanged,
                     this, &RenderView::showTrafo3c2);
}

RenderView::~RenderView()
{
    delete ui;
}

Qt3DCore::QEntity* RenderView::createScene()
{
    // Root
    Qt3DCore::QEntity *scene = new Qt3DCore::QEntity();

    // base nodes for sub scenes for exercises 3.a and 3.b (2.g - 2.j)
    node_2g = new Qt3DCore::QEntity(scene);
    node_2h = new Qt3DCore::QEntity(scene);
    node_2i = new Qt3DCore::QEntity(scene);
    node_2j = new Qt3DCore::QEntity(scene);
    node_3a = new Qt3DCore::QEntity(scene);
    node_3b = new Qt3DCore::QEntity(scene);

    // Cube
    Qt3DExtras::QCuboidMesh *cubeMesh = new Qt3DExtras::QCuboidMesh();
    cubeMesh->setXExtent(2);
    cubeMesh->setYExtent(2);
    cubeMesh->setZExtent(2);

    Qt3DExtras::QPhongMaterial *cubeMat = new Qt3DExtras::QPhongMaterial();
    cubeMat->setDiffuse(QColor(255, 0, 0)); // can be reused for same color

    Qt3DCore::QTransform *cubeTransform = new Qt3DCore::QTransform();

    node_3a->addComponent(cubeMesh);
    node_3a->addComponent(cubeMat);
    node_3a->addComponent(cubeTransform);


    // TODO: 3.a)
    // Cube transformed according to exercise 2.g)

    QMatrix4x4 matrixForG;
    matrixForG.setRow(0, QVector4D(1, 0, 0, 1));
    matrixForG.setRow(1, QVector4D(0, 1, 0, 10));
    matrixForG.setRow(2, QVector4D(0, 0, 1, 1));
    matrixForG.setRow(3, QVector4D(0, 0, 0, 1));


    // Erstellen des Würfels
    Qt3DExtras::QCuboidMesh *transformedCubeMesh = new Qt3DExtras::QCuboidMesh();
    transformedCubeMesh->setXExtent(2); // Seitenlänge in x-Richtung
    transformedCubeMesh->setYExtent(2); // Seitenlänge in y-Richtung
    transformedCubeMesh->setZExtent(2); // Seitenlänge in z-Richtung

    Qt3DExtras::QPhongMaterial *transformedCubeMat = new Qt3DExtras::QPhongMaterial();
    transformedCubeMat->setDiffuse(QColor(0, 0, 255)); // Blaue Farbe

    Qt3DCore::QTransform *transformedCubeTransform = new Qt3DCore::QTransform();
    transformedCubeTransform->setMatrix(matrixForG); // Anwenden der Transformationsmatrix T auf den Würfel

    // Hinzufügen der Komponenten zum Entity-Knoten für den transformierten Würfel
    node_2g->addComponent(transformedCubeMesh);
    node_2g->addComponent(transformedCubeMat);
    node_2g->addComponent(transformedCubeTransform);



    // use node_2g

    
    // Cube transformed according to exercise 2.h)

    // Definieren der Transformationsmatrix H
    QMatrix4x4 matrixForH;
    matrixForH.setRow(0, QVector4D(1, 0, 0, 0));
    matrixForH.setRow(1, QVector4D(0, 1 / sqrt(2), -1 / sqrt(2), 0));
    matrixForH.setRow(2, QVector4D(0, 1 / sqrt(2), 1 / sqrt(2), 0));
    matrixForH.setRow(3, QVector4D(0, 0, 0, 1));

    // Erstellen des Würfels
    Qt3DExtras::QCuboidMesh *transformedCubeMeshH = new Qt3DExtras::QCuboidMesh();
    transformedCubeMeshH->setXExtent(2); // Seitenlänge in x-Richtung
    transformedCubeMeshH->setYExtent(2); // Seitenlänge in y-Richtung
    transformedCubeMeshH->setZExtent(2); // Seitenlänge in z-Richtung

    Qt3DExtras::QPhongMaterial *transformedCubeMatH = new Qt3DExtras::QPhongMaterial();
    transformedCubeMatH->setDiffuse(QColor(0, 0, 255)); // Blaue Farbe

    Qt3DCore::QTransform *transformedCubeTransformH = new Qt3DCore::QTransform();
    transformedCubeTransformH->setMatrix(matrixForH); // Anwenden der Transformationsmatrix H auf den Würfel


    node_2h->addComponent(transformedCubeMeshH);
    node_2h->addComponent(transformedCubeMatH);
    node_2h->addComponent(transformedCubeTransformH);



    // use node_2h

    
    // Cube transformed according to exercise 2.i)
    // use node_2i

    // Cube transformed according to exercise 2.i)
    // use node_2i

    // Cube transformed according to exercise 2.i)
    // use node_2i

    QMatrix4x4 matrixForI;
    matrixForI.setRow(0, QVector4D(1, 0, 0, 0));
    matrixForI.setRow(1, QVector4D(0, 0, 1, 1));
    matrixForI.setRow(2, QVector4D(0, -1, 0, 1));
    matrixForI.setRow(3, QVector4D(0, 0, 0, 1));


    // Erstellen des Würfels
    Qt3DExtras::QCuboidMesh *transformedCubeMeshI = new Qt3DExtras::QCuboidMesh();
    transformedCubeMeshI->setXExtent(2); // Seitenlänge in x-Richtung
    transformedCubeMeshI->setYExtent(2); // Seitenlänge in y-Richtung
    transformedCubeMeshI->setZExtent(2); // Seitenlänge in z-Richtung

    Qt3DExtras::QPhongMaterial *transformedCubeMatI = new Qt3DExtras::QPhongMaterial();
    transformedCubeMatI->setDiffuse(QColor(0, 0, 255)); // Blaue Farbe

    Qt3DCore::QTransform *transformedCubeTransformI = new Qt3DCore::QTransform();
    transformedCubeTransformI->setMatrix(matrixForI); // Anwenden der Transformationsmatrix I auf den Würfel


    node_2i->addComponent(transformedCubeMeshI);
    node_2i->addComponent(transformedCubeMatI);
    node_2i->addComponent(transformedCubeTransformI);



    
    // // Cube transformed according to exercise 2.j)

    Qt3DCore::QTransform *cubeTransform2J = new Qt3DCore::QTransform();

    cubeTransform2J->setMatrix(
        QMatrix4x4(
            0, -cos45, cos45,1,
            cos45, half, half, -cos45,
            -cos45, half, half, cos45,
            0, 0,  0 , 1));

    Qt3DExtras::QPhongMaterial *cubeMat2J = new Qt3DExtras::QPhongMaterial();
    cubeMat2J->setDiffuse(QColor(255,255,255));

    node_2j->addComponent(cubeMesh);
    node_2j->addComponent(cubeMat2J);
    node_2j->addComponent(cubeTransform2J);

    // // use node_2j
    // QMatrix4x4 matrixForJ;


    // matrixForJ.setRow(0, QVector4D(1, -1/sqrt(2), 1/sqrt(2), 1));
    // matrixForJ.setRow(1, QVector4D(1/sqrt(2), 3/2, -1/2, 1-(1/(2*sqrt(2)))));
    // matrixForJ.setRow(2, QVector4D(-1/sqrt(2), -1/2, 3/2, 1+(1/(2*sqrt(2)))));
    // matrixForJ.setRow(3, QVector4D(0, 0, 0, 1));




    // Qt3DExtras::QCuboidMesh *transformedCubeMeshJ = new Qt3DExtras::QCuboidMesh();
    // transformedCubeMeshJ->setXExtent(2); // Seitenlänge in x-Richtung
    // transformedCubeMeshJ->setYExtent(2); // Seitenlänge in y-Richtung
    // transformedCubeMeshJ->setZExtent(2); // Seitenlänge in z-Richtung

    // Qt3DExtras::QPhongMaterial *transformedCubeMatJ = new Qt3DExtras::QPhongMaterial();
    // transformedCubeMatJ->setDiffuse(QColor(0, 0, 255)); // Blaue Farbe

    // Qt3DCore::QTransform *transformedCubeTransformJ = new Qt3DCore::QTransform();
    // transformedCubeTransformJ->setMatrix(matrixForJ); // Anwenden der Transformationsmatrix J auf den Würfel


    // // Hinzufügen der Komponenten zum Entity-Knoten für den transformierten Würfel
    // node_2j->addComponent(transformedCubeMeshJ);
    // node_2j->addComponent(transformedCubeMatJ);
    // node_2j->addComponent(transformedCubeTransformJ);

    Qt3DExtras::QCuboidMesh *transformedCubeMeshJ2 = new Qt3DExtras::QCuboidMesh();
    transformedCubeMeshJ2->setXExtent(2); // Seitenlänge in x-Richtung
    transformedCubeMeshJ2->setYExtent(2); // Seitenlänge in y-Richtung
    transformedCubeMeshJ2->setZExtent(2); // Seitenlänge in z-Richtung

    Qt3DExtras::QPhongMaterial *mat = new Qt3DExtras::QPhongMaterial();
    mat->setDiffuse(QColor(223, 95, 249));

    //Translation to (1,1,1)
    Qt3DCore::QTransform *translation1 = new Qt3DCore::QTransform();
    translation1->setTranslation(QVector3D(1,1,1));

    //Rotation um axe parallel zu (0,1,1) um 90°
    Qt3DCore::QTransform *rotation = new Qt3DCore::QTransform();
    rotation->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0,1,1).normalized(), 90));

    //Translation back to OG positon
    Qt3DCore::QTransform *translation2 = new Qt3DCore::QTransform();
    translation2->setTranslation(QVector3D(-1,-1,-1));

    //Combining the transormations
    Qt3DCore::QTransform *combinedTransformation = new Qt3DCore::QTransform();
    combinedTransformation->setTranslation(QVector3D(1,1,1));
    QMatrix4x4 matrix;
    matrix.translate(1,1,1); //First translate
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,1).normalized(), 90)); //Then rotate
    matrix.translate(-1,-1,-1); //Then Translate Back
    combinedTransformation->setMatrix(matrix);

    //Entity for the Cube
    node_3b->addComponent(cubeMesh);
    node_3b->addComponent(mat);
    node_3b->addComponent(combinedTransformation);

    
    // // TODO: 3.b)
    // // Cube transformed according to exercise 2.j), but with transformation
    // // hierarchy instead of single matrix (note that here node_3b should be
    // // lowest in hierarchy to make toggling visibility work in that case).
    // // Uses another color to be able to observe the depth buffer problems.
    // cubeMat = new Qt3DExtras::QPhongMaterial();
    // cubeMat->setDiffuse(QColor(0, 255, 255));
    // // use node_3b

    
    return scene;
}
void RenderView::createCoordCrossAndTree(Qt3DCore::QEntity *parent)
{
    // Create the root node for the coordinate cross
    nodeCross = new Qt3DCore::QEntity(parent);

    // Y-axis
    createAxis(nodeCross, QVector3D(1, 0, 0), QColor(255, 0, 0)); // Red
    // X-axis
    createAxis(nodeCross, QVector3D(0, 1, 0), QColor(0, 255, 0)); // Green
    // Z-axis
    createAxis(nodeCross, QVector3D(0, 0, 1), QColor(0, 0, 255)); // Blue
}

void RenderView::createAxis(Qt3DCore::QEntity *parent, const QVector3D &axis, const QColor &color)
{
    // Create cylinder for the axis
    Qt3DExtras::QCylinderMesh *cylinder = new Qt3DExtras::QCylinderMesh();
    cylinder->setRadius(0.1f);
    cylinder->setLength(10.0f);

    Qt3DCore::QTransform *cylinderTransform = new Qt3DCore::QTransform();
    cylinderTransform->setScale(1.0f);
    cylinderTransform->setTranslation(axis * 5.0f); // Position cylinder along the axis

    if (axis == QVector3D(1, 0, 0)) // X-axis
    {
        cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), 90));
    }
    else if (axis == QVector3D(0, 1, 0)) // Y-axis
    {
        // No rotation needed for Y-axis
    }
    else if (axis == QVector3D(0, 0, 1)) // Z-axis
    {
        cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 90));
    }

    Qt3DExtras::QPhongMaterial *cylinderMaterial = new Qt3DExtras::QPhongMaterial();
    cylinderMaterial->setDiffuse(color);

    Qt3DCore::QEntity *cylinderEntity = new Qt3DCore::QEntity(parent);
    cylinderEntity->addComponent(cylinder);
    cylinderEntity->addComponent(cylinderTransform);
    cylinderEntity->addComponent(cylinderMaterial);

    // Create cone for the arrow head
    Qt3DExtras::QConeMesh *cone = new Qt3DExtras::QConeMesh();
    cone->setBottomRadius(0.2f);
    cone->setLength(1.0f);

    Qt3DCore::QTransform *coneTransform = new Qt3DCore::QTransform();
    coneTransform->setScale(1.0f);
    coneTransform->setTranslation(axis * 10.5f); // Position cone at the end of the axis

    if (axis == QVector3D(1, 0, 0)) // X-axis
    {
        coneTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), -90));
    }
    else if (axis == QVector3D(0, 1, 0)) // Y-axis
    {
        // No rotation needed for Y-axis
    }
    else if (axis == QVector3D(0, 0, 1)) // Z-axis
    {
        coneTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 90));
    }

    Qt3DExtras::QPhongMaterial *coneMaterial = new Qt3DExtras::QPhongMaterial();
    coneMaterial->setDiffuse(color);

    Qt3DCore::QEntity *coneEntity = new Qt3DCore::QEntity(parent);
    coneEntity->addComponent(cone);
    coneEntity->addComponent(coneTransform);
    coneEntity->addComponent(coneMaterial);
}


void RenderView::setCameraAndParams(Qt3DRender::QCamera *cam)
{
    cam->lens()->setPerspectiveProjection(45.0f, 1.0f, 0.1f, 1000.0f);
    
    cam->setViewCenter(QVector3D(0, 0, 0));
    cam->setPosition(QVector3D(0, CAMERA_HEIGHT, CAMERA_DISTANCE));
    
    camera = cam;
    
    // Use ugly built-in camera controls to allow changing viewpoint with mouse
    // https://doc.qt.io/qt-6/qt3dextras-qorbitcameracontroller.html
    Qt3DExtras::QOrbitCameraController *camController =
                            new Qt3DExtras::QOrbitCameraController(root);
    camController->setCamera(cam);
    camController->setLinearSpeed(50.0f);
    camController->setLookSpeed(100.0f);
}


Qt3DCore::QTransform* applyTransformation(Qt3DCore::QEntity *node, QMatrix4x4 matrix) {
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setMatrix(matrix);
    node->addComponent(transform);
    return transform;
}

void RenderView::createSceneTransforms() {
    // Assuming matrices are provided or calculated as needed for each exercise
    QMatrix4x4 matrix2g, matrix2h, matrix2i, matrix2j;
    // Initialize matrices with transformations

    applyTransformation(node_2g, matrix2g);
    applyTransformation(node_2h, matrix2h);
    applyTransformation(node_2i, matrix2i);
    applyTransformation(node_2j, matrix2j);

    // For hierarchical transformations in exercise 3.b
    QMatrix4x4 part1, part2, part3, part4;
    // Apply parts of the transformation
    Qt3DCore::QEntity *part1Entity = new Qt3DCore::QEntity(node_3b);
    applyTransformation(part1Entity, part1);
    Qt3DCore::QEntity *part2Entity = new Qt3DCore::QEntity(part1Entity);
    applyTransformation(part2Entity, part2);
    // Repeat for part3 and part4
}


void RenderView::showTrafo2g(bool visible)
{
    if (node_2g)
        node_2g->setEnabled(visible);
}

void RenderView::showTrafo2h(bool visible)
{
    if (node_2h)
        node_2h->setEnabled(visible);
}

void RenderView::showTrafo2i(bool visible)
{
    if (node_2i)
        node_2i->setEnabled(visible);
}

void RenderView::showTrafo2j(bool visible)
{
    if (node_2j)
        node_2j->setEnabled(visible);
}

void RenderView::showTrafo3a(bool visible)
{
    if (node_3a)
        node_3a->setEnabled(visible);
}

void RenderView::showTrafo3b(bool visible)
{
    if (node_3b)
        node_3b->setEnabled(visible);
}

void RenderView::showTrafo3c1(bool visible)
{
    if (nodeTree)
        traverseToToggleVisib(nodeTree, visible);
}

void RenderView::showTrafo3c2(bool visible)
{
    if (nodeCross)
        traverseToToggleVisib(nodeCross, visible);
}

void RenderView::traverseToToggleVisib(Qt3DCore::QNode *node, bool visible)
{
    Qt3DCore::QNodeVector nodeVec = node->childNodes();

    for (int i=0; i<nodeVec.length(); i++) {
        if (nodeVec[i])
            traverseToToggleVisib(nodeVec[i], visible);
    }

    node->setEnabled(visible);
}
