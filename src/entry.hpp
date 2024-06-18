/*
** EPITECH PROJECT, 2019
** entry.hpp
** File description:
** entry
*/

#ifndef ENTRY_HPP_
    #define ENTRY_HPP_

    #include <string>
    #include <vector>

    enum entryType {
        None,
        Master,
        Compile
    };

    class entry {
        public:
            entry();
            ~entry();

            enum entryType type;
            std::string compilo;
            std::string parent;
            std::string line;
            entry *compiloEntry;
            std::vector<std::string> childrens;
    };


#endif /* !ENTRY_HPP_ */