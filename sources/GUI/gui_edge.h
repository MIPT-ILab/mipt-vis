/**
 * File: GUI/GuiEdge.h - Implementational header for GuiEdge of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_EDGE_H
#define GUI_EDGE_H

/**
 * Implements collection of edge properties and provides accessors for them
 */
class EdgeProperties
{
    int prob_priv;
	int thickness_priv;
	char * color_priv;
	char * label_priv;
	
	StId style_priv;
	StyleSheet *ss_priv;
public:
    EdgeProperties (StyleSheet* ss)
		:ss_priv (ss)
    {
        /** Default values */
        prob_priv = 1;
	    thickness_priv = 1;
	    color_priv = NULL;
	    style_priv = NULL;
	    label_priv = NULL;
    }
    inline int prob() const
    {
        return prob_priv;
    }
    inline int thickness() const
    {
        return thickness_priv;
    }
	inline char * color() const
    {
        return color_priv;
    }
    inline char * label() const
    {
        return label_priv;
    }
	inline const QString& stName() const
    { 
        return ss_priv->getStName (style_priv);
    }
	inline void applStyle (QPainter * painter)
    { 
        ss_priv->applayStyle (style_priv, painter);
    }
    /** Data writing routines */
    inline void setProb( int prob)
    {
        prob_priv = prob;
    }
	inline void setThickness( int thickness)
    {
        thickness_priv = thickness;
    }
	inline void setColor( char * color)
    {
        color_priv = color;
    }
	inline void setLabel( char * label)
    {
        label_priv = label;
    }
	inline void setStyle (const QString& stName) 
    { 
        style_priv = ss_priv->getId (stName);
    }
};

/**
 * Class GuiEdge. GuiEdge is movable. Double click create new point on the GuiEdge
 */
class GuiEdge:public QGraphicsItem, public EdgeAux, public EdgeProperties
{
    QPointF startP;
	QPointF startDir;
    QPointF endP;
	QPointF endDir;

	QPainterPath curve;
	bool valid;

    QPointF topLeft;
    QPointF btmRight;

public:
    enum { Type = QGraphicsItem::UserType + 2};//!!!I do not know what type here needed
public:
    void updatePosition();
    QRectF boundingRect() const;
    QPainterPath shape() const;
	GuiNode* insertNode ( QPointF p);
    int type() const
    {
        return Type;
    }

protected:
    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void mousePressEvent( QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event);
	
	virtual void writeByXMLWriter( xmlTextWriterPtr writer);
	virtual void readByXML ( xmlNode * cur_node);
private:
    GuiEdge( GuiGraph * graph_p, int _id, StyleSheet* ss, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
	virtual ~GuiEdge();
    friend class GuiGraph;
    friend class GuiNode;
    friend class GraphAux;
    friend class NodeAux;
};
#endif
