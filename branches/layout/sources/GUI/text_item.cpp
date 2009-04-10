/**
 * File: GUI/text_item.cpp - Implementation of text item in MiptVis
 * Copyright (C) 2009  Ivan Mashintsev
 */
#include "gui_impl.h"


/**
 * Constructor for class Text. Create element of the class
 */
Text::Text( QGraphicsItem * parent, QGraphicsScene *scene):QGraphicsTextItem( parent, scene)
{
    setPlainText( "Hello World");
    setTextWidth ( 100); //Set width of node
    setFlag( QGraphicsItem::ItemIsMovable, true); // Set node can move
    setFlag( QGraphicsItem::ItemIsSelectable, true); // Set node can select
    setTextInteractionFlags( Qt::NoTextInteraction);
}

/**
 * When forus in the text of the node function run
 */
void Text::focusInEvent( QFocusEvent * event)
{
    setSelected(true);
    QGraphicsTextItem::focusInEvent(event);
}

/**
 * When forus out the text of the node function run
 */
void Text::focusOutEvent( QFocusEvent * event)
{
    setTextInteractionFlags( Qt::NoTextInteraction);
    QGraphicsTextItem::focusOutEvent(event);
}

/**
 * Edit text in the node after double click mouse
 */
void Text::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if ( textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags( Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent( mouseEvent);
}

/**
 * 
 */
void Text::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    if( mouseEvent->button() & Qt::RightButton)
    {
    }
    update();
    QGraphicsTextItem::mousePressEvent( mouseEvent);
}

/**
 * 
 */
void Text::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsTextItem::mouseReleaseEvent( event);
}

/**
 * Draw rectangle around the node
 */
void Text::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->fillRect( boundingRect(), QBrush( Qt::green));
    QGraphicsTextItem::paint( painter, option, widget);
    myPolygon << (boundingRect().bottomLeft()) << (boundingRect().bottomRight())
                      << (boundingRect().topRight()) << (boundingRect().topLeft())
                      << (boundingRect().bottomLeft());
}

/**
 * Get bounding rectangle of the node
 */
QRectF Text::boundingRect() const
{
    qreal adjust = 3;
    return QGraphicsTextItem::boundingRect()
               .adjusted( -adjust, -adjust, adjust, adjust);
}

/**
 * 
 */
void Text::removeLine( Line *line)
{
    int index = lines.indexOf( line);

    if (index != -1)
        lines.removeAt( index);
}

/**
 * 
 */
void Text::removeLines()
{
    foreach (Line *line, lines)
    {
        line->startItem()->removeLine( line);
        line->endItem()->removeLine( line);
        scene()->removeItem( line);
        delete line;
    }
}

/**
 * 
 */
void Text::addLine(Line *line)
{
    lines.append( line);
}

/**
 * 
 */
QVariant Text::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange)
    {
        foreach( Line *line, lines)
        {
            line->updatePosition();
        }
    }
    return value;
}