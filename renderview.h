#ifndef RENDERVIEW_H
#define RENDERVIEW_H
#include <QWidget>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>

namespace Ui {
    class RenderView;
}

class RenderView : public QWidget
{
    Q_OBJECT

public:
    explicit RenderView(QWidget *parent = 0);
    ~RenderView();

protected:
    // returns root node of 3d scene
    Qt3DCore::QEntity* createScene();

    // creates sub trees for coordinate cross and tree shape
    void createCoordCrossAndTree(Qt3DCore::QEntity *parent);
    
    // currently only creates headlight
    void createLights();
    
    // sets camera parameters along with controller
    void setCameraAndParams(Qt3DRender::QCamera *cam);

    void createAxis(Qt3DCore::QEntity *parent, const QVector3D &axis, const QColor &color);

    void createSceneTransforms();
    
protected slots:
    void showTrafo2g(bool visible);
    void showTrafo2h(bool visible);
    void showTrafo2i(bool visible);
    void showTrafo2j(bool visible);
    void showTrafo3a(bool visible);
    void showTrafo3b(bool visible);
    void showTrafo3c1(bool visible);
    void showTrafo3c2(bool visible);

private:
    void traverseToToggleVisib(Qt3DCore::QNode *node, bool visible);

    Ui::RenderView *ui;
    
    Qt3DCore::QEntity *root;      // root node of scene graph
    
    Qt3DRender::QCamera *camera;  // virtual camera

    Qt3DCore::QEntity *node_2g,   // holds subtree for exercise 3.a (2.g)
                      *node_2h,   // holds subtree for exercise 3.a (2.h)
                      *node_2i,   // holds subtree for exercise 3.a (2.i)
                      *node_2j,   // holds subtree for exercise 3.a (2.j)
                      *node_3a,   // holds subtree with untransformed box
                      *node_3b,   // holds subtree for exercise 3.b (2.j)
                      *nodeCross, // holds subtree for coordinate cross
                      *nodeTree;  // holds subtree for simple tree shape
};

#endif // RENDERVIEW_H
