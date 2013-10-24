
#include <QSurfaceFormat>
#include <QWidget>

#include "Application.h"

#include "Viewer.h"
#include "Canvas.h"


int main(int argc, char* argv[])
{
    int result = -1;

    Application app(argc, argv);
    {
        QScopedPointer<Viewer> viewer(new Viewer());

        QSurfaceFormat format;
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);

        Canvas * canvas = new Canvas(format);
        QWidget * widget = QWidget::createWindowContainer(canvas);
        widget->setMinimumSize(1, 1);
        widget->setAutoFillBackground(false); // Important for overdraw, not occluding the scene.
        widget->setFocusPolicy(Qt::TabFocus);

        viewer->setCentralWidget(widget);
        viewer->show();

        result = app.exec();
    }
    return result;
}