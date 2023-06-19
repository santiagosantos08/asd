#include <iostream>
#include <set>
#include<map>
#include<climits>
using namespace std;
//  *-*
//map<empleados,set de lenguajes que sabe>

//======= [LENGUAJES] =======
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

//======= [METRICAS] =======
int metricaBackTracking = 0;
int metricaGreedy = 0;

//======= [FUNCIONES COMUNES] =======

void printSet(std::set<int> & s){
	auto it = s.begin();
	while(it!=s.end()){
		std::cout<<" - "<<*it<<" "<<std::endl;
		it++;
	}
}

void eliminarRequisitos(std::map<int,std::set<int>> & candidatos, std::set<int> & requisitos, int empleado){
	//elimina de los requisitos las habilidades que tiene el nuevo candidato a contratar
	set<int> lenguajesNuevoEmpleado = candidatos[empleado];

	auto it = lenguajesNuevoEmpleado.begin();
	while(it != lenguajesNuevoEmpleado.end()){
		requisitos.erase(*it);
		it++;
	}
}

void cargaCandidatos(std::map<int,std::set<int>> & candidatos){
    set<int> c1={java,prolog,cpp,fortran};
    set<int> c2={python,perl,smalltalk,php,typescript,pascal};
    set<int> c3={ruby,javascript,smalltalk};
    set<int> c4={fortran,javascript,perl,smalltalk,typescript,pascal};
    set<int> c5={cpp,python,php};
    set<int> c6={prolog,ruby};
    set<int> c7={php,typescript,pascal};
    set<int> c8={prolog,fortran,perl,typescript};

	set<int> comodin={0,1,2,3,4,5,6,7,8,9,10,11};


	//si dejamos el comodin en 1 y c1 en 9, greedy da la misma sol que backtracking (ambos contratan solo al comodin)
	//si dejamos el comodin en 9, greedy da un aproximado peor que backtracking (1,2,3 vs comodin)
	//si no podemos al comodin ambos contratan a 1,2,3

	//candidatos[1]=comodin;

    candidatos[1]=c1;
    candidatos[2]=c2;
    candidatos[3]=c3;
    candidatos[4]=c4;
    candidatos[5]=c5;
    candidatos[6]=c6;
    candidatos[7]=c7;
    candidatos[8]=c8;

	//candidatos[9]=comodin;
	//candidatos[9]=c1;
}

//======= [SOLUCION EXTACTA (IMPLEMENTADO EN BASE A BACKTRACKING)] =======

std::set<int> asignacionGlobal;
int cotaGlobal = INT_MAX;

void restauraRequisitos(std::set<int> & requisitos, std::set<int> & lenguajesEmpleado){
	//restaura los cambios hechos en el backtracking luego del llamado recursivo
	auto it = lenguajesEmpleado.begin();

	while(it != lenguajesEmpleado.end()){
		requisitos.insert(*it);
		it++;
	}

}

void buscarAsignacionExacta(std::map<int,std::set<int>> & candidatos, std::set<int> & requisitos, std::set<int> & contratados, int nivel){
	if(requisitos.size() == 0){
		if(nivel < cotaGlobal){ //nivel es la cantidad de contratados hasta ahora
			cotaGlobal = nivel;
			asignacionGlobal = contratados;
		}
	}

	metricaBackTracking++;

	auto it = candidatos.begin();

	while(it != candidatos.end() && nivel < cotaGlobal){

		std::map<int,std::set<int>> copiaCandidatos = candidatos; //se copia para no tener que restaurarlo luego del llamado recursivo
		eliminarRequisitos(candidatos,requisitos,it->first);
		copiaCandidatos.erase(it->first);
		contratados.insert(it->first);

		buscarAsignacionExacta(copiaCandidatos,requisitos,contratados,nivel+1);

		contratados.erase(it->first); //deshacer cambios
		restauraRequisitos(requisitos,it->second);
		it++;
	}
}

void solBacktracking(std::map<int,std::set<int>> candidatos, std::set<int> requisitos){

	std::set<int> contratadosBack;
	buscarAsignacionExacta(candidatos,requisitos,contratadosBack,0);

	if(asignacionGlobal.size() == 0){
		std::cout<<"[Backtracking]: Sin sol. que permita cubrir todos los lenguajes | "<<metricaBackTracking<<" iteraciones "<<std::endl;
	}
	else{
		std::cout<<"[Backtracking]: "<<metricaBackTracking<<" iteraciones "<<std::endl;
		printSet(asignacionGlobal);
	}
}

//======= [SOLUCION APROXIMADA (MEDIANTE ALGORITMO GREEDY)] =======

void interseccion(std::set<int> & a, std::set<int> & b, std::set<int> & result){
	//calcula interseccion entre dos conjuntos de manera simple
	auto it1 = a.begin();
	auto it2 = b.begin();

	while(it1 != a.end()){
		while(it2 != b.end()){
			if(*it1 == *it2){
				metricaGreedy++;
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
		metricaGreedy++;
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

void buscarAsignacionesGreedy(std::map<int,std::set<int>> & candidatos, std::set<int> & requisitos, std::set<int> & contratados){

	bool buscando = true;
	int candidato = 0;

	while(buscando){
		metricaGreedy++;
		candidato = buscarMejorCandidato(candidatos,requisitos);
		if(candidato != -1){
			contratados.insert(candidato);
			eliminarRequisitos(candidatos,requisitos,candidato);
			candidatos.erase(candidato);
		} else {buscando = false;}
	}

	if (requisitos.size() != 0){ //si no asignó a todos los lenguajes al menos un candidato, no debe ser solucion
		contratados.clear();
	}
}

void solGreedy(std::map<int,std::set<int>> candidatos, std::set<int> requisitos){

	std::set<int> contratadosGreedy;
	buscarAsignacionesGreedy(candidatos,requisitos,contratadosGreedy);

	if(contratadosGreedy.size() == 0){
		std::cout<<"[Greedy]: Sin sol. que permita cubrir todos los lenguajes | "<<metricaGreedy<<" iteraciones "<<std::endl;
	}
	else{
		std::cout<<"[Greedy]: "<<metricaGreedy<<" iteraciones "<<std::endl;
		printSet(contratadosGreedy);
	}
}



int main (){

	std::set<int> requisitos = {java,prolog,ruby,cpp,fortran,javascript,python,perl,smalltalk,php,typescript,pascal};

	std::map<int,std::set<int>> candidatos;
	cargaCandidatos(candidatos);

	std::cout<<" ===== Empleados a contratar : ====="<<std::endl;
	solGreedy(candidatos,requisitos);
	solBacktracking(candidatos,requisitos);
	std::cout<<" ===== Greedy es "<<metricaBackTracking/metricaGreedy<<" veces más rápido."<<std::endl;

    return 0;
}
