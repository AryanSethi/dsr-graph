/*
 *    Copyright (C)2018 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "specificworker.h"
#include <QDesktopWidget>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QXmlDefaultHandler>
#include <QGLViewer/qglviewer.h>
#include <QGraphicsEllipseItem>
#include <cppitertools/range.hpp>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>


/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{

}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	std::cout << "Destroying SpecificWorker" << std::endl;
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	//THE FOLLOWING IS JUST AN EXAMPLE
	//To use innerModelPath parameter you should uncomment specificmonitor.cpp readConfig method content
	try
	{
		RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
		std::string innermodel_path = par.value;
		innerModel = new InnerModel(innermodel_path);
	}
	catch(std::exception e) { qFatal("Error reading config params"); }

	return true;
}



// void SpecificWorker::initializeXML(std::string file_name)
// {
// 	//version = 0;
// 	std::cout << __FUNCTION__ << "Reading xml file: " << file_name << std::endl;

// 	// Open file and make initial checks
// 	xmlDocPtr doc;
// 	if ((doc = xmlParseFile(file_name.c_str())) == NULL)
// 	{
// 		fprintf(stderr,"Can't read XML file - probably a syntax error. \n");
// 		exit(1);
// 	}
// 	xmlNodePtr root;
// 	if ((root = xmlDocGetRootElement(doc)) == NULL)
// 	{
// 		fprintf(stderr,"Can't read XML file - empty document\n");
// 		xmlFreeDoc(doc);
// 		exit(1);
// 	}
// 	if (xmlStrcmp(root->name, (const xmlChar *) "AGMModel"))
// 	{
// 		fprintf(stderr,"Can't read XML file - root node != AGMModel");
// 		xmlFreeDoc(doc);
// 		exit(1);
// 	}

// 	// Read symbols (just symbols, then links in other loop)
// 	for (xmlNodePtr cur=root->xmlChildrenNode; cur!=NULL; cur=cur->next)
// 	{
// 		if (xmlStrcmp(cur->name, (const xmlChar *)"symbol") == 0)
// 		{
// 			xmlChar *stype = xmlGetProp(cur, (const xmlChar *)"type");
// 			xmlChar *sid = xmlGetProp(cur, (const xmlChar *)"id");
			
// 			//AGMModelSymbol::SPtr s = newSymbol(atoi((char *)sid), (char *)stype);
// 			IDType node_id = std::atoi((char *)sid);
// 			std::string node_type((char *)stype);
// 			graph->addNode(node_id, node_type);
// 			graph->addNodeAttribs(node_id, DSR::Attribs{ 
// 								//std::pair("name", node_type), 
// 								//std::pair("type", node_type),
// 								std::pair("level", std::int32_t(0)),
// 								std::pair("parent", IDType(0))
// 				});
	
// 			// Draw attributes come now
// 			DSR::DrawAttribs atts;
// 			std::string qname = (char *)stype;
// 			std::string full_name = std::string((char *)stype) + " [" + std::string((char *)sid) + "]";
// 			auto rd = QVec::uniformVector(2,-200,200);
// 			atts.insert(std::pair("name", full_name));
// 			atts.insert(std::pair("posx", rd[0]));
// 			atts.insert(std::pair("posy", rd[1]));
			
// 			// color selection
// 			std::string color = "coral";
// 			if(qname == "world") color = "SeaGreen";
// 			else if(qname == "transform") color = "SteelBlue";
// 			else if(qname == "plane") color = "Khaki";
// 			else if(qname == "differentialrobot") color = "GoldenRod";
// 			else if(qname == "laser") color = "GreenYellow";
// 			else if(qname == "mesh") color = "LightBlue";
// 			else if(qname == "imu") color = "LightSalmon";
			
			
			
			
// 			atts.insert(std::pair("color", color));
// 			graph->addNodeDrawAttribs(node_id, atts);
// 			std::cout << node_id << " " <<  std::string((char *)stype) << std::endl;
			
// 			xmlFree(sid);
// 			xmlFree(stype);

// 			for (xmlNodePtr cur2=cur->xmlChildrenNode; cur2!=NULL; cur2=cur2->next)
// 			{
// 				if (xmlStrcmp(cur2->name, (const xmlChar *)"attribute") == 0)
// 				{
// 					xmlChar *attr_key   = xmlGetProp(cur2, (const xmlChar *)"key");
// 					xmlChar *attr_value = xmlGetProp(cur2, (const xmlChar *)"value");
					
// 					//s->setAttribute(std::string((char *)attr_key), std::string((char *)attr_value));
// 					graph->addNodeAttribs(node_id, DSR::Attribs{ std::pair(std::string((char *)attr_key), std::string((char *)attr_value))});
					
// 					xmlFree(attr_key);
// 					xmlFree(attr_value);
// 				}
// 				else if (xmlStrcmp(cur2->name, (const xmlChar *)"comment") == 0) { }           // coments are always ignored
// 				else if (xmlStrcmp(cur2->name, (const xmlChar *)"text") == 0) { }     // we'll ignore 'text'
// 				else { printf("unexpected tag inside symbol: %s\n", cur2->name); exit(-1); } // unexpected tags make the program exit
// 			}
// 		}
// 		else if (xmlStrcmp(cur->name, (const xmlChar *)"link") == 0) { }     // we'll ignore links in this first loop
// 		else if (xmlStrcmp(cur->name, (const xmlChar *)"text") == 0) { }     // we'll ignore 'text'
// 		else if (xmlStrcmp(cur->name, (const xmlChar *)"comment") == 0) { }  // coments are always ignored
// 		else { printf("unexpected tag #1: %s\n", cur->name); exit(-1); }      // unexpected tags make the program exit
// 	}

// 	// Read links
// 	for (xmlNodePtr cur=root->xmlChildrenNode; cur!=NULL; cur=cur->next)
// 	{
// 		if (xmlStrcmp(cur->name, (const xmlChar *)"link") == 0)
// 		{
// 			xmlChar *srcn = xmlGetProp(cur, (const xmlChar *)"src");
// 			if (srcn == NULL) { printf("Link %s lacks of attribute 'src'.\n", (char *)cur->name); exit(-1); }
// 			IDType a = atoi((char *)srcn);
// 			xmlFree(srcn);

// 			xmlChar *dstn = xmlGetProp(cur, (const xmlChar *)"dst");
// 			if (dstn == NULL) { printf("Link %s lacks of attribute 'dst'.\n", (char *)cur->name); exit(-1); }
// 			IDType b = atoi((char *)dstn);
// 			xmlFree(dstn);

// 			xmlChar *label = xmlGetProp(cur, (const xmlChar *)"label");
// 			if (label == NULL) { printf("Link %s lacks of attribute 'label'.\n", (char *)cur->name); exit(-1); }
// 			std::string edgeName((char *)label);
// 			xmlFree(label);

// 			std::map<std::string, std::string> attrs;
// 			for (xmlNodePtr cur2=cur->xmlChildrenNode; cur2!=NULL; cur2=cur2->next)
// 			{
// 				if (xmlStrcmp(cur2->name, (const xmlChar *)"linkAttribute") == 0)
// 				{
// 					xmlChar *attr_key   = xmlGetProp(cur2, (const xmlChar *)"key");
// 					xmlChar *attr_value = xmlGetProp(cur2, (const xmlChar *)"value");
// 					attrs[std::string((char *)attr_key)] = std::string((char *)attr_value);
// 					xmlFree(attr_key);
// 					xmlFree(attr_value);
// 				}
// 				else if (xmlStrcmp(cur2->name, (const xmlChar *)"comment") == 0) { }           // coments are always ignored
// 				else if (xmlStrcmp(cur2->name, (const xmlChar *)"text") == 0) { }     // we'll ignore 'text'
// 				else { printf("unexpected tag inside symbol: %s ==> %s\n", cur2->name,xmlGetProp(cur2, (const xmlChar *)"id") ); exit(-1); } // unexpected tags make the program exit
// 			}
			
// 			graph->addEdge(a, b, edgeName);
//  			//graph->addEdgeAttribs(a, b, DSR::Attribs{std::pair("name", edgeName)});
// 			graph->addEdgeDrawAttribs(a, b, DSR::DrawAttribs{std::pair("name", edgeName)});
//  			DSR::Attribs edge_attribs;
// 			if( edgeName == "RT")   //add level to node b as a.level +1, and add parent to node b as a
// 			{ 	
// 				graph->addNodeAttribs(b, DSR::Attribs{ std::pair("level", graph->getNodeLevel(a)+1), std::pair("parent", a)});
// 				RMat::RTMat rt;
// 				float x,y,z;
// 				for(auto &[k,v] : attrs)
// 				{
// 					if(k=="tx")	( x = std::stof(v) );
// 					if(k=="ty")	( y = std::stof(v) );
// 					if(k=="tz")	( z = std::stof(v) );
// 					rt.setTr( x, y, z);
// 					if(k=="rx")	rt.setRX(std::stof(v));
// 					if(k=="ry")	rt.setRY(std::stof(v));
// 					if(k=="rz")	rt.setRZ(std::stof(v));
//  					graph->addEdgeAttribs(a, b, DSR::Attribs{std::pair("RT", rt)});
// 				}
// 			}
// 			else
// 			{
// 				graph->addNodeAttribs(b, DSR::Attribs{ std::pair("parent", 0)});
// 				for(auto &r : attrs)
//  					edge_attribs.insert(r);
// 			}

// 			std::cout << __FUNCTION__ << "edge  from " << a << " " <<  edgeName  << std::endl;		
//  			graph->addEdgeAttribs(a, b, edge_attribs);
			
// 	//	  graph->addEdgeByIdentifiers(a, b, edgeName, attrs);
// 		}
// 		else if (xmlStrcmp(cur->name, (const xmlChar *)"symbol") == 0) { }   // symbols are now ignored
// 		else if (xmlStrcmp(cur->name, (const xmlChar *)"text") == 0) { }     // we'll ignore 'text'
// 		else if (xmlStrcmp(cur->name, (const xmlChar *)"comment") == 0) { }  // comments are always ignored
// 		else { printf("unexpected tag #2: %s\n", cur->name); exit(-1); }      // unexpected tags make the program exit
// 	}
	
// }

void SpecificWorker::initialize(int period)
{
	graph = std::make_shared<DSR::Graph>();
	graph->readFromFile("/home/robocomp/robocomp/files/innermodel/simpleworld-hybrid.xml");
	graph->print();
	
	std::cout << __FILE__ << __FUNCTION__ << " -- Initializing graphic graph" << std::endl;
	graph_viewer.setWidget(this);
	connect(this, SIGNAL(addNodeSIGNAL(std::int32_t, const std::string&, const std::string&, float , float , const std::string&)), 
					&graph_viewer, SLOT(addNodeSLOT(std::int32_t, const std::string&, const std::string&, float , float , const std::string&)));
	connect(this, SIGNAL(addEdgeSIGNAL(std::int32_t, std::int32_t, const std::string&)), 
					&graph_viewer, SLOT(addEdgeSLOT(std::int32_t, std::int32_t, const std::string&)));
	connect(&graph_viewer, SIGNAL(saveGraphSIGNAL()), this, SLOT(saveGraphSLOT()));
					
	drawGraph();
	

	std::cout << __FILE__ << __FUNCTION__ << " -- Graph set OK" << std::endl;
	//graph_viewer.show();	
	std::cout << __FILE__ << __FUNCTION__ << " -- Graph shown OK" << std::endl;

	std::cout << __FILE__ << __FUNCTION__ << " -- Iniializing  innerAPI" << std::endl;
	innerapi.setGraph(graph);
	
	//std::cout << __FILE__ << __FUNCTION__ << " -- TEST treewalk" << std::endl;
	//innerapi.innerModelTreeWalk("world");

	std::cout << __FILE__ << __FUNCTION__ << " -- TESTS transform" << std::endl;
	auto r = innerapi.transform("base", QVec::zeros(3), "laser");
	r.print("Target coordinates from 0, 0, 0");
	
	std::cout << __FILE__ << __FUNCTION__ << " -- TESTS updatetransformvalues " << std::endl;
	innerapi.updateTransformValues("base", 20, 30, 40, 50, 60, 70);

	this->Period = 100;
	timer.start(Period);  
	
	std::cout<< __FILE__ << __FUNCTION__ << ":  Initialize OK" << std::endl;
}

void SpecificWorker::compute()
{
	// try
	// {
	// 	auto lData = laser_proxy->getLaserData();
	// 	RoboCompGenericBase::TBaseState bState;
	// 	differentialrobot_proxy->getBaseState(bState);
	// 	innerapi.updateTransformValues("base", bState.x, 0, bState.z, 0, bState.alpha, 0);
	// 	auto r = innerapi.transform("world", "base");
	// 	r.print("transform");
	// 	std::cout << bState.x << " " << bState.z << std::endl;
		
	// }
	// catch(const Ice::Exception &e)
	// {
	// 	std::cout << "Error reading from Laser" << e << std::endl;
	// }
}

void SpecificWorker::drawGraph()
{
	std::cout << __FUNCTION__ << "-- Entering drawGraph" << std::endl;
	for(const auto &par : *graph)
	{
		const auto &node_id = par.first;
		float node_posx = graph->getNodeDrawAttribByName<float>(node_id, "posx");
		float node_posy = graph->getNodeDrawAttribByName<float>(node_id, "posy");
		std::string color_name = graph->getNodeDrawAttribByName<std::string>(node_id, "color");
		std::string qname = graph->getNodeDrawAttribByName<std::string>(node_id, "name");
		std::string type = graph->getNodeType(node_id);
		emit addNodeSIGNAL(node_id, qname, type, node_posx, node_posy, color_name);
	}		

	// add edges after all nodes have been created
	for(const auto &par : *graph)
	{
		auto &node_fanout = graph->fanout(par.first);
		for( auto &[node_adj, edge_atts] : node_fanout)
		{
			auto edge_tag = graph->attr<std::string>(edge_atts.draw_attrs.at("name"));
			emit addEdgeSIGNAL(par.first, node_adj, edge_tag);
		}
	}
}
