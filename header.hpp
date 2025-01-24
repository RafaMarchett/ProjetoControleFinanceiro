#pragma once //PARA NÃO TER REDEFINIÇÃO DE ARQUIVOS

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <ctime>

using namespace std;

ostream& boldVerde(ostream& manip) {return manip << "\033[1;32m";}

ostream& boldVermelho(ostream& manip){return manip << "\033[1;31m";}

ostream& bold(ostream& manip) {return manip << "\033[1m";}

ostream& boldBranco(ostream& manip) {return manip << "\033[1;37m";}

ostream& noBold(ostream& manip) {return manip << "\033[0m";}
