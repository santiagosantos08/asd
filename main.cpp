#include <iostream>
#include <set>
#include <list>
#include<map>
#include<vector>
#include<algorithm>
using namespace std;
//  *-*

/*
 * 0 java
 * 1 prolog
 * 2 ruby
 * 3 c++
 * 4 fortran
 * 5 js
 * 6 python
 * 7 perl
 * 8 smalltalk
 * 9 php
 * 10 typescript
 * 11 pascal
 */
//map<empleados,set de lenguajes que sabe>
int const java=0;
int const prolog=1;
int const ruby=2;
int const cpp=3;
int const fortran=4;
int const javascript=5;
int const python=6;
int const perl=7;
int const smalltalk=8;
int const php=9;
int const typescript=10;
int const pascal=11;

void cargaCandidatos(std::map<int,std::set<int>> & candidatos){ //carga del ejemplo del pdf para verificar con una solucion conocida
    set<int> c1={java,prolog,cpp,fortran};
    set<int> c2={python,perl,smalltalk,php,typescript,pascal};
    set<int> c3={ruby,javascript,smalltalk};
    set<int> c4={fortran,javascript,perl,smalltalk,typescript,pascal};
    set<int> c5={cpp,python,php};
    set<int> c6={prolog,ruby};
    set<int> c7={php,typescript,pascal};
    set<int> c8={prolog,fortran,perl,typescript};

	set<int> c9={0,1,2,3,4,5,6,7,8,9,10,11};

	//candidatos[9]=c9;
    candidatos[1]=c1;
    candidatos[2]=c2;
    candidatos[3]=c3;
    candidatos[4]=c4;
    candidatos[5]=c5;
    candidatos[6]=c6;
    candidatos[7]=c7;
    candidatos[8]=c8;
}

//=============================================
//aprox greedy. punto B
void interseccion(std::set<int> & a, std::set<int> & b, std::set<int> & result){
	auto it1 = a.begin();
	auto it2 = b.begin();

	while(it1 != a.end()){
		while(it2 != b.end()){
			if(*it1 == *it2){
				result.insert(*it1);
			}
			it2++;
		}
		it1++;
	}
}

int buscarMejorCandidato(std::map<int,std::set<int>> & candidatos, std::set<int> & requisitos){

	int mejorParcial = 0;
	int mejorCandidato = -1;
	auto itCandidatos = candidatos.begin();
	set<int> intersec;
	int size = 0;
	bool encontrado = false;

	while(itCandidatos != candidatos.end()){

		interseccion(requisitos,itCandidatos->second,intersec);

		size = intersec.size();

		if(size > mejorParcial){
			mejorCandidato = itCandidatos->first;
			mejorParcial = size;
		}
		size = 0;
		intersec.clear();
		itCandidatos++;
	}

	return mejorCandidato;
}

void eliminarRequisitos(std::map<int,std::set<int>> & candidatos, std::set<int> & requisitos, int empleado){
	set<int> lenguajesNuevoEmpleado = candidatos[empleado];

	auto it = lenguajesNuevoEmpleado.begin();
	while(it != lenguajesNuevoEmpleado.end()){
		requisitos.erase(*it);
		it++;
	}
}

void buscarAsignaciones(std::map<int,std::set<int>> & candidatos, std::set<int> & requisitos, std::set<int> & contratados){

	bool buscando = true;
	int candidato = 0;

	while(buscando){
		candidato = buscarMejorCandidato(candidatos,requisitos);
		if(candidato != -1){
			contratados.insert(candidato);
			eliminarRequisitos(candidatos,requisitos,candidato);
			candidatos.erase(candidato);
		} else {buscando = false;}
	}
}

void printSet(std::set<int> & s){
	auto it = s.begin();
	while(it!=s.end()){
		std::cout<<" - "<<*it<<" "<<std::endl;
		it++;
	}
}

int main (){
    //armar un set con las skill sin cubrir (al principio seran todas)
    //2: elegir el que mas cubre de esas sin cubrir
    //3:asignarlo, rearmar el set de sin curbrir sacandole la intersecc de esas y lo q hace el nuevo asignado
    //4 ->> repetir gg

	//estructuras encesarias
	/*
	 * candidatos con las skill que tienen
	 * los que nya estan contratados
	 * skill que se necesitan
	 */
	std::map<int,std::set<int>> candidatos;
	cargaCandidatos(candidatos);
	std::set<int> requisitos = {java,prolog,ruby,cpp,fortran,javascript,python,perl,smalltalk,php,typescript,pascal};
	std::set<int> contratados;

	buscarAsignaciones(candidatos,requisitos,contratados);
	if(contratados.size() == 0){
		std::cout<<"No hay asignacion que permita cubrir todos los lenguajes"<<std::endl;
	}
	else{
		printSet(contratados);
	}

    return 0;

}
