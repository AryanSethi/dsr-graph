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

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(TuplePrx tprx) : GenericWorker(tprx) {
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker() {
    std::cout << "Destroying SpecificWorker" << std::endl;
    G.reset();

}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params) 
{

    agent_id = stoi(params["agent_id"].value);
    dsr_output_file = params["dsr_output_file"].value;
    //dsr_input_file = params["dsr_input_file"].value;
    test_output_file = params["test_output_file"].value;
    dsr_input_file = params["dsr_input_file"].value;    
    return true;
}

void SpecificWorker::initialize(int period) 
{
    std::cout << "Initialize worker" << std::endl;

    // create graph
    G = std::make_shared<CRDT::CRDTGraph>(0, agent_name, agent_id, ""); // Init nodes
    G->print();
    // G->start_subscription_thread(true);     // regular subscription to deltas
    // G->start_fullgraph_request_thread();    // for agents that want to request the graph for other agent
    
    // Graph viewer
	graph_viewer = std::make_unique<DSR::GraphViewer>(G);
	mainLayout.addWidget(graph_viewer.get());
	window.setLayout(&mainLayout);
	setCentralWidget(&window);
    
    // Random initialization
    // mt = std::mt19937(rd());
    // unif_float = std::uniform_real_distribution((float)-40.0, (float)40.0);
    // unif_int = std::uniform_int_distribution((int)100, (int)140.0);
    timer.setSingleShot(true);
    timer.start(100);
}

void SpecificWorker::compute()
{
    auto vertex_op = G->get_vertex("world");
    if (vertex_op.has_value()) 
    {
        auto vertex = vertex_op.value();
        vertex->print();
        qDebug() << ":::::::::::::::::::::::::::::::::::";
        auto edge_op = vertex->get_edge(131, "RT");
        if(edge_op.has_value()) {
            auto edge = edge_op.value();
            edge->print();
            
            qDebug() << ":::::::::::::::::::::::::::::::::::";
            auto edges = vertex->get_edges();
            for (auto e: edges)
                e->print();
            
            qDebug() << ":::::::::::::::::::::::::::::::::::";
            auto t = vertex->get_attrib_by_name<std::string>("imType");
            std::cout << t.value_or("error") << std::endl;
            
            qDebug() << ":::::::::::::::::::::::::::::::::::";
            auto v = edge->get_attrib_by_name<QVec>("translation");
            if (v.has_value())
                v->print("QVec");
            
            qDebug() << ":::::::::::::::::::::::::::::::::::";
            auto r = edge->get_attrib_by_name<QMat>("rotation_euler_xyz");
            if (r.has_value())
                r->print("QMat");

            qDebug() << ":::::::::::::::::::::::::::::::::::";  
            Val val;
            val.float_vec(std::vector<float>{2,2,2});
            edge->add_attrib("rotation_euler_xyz", 3, val);
            G->insert_or_assign_node(vertex);
        }
        qDebug() << ":::::::::::::::::::::::::::::::::::";
        auto m = vertex->get_edge_RT(131);
        if (m.has_value())
            m->print("RT"); 

        try
        {
            G->get_attrib_by_name<RTMat>(vertex->get_CRDT_node(), "color");
        }
        catch(const CRDT::DSRException& e)
        {
            std::cerr << e.what() << '\n';
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
   
    qDebug() << ":::::::::::::::::::::::::::::::::::";
    auto innermodel = G->get_inner_api();
    auto r = innermodel->transform("world", "base");
    if(r.has_value())
        r.value().print("r");

    qDebug() << ":::::::::::::::::::::::::::::::::::";
    Edge edge;
    edge.to(131); edge.from(100); edge.type("RT");
    std::map<string, Attrib> attrs;
    Attrib av;
    av.type(3);
    G->add_attrib(attrs, "rotation_euler_xyz", std::vector<float>{2,2,2});
    edge.attrs(attrs);
    G->insert_or_assign_edge(edge);

    //G->insert_or_assign_edge_attr(node, to, "RT", 3, {2,2,2});

  

}


