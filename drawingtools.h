#ifndef DRAWINGTOOLS_H
#define DRAWINGTOOLS_H

#include <QImage>
#include <QPointF>

class DrawingTools
{
public:
    enum ToolType {PEN, FILL};
    static ToolType currentTool;
    static QColor toolColor;
    static void useCurrentTool(QImage&, QPoint&);

private:
    // disallow instancing
    DrawingTools() {}

    static void usePen(QImage&, QPoint&);
    static void useFill(QImage&, QPoint, QColor&);
    static bool isPointInImage(QImage&, QPoint&);

};

#endif // DRAWINGTOOLS_H
