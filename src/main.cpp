/*
** EPITECH PROJECT, 2019
** Work
** File description:
** main
*/

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <list>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

#include "entry.hpp"

std::ifstream makefileStream;
std::ifstream fileStream;
std::vector<std::string> data;
std::vector<entry*> entries;
std::vector<entry*> tmp;
std::vector<std::vector<std::string>> links;
std::vector<std::string> names;

bool vectorContains(std::vector<std::string> val, std::string v)
{
    try {
        for (int i = 0; i < val.size(); i++)
            if (!val[i].compare(v) && val[i].size() == v.size())
                return (true);
    } catch (...) { return (false); }
    return (false);
}

bool contains(std::string val, char c)
{
    for (int i = 0; i < val.size(); i++)
        if (val[i] == c)
            return (true);
    return (false);
}

bool sorter(const std::vector<std::string>& left, const std::vector<std::string>& right)
{
    try {
        for (int i = 0; i < left.size() && i < right.size(); i++) {
            if (left[i] > right[i])
                return (true);
            else if (left[i] < right[i])
                return (false);
        }
    } catch (...) { return (false); }
}

entry *get_entry_parent(std::string child)
{
    try {
        for(int i = 0; i < entries.size(); i++)
            for (int x = 0; x < entries[i]->childrens.size(); x++)
                if (!entries[i]->childrens[x].compare(child) && entries[i]->childrens[x].size() == child.size())
                        return (entries[i]);
    } catch (...) { return (NULL); }
    return (NULL);
}

std::vector<entry *>get_master_parents(std::string child)
{
    std::vector<entry *> n;
    try {
        for(int i = 0; i < entries.size(); i++) {
            if (entries[i]->type != Master)
                continue;
            if (!entries[i]->parent.compare(child) && entries[i]->parent.size() == child.size())
                n.push_back(entries[i]);
        }
    } catch (...) { return (n); }
    return (n);
}

bool link()
{
    try {
        while (tmp.size()) {
            entry *obj = tmp.back();
            entry *parent;
            tmp.pop_back();
            if (obj->type == Compile || !contains(obj->parent, '.'))
                return (link());
            if (!obj->childrens.size()) {
                //std::cout << "No childrens." << std::endl;
                return (false);
            }
            parent = get_entry_parent(obj->parent);
            if (parent == NULL) {
                //std::cout << "Can't found parent." << std::endl;
                return (false);
            }
            std::vector<std::string> val;
            val.push_back(obj->childrens[0]);
            val.push_back(obj->parent);
            val.push_back(parent->parent);
            links.push_back(val);
            std::vector<std::string> small;
            small.push_back(obj->parent);
            small.push_back(parent->parent);
            links.push_back(small);
            if (obj->childrens.size() < 2)
                return (false);
            std::vector<std::string> temp;
            temp.push_back(obj->childrens[1]);
            temp.push_back(obj->parent);
            temp.push_back(parent->parent);
            links.push_back(temp);
        }
    } catch (...) { return (false); }
    return (true);
}

bool parse(std::vector<std::string> dat, std::string _line)
{
    try {
        entry *obj = new entry();
        if (!dat.size())
            return (false);
        if (dat[0][dat[0].length() - 1] == ':') {
            obj->type = Master;
            dat[0].erase(dat[0].length() - 1);
            obj->parent = dat[0];
            if (!vectorContains(names, obj->parent))
                names.push_back(obj->parent);
            for (int i = 1; i < dat.size(); i++) {
                obj->childrens.push_back(dat[i]);
                if (!vectorContains(names, dat[i]))
                    names.push_back(dat[i]);
            }
        } else if (dat[0] == "cc") {
            obj->compilo = dat[1];
            obj->type = Compile;
            for (int i = 2; i < dat.size(); i++) {
                obj->childrens.push_back(dat[i]);
                if (!vectorContains(names, dat[i]))
                    names.push_back(dat[i]);
            }
            if (entries.size() && entries.back()->type == Master)
                entries.back()->compiloEntry = obj;
        } else {
            //std::cerr << "Error while parsing" << std::endl;
            return (false);
        }
        obj->line = _line;
        tmp.push_back(obj);
        entries.push_back(obj);
    } catch (...) { return (false); }
    return (true);
}

bool isDependence(std::string name1, std::string name2)
{
    try {
        for (int i = 0; i < links.size(); i++) {
            if (links[i].size() < 2)
                continue;
            if (!links[i][0].compare(name1) && links[i][0].size() == name1.size() &&
                !links[i][1].compare(name2) && links[i][1].size() == name2.size()) {
                return (true);
            } 
        }
        return (false);
    } catch (...) { return (false); }
}

bool matrix()
{
    if (!names.size())
        return (false);
    try {
        std::sort(names.begin(), names.end(), std::less<std::string>());
        for (int i = 0; i < names.size(); i++) {
            for (int y = 0; y < names.size(); y++) {
                if (y == 0)
                    std::cout << "[";
                if (i == y)
                    std::cout << "0";
                else if (isDependence(names[i], names[y]))
                    std::cout << "1";
                else
                    std::cout << "0";
                if (y + 1 < names.size())
                    std::cout << " ";
                else
                    std::cout << "]" << std::endl;
            }
        }
    } catch (...) { return (false); }
    return (true);
}

int main(int ac, char **av)
{
    std::string line;
    // Help
    if (ac >= 2 && (!strcmp(av[1], "-h") || !strcmp(av[1], "-help"))) {
        std::cout << "USAGE" << std::endl;
        std::cout << "\t" << av[0] << " makefile [file]" << std::endl;
        std::cout << "DESCRIPTION" << std::endl;
        std::cout << "\tmakefile\tname of the makefile" << std::endl;
        std::cout << "\tfile\t\tname of a recently modified file" << std::endl;
        return (0);
    }
    // ErrorHandling
    if (ac < 2 || (ac != 2 && ac != 3)) {
        std::cerr << "Bad usage." << std::endl;
        return (84);
    }
    try {
        makefileStream.open(av[1]);
        if (makefileStream.fail()) {
            std::cerr << "Bad input." << std::endl;
            return (84);
        }
        // Parsing
        while (std::getline(makefileStream, line))
        {
            std::string d(line.c_str());
            data.push_back(d);
            if (line.find(" ") != std::string::npos) {
                std::string cpy = std::string(line.c_str());
                std::vector<std::string> splitedData;
                size_t pos = 0;
                std::string token;
                while ((pos = line.find(" ")) != std::string::npos) {
                    token = line.substr(0, pos);
                    if (token.size())
                        splitedData.push_back(token);
                    line.erase(0, pos + 1);
                }
                if (line.length() > 1 && line.size()) {
                    splitedData.push_back(line);
                }
                if (!parse(splitedData, cpy))
                    return (84);
            }
        }
    } catch (...) { return (84); }
    // ErrorHandling
    if (!entries.size()) {
        //std::cerr << "Can't split data." << std::endl;
        return (84);
    }
    // Process
    if (ac == 2) {
        try {
            if (!link() || !matrix())
                return (84);
            std::sort(links.begin(), links.end(), sorter);
            std::cout << std::endl;
            for (int i = links.size() - 1; i >= 0; i--) {
                for (int y = 0; y < links[i].size(); y++) {
                    std::cout << links[i][y];
                    if (y + 1 < links[i].size())
                        std::cout << " -> ";
                }
                std::cout << std::endl;
            }
        } catch (...) { return (84); }
    } else if (ac == 3) {
        try {
            for (int i = 0; i < strlen(av[2]); i++) {
                if (!(av[2][i] >= 'a' && av[2][i] <= 'z') &&
                !(av[2][i] >= 'A' && av[2][i] <= 'Z') &&
                (av[2][i] != '.')) {
                    std::cout << std::endl;
                    return (84);
                }
            }
            if (line.find(".") == std::string::npos || line.size() <= 1 || !link() || get_entry_parent(av[2]) == NULL) {
                std::cout << std::endl;
                return (84);
            }
            std::vector<std::string> val;
            for (int i = links.size() - 1; i >= 0; i--) {
                if (!links[i][0].compare(av[2]) && links[i][0].size() == std::string(av[2]).size()) {
                    for (int y = 1; y < links[i].size(); y++) {
                        std::vector<entry *> nt = get_master_parents(links[i][y]);
                        if (!nt.size()) {
                            //std::cout << "No found." << std::endl;
                            return (84);
                        }
                        for (int u = 0; u < nt.size(); u++)
                            if (!vectorContains(val, nt[u]->compiloEntry->line))
                                val.push_back(nt[u]->compiloEntry->line);
                    }
                }
            }
            std::sort(val.begin(), val.end(), std::greater<std::string>());
            if (!val.size()) {
                std::cout << std::endl;
            } else
                for (int i = val.size() - 1; i >= 0; i--)
                    std::cout << val[i] << std::endl;
        } catch (...) { return (84); }
    }
    return (0);
}