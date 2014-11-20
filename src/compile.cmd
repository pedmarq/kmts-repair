win_bison grammarFormulas.y 
win_flex lexicoFormulas.lex
g++ -g -o parser Configuracao.cpp Estado.cpp Formula.cpp Leitor.cpp main.cpp ModelChecking.cpp RefineGame.cpp VisitTree.cpp



