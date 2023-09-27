#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "../PathFinder/src/cGraph.h" // https://github.com/JamesBremner/PathFinder

int intersectionNumber;
std::vector<int> vCliqueCount;
raven::graph::cGraph g;

void input()
{
    std::cout << "Intersection Number: ";
    std::cin >> intersectionNumber;
    std::cout << "clique counts:\n";
    for (int i = 0; i < intersectionNumber; i++)
    {
        int cc;
        std::cout << "#" << i + 1 << ": ";
        std::cin >> cc;
        vCliqueCount.push_back(cc);
    }
}

void generate()
{
    std::string previs;
    std::vector<std::string> vsclique;
    for (int i = 0; i < intersectionNumber; i++)
    {
        auto is = std::to_string(i);
        auto is0 = is + "_0";
        if (i > 0)
            g.add(is0, previs);
        previs = is0;

        vsclique.clear();
        for (int c1 = 0; c1 < vCliqueCount[i] - 1; c1++)
        {
            vsclique.push_back(is + "_" + std::to_string(c1 + 1));
            g.add(is0, vsclique.back());
        }
        for (int c1 = 0; c1 < vsclique.size(); c1++)
        {

            for (int c2 = c1 + 1; c2 < vsclique.size(); c2++)
                g.add(vsclique[c1], vsclique[c2]);
        }
    }
    if (intersectionNumber > 2)
        g.add("0_0", previs);
}

void extraEdges()
{
    // loop over all cliques
    for (int c1 = 0; c1 < intersectionNumber; c1++)
    {
        auto is = std::to_string(c1) + "_1";

        // loop over other cliques, not already connected to c1
        for (
            int c2 = c1 + 1;
            c2 < intersectionNumber;
            c2++)
        {
            // loop over vertices on other clique, except numbers 0 and 1
            for (int v = 2; v < vCliqueCount[c1]; v++)
            {
                auto sv2 = std::to_string(c2) + "_" + std::to_string(v);
                g.add(is, sv2);
            }
        }
    }
}

void output()
{
    std::ofstream ifs("gengraph.txt");
    if (!ifs.is_open())
        throw std::runtime_error("Cannot open output");

    auto vl = g.edgeList();

    // this outputs vertex names that reveal the clique each vertex belongs to
    // for( auto& l : vl )
    // {
    //     ifs << "l " << g.userName(l.first)
    //          <<" "<<g.userName(l.second)<< "\n";
    // }

    // this outputs obscured graph construction
    std::random_shuffle(vl.begin(), vl.end());
    std::map<int, int> obvertex;
    int kv = 0;
    for (auto &l : vl)
    {
        obvertex.insert(std::make_pair(l.first, kv++));
        obvertex.insert(std::make_pair(l.second, kv++));
        ifs << "l " << std::to_string(obvertex.find(l.first)->second)
            << " " << std::to_string(obvertex.find(l.second)->second) << "\n";
    }
}
void viz()
{
    std::string graphvizgraph = "graph";
    std::string graphvizlink = "--";

    std::ofstream f("graph.dot");
    f << graphvizgraph << " G {\n";

    // loop over vertices
    for (int vi = 0; vi < g.vertexCount(); vi++)
    {
        f << "\"" << g.userName(vi) << "\""
          << " [  penwidth = 3.0 ];\n";
    }

    // loop over edges
    for (int v1 = 0; v1 < g.vertexCount(); v1++)
    {
        for (int v2 : g.adjacentOut(v1))
        {
            // check not reverse link in undirected graph
            if (v2 > v1)
                continue;

            f << "\"" << g.userName(v1) << "\"" << graphvizlink
              << "\"" << g.userName(v2) << "\";\n";
        }
    }
    f << "}\n";
}
void vizObscure()
{
    std::vector<std::string> vobname(g.vertexCount());
    for (int i = 0; i < vobname.size(); i++)
        vobname[i] = std::to_string(i);
    std::random_shuffle(vobname.begin(), vobname.end());

    std::string graphvizgraph = "graph";
    std::string graphvizlink = "--";

    std::ofstream f("graphObscure.dot");
    f << graphvizgraph << " G {\n";

    // loop over vertices
    for (int vi = 0; vi < g.vertexCount(); vi++)
    {
        f << "\"" << vobname[vi] << "\""
          << " [  penwidth = 3.0 ];\n";
    }

    // loop over edges
    for (int v1 = 0; v1 < g.vertexCount(); v1++)
    {
        for (int v2 : g.adjacentOut(v1))
        {
            // check not reverse link in undirected graph
            if (v2 > v1)
                continue;

            f << "\"" << vobname[v1] << "\"" << graphvizlink
              << "\"" << vobname[v2] << "\";\n";
        }
    }
    f << "}\n";
}

main()
{
    input();
    generate();
    extraEdges();
    viz();
    vizObscure();
    output();

    return 0;
}
