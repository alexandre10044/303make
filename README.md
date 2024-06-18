# 303make
 
As you know, when compiling a program, only the files that are dependent on recently modified sourcesare effectively recompiled during a Make function call. In order to do this, this binary parses the Makefilein the current folder so that a dependency graph can be generated. Next, it analyzes this graph in order toexecute the minimum amount of compilations.In this project you must simulate the behavior of Make by reconstructing the dependency graph, and thendisplay all of the commands to be executed after a source file is modified.First, you’ll need to display the adjacency matrix and the dependency strings.


USAGE

./303make makefile [file]

DESCRIPTION

makefile    name of the makefile

file        name of a recently modified file
