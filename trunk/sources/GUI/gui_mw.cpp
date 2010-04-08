/**
 * File: GUI/GuiMW.cpp - Implementation of MainWindow in MiptVis
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#include <QtGui/QtGui>
#include <QtCore/QTextCodec>
#include "gui_mw.h"
#include "../Parser/parser_iface.h"
/**
 * Constructor of MainWindow class
 */
MainWindow::MainWindow()
{
    current_file = "";
	setCurrentFile( current_file);

    graph = new GuiGraph();
    graph->setSceneRect( QRectF( 0, 0, 5000, 5000));
    connect( graph, SIGNAL( newNodeCreated( int)), this, SLOT( addNewTextDock( int)));

    view = new GuiView(graph);
    view->setScene(graph);
	
    if( graph->getNodeItem())
      view->centerOn( graph->getNodeItem());
       

    view_layout = new QHBoxLayout;
    view_layout->addWidget( view);

    view_widget = new QWidget;
    view_widget->setLayout(view_layout);
	setCentralWidget( view_widget);

	gravity_timer = new QTimer ( this);
	connect( gravity_timer, SIGNAL( timeout()), this, SLOT(makeGravity()));
	gravity_timer->setInterval ( 100);
    
	createActions();
    createMenus();
    createStatusBar();
	createDockWindows();
	createToolBars();
	createHotKeys();
}

/**
 * Load a file
 */
void MainWindow::load()
{
	closeOldDocks();

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    current_file = QFileDialog::getOpenFileName( this, tr( "Open File"), ".", tr( "XML (*.xml);;All files(*.*)")); 
    if ( current_file.isEmpty()) return;
    QByteArray cur_file = current_file.toAscii();
    char *file;
    file = (char*) calloc( cur_file.size(), sizeof(char));
    if (file==NULL) return;
    int i;
    for (i=0;i<=cur_file.size();i++) file[i]=cur_file[i];

    QApplication::setOverrideCursor( Qt::WaitCursor);

    if ( graph!=NULL) delete graph;


    graph = new GuiGraph( file);
    connect( graph, SIGNAL( newNodeCreated( int)), this, SLOT( addNewTextDock( int)));
	view->setScene( graph);

	createConnectionsToNodes();

    QApplication::restoreOverrideCursor();

    setCurrentFile( current_file);

	search_text_edit->clear();
	search_result_list->clear();
	search_result_dock->hide();
}

/**
 * Save current file
 */
void MainWindow::save()
{
	saveNodeTexts();

    current_file = QFileDialog::getSaveFileName( this, tr("Save File"), "", tr("XML (*.xml);;All files(*.*)")); 
    if ( current_file.isEmpty()) return;
    QByteArray cur_file = current_file.toAscii();
    char *file;
    file = ( char*) calloc( cur_file.size(), sizeof( char));
    if ( file==NULL) return;
    int i;
    for ( i=0; i<=cur_file.size(); ++i) file[i] = cur_file[i];
    graph->writeToXml( file);
    setCurrentFile( current_file);
}

/**
 * HotKeysInfo
 */
void MainWindow::hotKeysInfo()
{
	QMessageBox::about( this, tr("Hot Keys"), tr("Available Hot Keys\n\'+\':scaling view closer\n\'-\':scaling view off"));
}

/**
 * Help
 */
void MainWindow::help()
{
    QMessageBox::about( this, tr("Mipt-Vis Help"),
      tr("To open a file :  On the File menu, click Open\n\n"\
         "To save the current file :  On the File menu, click Save\n\n"\
         "To zoom in and out :  Use the mouse wheel"));
}

/**
 * About
 */
void MainWindow::about()
{
    QMessageBox::about( this, tr("About Mipt-Vis"),
      tr("  Mipt-Vis is a project that is developed in the Intel-MIPT "
         "students laboratory by a group of students from "
         "Moscow Institute of Physics and Technology( MIPT) "
         "under Intel employees mentoring.\n\n  Mipt-vis is an "
         "open source software product that helps you to "
         "visualize and perform different analyses on "
         "compiler-specific graphs. Also it is educational project "
         "that gives its members more knowledge about compiler science "
         "and work in team.\n\n  There are four components: "
         "GUI( mentor Rumyantsev Lev), Graph placement( mentor Shurygin Boris), "
         "Graph analyses( mentor Zolotukhin Michael) and Parser( mentor Sidorenko Ivan). \n\n"
         "  At first IR from GCC and ICC will be supported."));
}

/**
 * HotKeys
 */
void MainWindow::createHotKeys()
{   
	(void) new QShortcut(Qt::Key_Plus, this, SLOT(zoomViewIn(void)));	
	(void) new QShortcut(Qt::Key_Equal, this, SLOT(zoomViewIn(void)));
	(void) new QShortcut(Qt::Key_Minus, this, SLOT(zoomViewOut(void)));
}

/**
 * Do Layout
 */
void MainWindow::doLayoutSlot()
{
    graph->doLayout();//!!! An error occurs when after layout, one calls load
    update();//!!! When the edges' successors are changed, they continue point to old ones !!!
}

/**
 * enableGravity
 */
void MainWindow::enableGravity()
{
	gravity_timer->start();
}

/**
 * disableGravity
 */
void MainWindow::disableGravity()
{
	gravity_timer->stop();
}

/**
 * enableGravity
 */
void MainWindow::makeGravity()
{
	graph->iterateGravity();
}

/*
 *Switch showing virtual nodes in graph
 */
void MainWindow::switchVnodesShow()
{
	graph->switchVnodesShow();
}
/*
 *Switch showing edge notes in graph
 */
void MainWindow::switchEdgeLabelsShow()
{
	graph->switchEdgeLabelsShow();
}
/**
 * convertDumpToXML
 */
void MainWindow::convertDumpToXmlSlot()
{
	convertDumpToXml( this);
} 


/**
 * centreOnNode
 */
void MainWindow::centreOnNode()
{
	int nodeId = centre_on_node_spin_box->value();
    doCentreOnNode( nodeId);
} 

/**
 * doCentreOnNode
 */
void MainWindow::doCentreOnNode( int nodeNumber)
{
    GuiNode * node;
    for ( node = ( GuiNode *)graph->firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		if ( node->userId() == nodeNumber)
		{
			view->centerOn( node);
			break;
		}
} 

/**
 * createConnectionsToNodes
 */
void MainWindow::createConnectionsToNodes()
{
    GuiNode * node;
    for ( node = ( GuiNode *)graph->firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		{
			connect(node, SIGNAL( createNodeTextDock( int)), this, SLOT( addNewTextDock( int)));
		}
} 

/**
 * createDockWindows
 */
void MainWindow::createDockWindows()
{
	/* set corners to left and right dock areas */
	setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
	setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
	setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea);
	setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea);

	/* centreOnNode dock*/
	centre_on_node_dock = new QDockWidget( tr("Centre on node"),this);
    centre_on_node_dock->setAllowedAreas( Qt::AllDockWidgetAreas);
	centre_on_node_dock->setFloating( false);

	centre_on_node_spin_box = new QSpinBox;
	centre_on_node_spin_box->setRange(0,1000);

    centre_on_node_button = new QPushButton( tr("Go to node"));
    centre_on_node_button->setEnabled( true);
    centre_on_node_button->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    QObject::connect( centre_on_node_button, SIGNAL( clicked()), this, SLOT( centreOnNode()));

	centre_on_node_layout = new QHBoxLayout( centre_on_node_dock);
	centre_on_node_layout->addWidget( centre_on_node_spin_box);
	centre_on_node_layout->addWidget( centre_on_node_button);

    centre_on_node_widget = new QWidget;
    centre_on_node_widget->setLayout( centre_on_node_layout);
	centre_on_node_widget->setMaximumWidth( 150);

	centre_on_node_dock->setWidget(centre_on_node_widget);
    addDockWidget( Qt::TopDockWidgetArea, centre_on_node_dock, Qt::Horizontal);
	view_menu->addAction(centre_on_node_dock->toggleViewAction());

	/* Search dock widget*/
	search_dock = new QDockWidget( tr("Find node"),this);
    search_dock->setAllowedAreas( Qt::AllDockWidgetAreas);
	search_dock->setFloating( false);

	search_text_edit = new QLineEdit;

	search_push_button = new QPushButton( tr("Find text"));
    search_push_button->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
	QObject::connect( search_push_button, SIGNAL( clicked()), this, SLOT( searchButtonClicked()));

	search_layout = new QHBoxLayout( search_dock);
	search_layout->addWidget( search_text_edit);
	search_layout->addWidget( search_push_button);

	search_widget = new QWidget;
	search_widget->setLayout( search_layout);
	search_widget->setMaximumWidth( 250);

	search_dock->setWidget(search_widget);
    addDockWidget( Qt::TopDockWidgetArea, search_dock, Qt::Horizontal);
	view_menu->addAction(search_dock->toggleViewAction());

	/* Search result dock widget*/
	search_result_dock = new QDockWidget( tr("Search result"), this);
    search_result_dock->setAllowedAreas( Qt::AllDockWidgetAreas);
	search_result_dock->setFloating( false);

	search_result_list = new QListWidget;
	QObject::connect(search_result_list, SIGNAL( itemDoubleClicked( QListWidgetItem *)), 
		             this, SLOT( nodeClickedFromList( QListWidgetItem *)));

	search_result_layout = new QVBoxLayout( search_result_dock);
	search_result_layout->addWidget( search_result_list);

	search_result_widget = new QWidget;
	search_result_widget->setLayout( search_result_layout);
	search_result_widget->setMaximumHeight( 80);

	search_result_dock->setWidget(search_result_widget);
    addDockWidget( Qt::RightDockWidgetArea, search_result_dock, Qt::Vertical);
	
	search_result_dock->hide();
}

/**
 * Creat actions
 */
void MainWindow::createActions()
{
    load_act =  new QAction( QIcon("../GUI/images/load.bmp"),tr("&Load"), this);
    load_act->setStatusTip( tr( "Load..."));
    connect( load_act, SIGNAL( triggered()), this, SLOT( load()));

    save_act = new QAction( QIcon("../GUI/images/save.bmp"),tr( "&Save"), this);
    save_act->setShortcuts(QKeySequence::Save);
    save_act->setStatusTip( tr( "Save..."));
    connect( save_act, SIGNAL( triggered()), this, SLOT( save()));

    help_act = new QAction(QIcon("../GUI/images/help.bmp"), tr( "Mipt-Vis &Help"), this);
    help_act->setStatusTip( tr( "Help"));
    connect( help_act, SIGNAL( triggered()), this, SLOT( help()));

    about_act = new QAction(QIcon("../GUI/images/about.bmp"), tr( "&About Mipt-Vis"), this);
    about_act->setStatusTip( tr( "About"));
    connect( about_act, SIGNAL(triggered()), this, SLOT( about()));

    do_layout_act = new QAction( QIcon("../GUI/images/setLayout.bmp"),tr( "Do &Layout"), this);
    do_layout_act->setStatusTip( tr( "Do Layout"));
    connect( do_layout_act, SIGNAL( triggered()), this, SLOT( doLayoutSlot()));

    convert_dump_to_xml_act = new QAction(QIcon("../GUI/images/conversion.bmp"),tr( "&Convert dump to XML..."), this);
    convert_dump_to_xml_act->setStatusTip( tr( "Convert dump to XML..."));
    connect( convert_dump_to_xml_act, SIGNAL( triggered()), this, SLOT( convertDumpToXmlSlot()));
		
    show_virtual_nodes_act = new QAction(QIcon("../GUI/images/node_icon.bmp"),tr( "Show &pseudonodes"), this);
    show_virtual_nodes_act->setStatusTip( tr( "Show pseudonodes trigger"));
	show_virtual_nodes_act->setCheckable(true);
	show_virtual_nodes_act->setChecked(false);
    connect( show_virtual_nodes_act, SIGNAL( triggered()), this, SLOT( switchVnodesShow()));
	
    show_edge_labels_act = new QAction(QIcon("../GUI/images/showLabel.bmp"),tr( "Show &labels on graph edges"), this);
    show_edge_labels_act->setStatusTip( tr( "Show labels on graph edges"));
	show_edge_labels_act->setCheckable(true);
	show_edge_labels_act->setChecked(false);
	connect( show_edge_labels_act, SIGNAL( triggered()), this, SLOT( switchEdgeLabelsShow()));

	do_gravity_act = new QAction(QIcon("../GUI/images/enGravityAct.bmp"),tr( "&Change Gravity"),this);
	do_gravity_act->setStatusTip( tr( "Change Gravity"));
	do_gravity_act->setCheckable(true);
	do_gravity_act->setChecked(false);
	connect(do_gravity_act, SIGNAL(toggled(bool)), this, SLOT(reactToGravityToggle(bool)));
	
	hot_keys_act = new QAction(QIcon("../GUI/images/HotKeys.bmp"),tr( "&Hot keys"),this);
	hot_keys_act->setStatusTip( tr( "Hot keys"));
	connect(hot_keys_act, SIGNAL(triggered()), this, SLOT(hotKeysInfo()));

}

/**
 * Creat menus
 */
void MainWindow::createMenus()
{
    file_menu = menuBar()->addMenu( tr( "&File"));
    file_menu->addAction( load_act);
    file_menu->addAction( save_act);

    view_menu = menuBar()->addMenu( tr( "&View"));

    tools_menu = menuBar()->addMenu( tr( "&Tools"));
    tools_menu->addAction( do_layout_act);
    tools_menu->addAction( do_gravity_act);
    tools_menu->addAction( convert_dump_to_xml_act);
    tools_menu->addAction( show_edge_labels_act);

	settings_menu = menuBar()->addMenu( tr("&Settings"));
	settings_menu->addAction(hot_keys_act);

    help_menu = menuBar()->addMenu( tr( "&Help"));
    help_menu->addAction( help_act);
    help_menu->addAction( about_act);
}

/**
 * Create status bar
 */
void MainWindow::createStatusBar()
{
    statusBar()->showMessage( tr( "Ready"));
}

/**
 * Add new text dock
 */
void MainWindow::addNewTextDock(int number)
{
    GuiNode * node;
    for ( node = ( GuiNode *)graph->firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		if ( node->userId() == number) 
			{
				if (node->text_dock == NULL)
				{
					node->text_dock = new QDockWidget( QString( "Node %1").arg( node->userId()),this);
					node->text_dock->setAllowedAreas( Qt::AllDockWidgetAreas);
					node->text_dock->setFloating( false);

					node->text_edit = new GuiTextEdit;
					node->text_edit->setPlainText( node->getNodeText());
					node->text_edit->setReadOnly( false);

					node->text_layout = new QVBoxLayout( node->text_dock);
					node->text_layout->addWidget( node->text_edit);

					node->text_widget = new QWidget;
					node->text_widget->setLayout( node->text_layout);
					node->text_widget->setMaximumHeight( 80);

					node->text_dock->setWidget( node->text_widget);

					addDockWidget( Qt::RightDockWidgetArea, node->text_dock, Qt::Vertical);
					connect(node->text_edit, SIGNAL( nodeToBeCentreOn( int)), this, SLOT( doCentreOnNode( int)));
				}
				node->text_dock->show();
			}
}

/**
 * setCurrentFile
 */
void MainWindow::setCurrentFile( const QString & file_name)
{
    QString shown_name;
    if ( file_name.isEmpty()) shown_name = "untitled.xml";
    else shown_name = strippedName( file_name);
    setWindowTitle( tr("%1[*] - %2").arg( shown_name).arg( tr("MIPT-Vis")));
}

/**
 * strippedName
 */
QString MainWindow::strippedName( const QString &full_file_name)
{
    return QFileInfo( full_file_name).fileName();
} 
/**
 * createToolBars
 */
void MainWindow::createToolBars()
{
    tool_bar = addToolBar (tr("File"));
	tool_bar->addAction (load_act);
    tool_bar->addAction (save_act);
	tool_bar->addAction (do_layout_act);
	tool_bar->addAction (convert_dump_to_xml_act);
	tool_bar->addAction (show_virtual_nodes_act);
	tool_bar->addAction (show_edge_labels_act);
	tool_bar->addAction (do_gravity_act);
}
/**
 * reactToGravityToggle
 */
void MainWindow::reactToGravityToggle(bool checked)
 {
    if (checked) 
		emit enableGravity();
	else
		emit disableGravity();
 }

/**
 * zoomViewIn
 */
void MainWindow::zoomViewIn()
{
	view->scaleView (1.5);
}

/**
 * zoomViewOut
 */
void MainWindow::zoomViewOut()
{
	view->scaleView (0.666);
}

/**
 * saveNodeTexts
 */
void MainWindow::saveNodeTexts()
{
    GuiNode * node;
    for ( node = ( GuiNode *)graph->firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		if (node->text_dock != NULL)
		{
			node->saveText();
			node->textChange();
		}
}

/**
 * closeOldDocks
 */
void MainWindow::closeOldDocks()
{
    GuiNode * node;
    for ( node = ( GuiNode *)graph->firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		if (node->text_dock != NULL) 
			node->text_dock->close();
}

/**
 * searchButtonClicked
 */
void MainWindow::searchButtonClicked()
{
	search_result_list->clear();
	result_list.clear();
    GuiNode * node;
    for ( node = ( GuiNode *)graph->firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		if (node->node_text.contains( search_text_edit->text(),Qt::CaseInsensitive))
		{
			search_result_list->addItem(QString( "Node %1").arg( node->userId()));
			result_list.append( node);
		}
	search_result_dock->show();
}

/**
 * nodeClickedFromList
 */
void MainWindow::nodeClickedFromList( QListWidgetItem * item)
{
	int selected_node_id = result_list[search_result_list->currentRow()]->userId();
	doCentreOnNode( selected_node_id);
	addNewTextDock( selected_node_id);
}
