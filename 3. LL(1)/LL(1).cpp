#include<bits/stdc++.h>

using namespace std;
//Hizo gramatica de tipo par; first entrega el primer grupo guardado, en este caso los no Terminales, mientras que en el segundo grupo están guardados los Simbolos/Terminales
set<string> noTerminales;
set<string> Terminales;
vector<pair<string,vector<string>>> gramatica;
vector<pair<string,set<string>>> setsPrimeros;
vector<pair<string,set<string>>> setsSiguientes;

void definirTerminales(vector<string> posiblesTerminales){
	for(int i=0; i<posiblesTerminales.size();i++){
		if(noTerminales.count(posiblesTerminales[i])==0){
			Terminales.insert(posiblesTerminales[i]);
		}
	}
}

void definirGramatica(string derivacion){
	string noTer;
	vector<string> simbolos;
	string aux;
	int i=0;
	do{
		noTer+=derivacion[i];
		i++;
	}while(derivacion[i]!='-');			//Agrega antes de la flecha que son no Terminales

	i+=1;		//Simbolo >
	for(int j=i+1;j<derivacion.size();j++){			//Ve el resto de la grámatica
		if(derivacion[j]=='\'')								//Si encuentra un caracter "primo"
			simbolos[simbolos.size()-1]+='\'';				//Concatena '
		else{
			aux=derivacion[j];
			simbolos.push_back(aux);
		}
	}
	gramatica.push_back(make_pair(noTer,simbolos));					//Hace un par entre el no Terminal y los simbolos resultantes
	noTerminales.insert(noTer);									//Agregar el no terminal
}

set<string> calcularPrimeros(pair<string,vector<string>> produccion){
	set<string> respuesta;
	for(int i=0; i<produccion.second.size();i++){					//Los simbolos
		if(noTerminales.count(produccion.second[i])==1 && (respuesta.size()==0 || respuesta.count("e")==1)){		//Si hay un noTerminal en la parte de los simbolos.
			for(int j=0; j<gramatica.size();j++){
				if(gramatica[j].first==produccion.second[i]){			//No terminal en la gramatica == No terminal en los simbolos.
					set<string> aux;									//auxiliar de tipo set
					aux=calcularPrimeros(gramatica[j]);					//Recursividad pero con iniciando con la gramatica donde se encontró el no terminal en los simbolos.
					for(string k:aux)										
						respuesta.insert(k);
				}
			}
		}else if(noTerminales.count(produccion.second[i])==0){				//es un simbolo
			respuesta.insert(produccion.second[i]);							//inserta en respuesta el elemento actual
			break;															//Interrumpe el proceso y se pasa al siguiente caracter.
		}
	}
	return respuesta;
}

set<string> calcularSiguientes(pair<string,vector<string>> produccion,string aux){
	set<string> respuesta;
	for(int i=0; i<produccion.second.size();i++){					//Desde 0 hasta el tamaño de los simbolos de la grámatica
		if(aux==produccion.second[i]){									//Si el no Terminal == simbolo actual de la gramatica de la parte de los simbolos
			if(i==produccion.second.size()-1){								//Si i alcanza el final
				respuesta.insert("$");											//Se inserta $
				for(int j=0; j<gramatica.size();j++){								
					for(int k=0;k<gramatica[j].second.size();k++){						//Desde 0 hasta el tamaño de los simbolos en la gramática j
						if(produccion.first==gramatica[j].second[k] && produccion.first!=gramatica[j].first){			//Si el noTerminal==simbolo en la gramatica y difiere del no terminal en j
							set<string> setAux;
							setAux=calcularSiguientes(gramatica[j],produccion.first);								//Recursividad, con la gramatica actual y el no terminal actual
							for(string m:setAux){
								respuesta.insert(m);												//Inserta en la respuesta la cadena de los siguientes calculados
							}
						}
					}
				}
			}else if(i<produccion.second.size()-1){					//Mientras no alcance el final.
				if(Terminales.count(produccion.second[i+1]))			//Si hay terminales en la parte de los simbolos de la gramatica
					respuesta.insert(produccion.second[i+1]);				//Incluye estos terminales.
				else{
					for(int j=0;j<gramatica.size();j++){
						if(gramatica[j].first==produccion.second[i+1]){				//No Terminal == siguiente
							set<string>setAux;
							setAux=calcularPrimeros(gramatica[j]);					//Calcula el primero de este No terminal y lo incluye para la respuesta final de los siguientes
							for(auto k:setAux)
								respuesta.insert(k);
						}
					}
					if(respuesta.count("e")==1){									//Si hay algún epsilon en la respuesta, lo elimina
						respuesta.erase("e");
						for(int n=0; n<gramatica.size();n++){
							if(produccion.first==gramatica[n].first){				//primero == No terminal
								set<string> setAux;
								setAux=calcularSiguientes(gramatica[n],produccion.first);	//Calcula sus siguientes, mandando la gramatica actual, y los primeros
								for(auto k:setAux)
									respuesta.insert(k);
							}
						}
					}
				}
			}
			break;							//Interrumpe y pasa al siguiente 'siguiente'
		}
	}
	return respuesta;
}

void crearTabla(){
	vector<string> simbolos;
	for(auto i: Terminales){
		if(i!="e")
			simbolos.push_back(i);			//Incluye todos  los elementos excepto el epsilon
	}
	simbolos.push_back("$");					
	vector<pair<string,vector<string>>> filas;							
	for(auto i: noTerminales){
		vector<string> aux;
		for(int j=0;j<simbolos.size();j++)
			aux.push_back("-");								//Por el momento, todas las cadenas incluirán un vacío: -
		filas.push_back(make_pair(i,aux));				//Se harán par las filas y sus datos (-)
	}

	for(int i=0; i<filas.size();i++){						//Todas las filas.
		vector<string> aux;
		for(int j=0;j<setsPrimeros.size();j++){
			if(filas[i].first==setsPrimeros[j].first){			//Si la fila de i == No terminal del primero
				string comparar;
				for(auto r:setsPrimeros[j].second)								
					comparar=r;											//compara tomará los simbolos de los primeros
				if(comparar=="e"){
					for(int m=0;m<setsSiguientes.size();m++){
						if(setsSiguientes[m].first==filas[i].first){			//si el primero de siguiente es igual al no terminal de la fila.
							for(int l=0; l<simbolos.size();l++){
								for(auto k:setsSiguientes[m].second){				//Hasta los siguientes de m
									if(simbolos[l]==k)							//Si el simbolo l == al simbolo actual de primero de j				
										filas[i].second[l]=to_string(j+1);			//sustiruye el valor en filas por el número encontrado en primero de j, +1
								}
							}
						}
					}
				}else{
					for(int l=0; l<simbolos.size();l++){
						for(auto k:setsPrimeros[j].second){						
							if(simbolos[l]==k)							//Si el simbolo l == al simbolo actual de primero de j
								filas[i].second[l]=to_string(j+1);					//sustiruye el valor en filas por el número encontrado en primero de j, +1
						}
					}
				}
			}
		}
	}

	cout<<"   ";
	for(int i=0; i<simbolos.size();i++)							//Imprime los simbolos
		cout<<simbolos[i]<<" ";
	cout<<"\n";

	for(int i=0; i<filas.size();i++){
		cout<<filas[i].first<<"  ";
		for(int j=0;j<filas[i].second.size();j++)				//Imprime el valor guardado en la respectiva fila.
			cout<<filas[i].second[j]<<" ";
		cout<<"\n";
	}
}


int main(){
	int cantidad;
	string derivacion;
	vector<string> derivaciones;
	cout<<"Ingrese el número de derivaciones\n";
	cin>>cantidad;
	for(int i=0; i<cantidad ;i++){
		cin>>derivacion;
		derivaciones.push_back(derivacion);
	}
	for(int i=0;i<derivaciones.size();i++)
		definirGramatica(derivaciones[i]);

	for(int i=0;i<gramatica.size();i++)
		definirTerminales(gramatica[i].second);
	
	for(int i=0;i<gramatica.size();i++){
		set<string> aux;
		aux = calcularPrimeros(gramatica[i]);
		setsPrimeros.push_back(make_pair(gramatica[i].first,aux));							//Par entre los No Terminales y sus respectivos primeros
	}
	
	for(int i=0; i<setsPrimeros.size();i++){
		for(auto j:setsPrimeros[i].second){												//Desde j hasta ver todos los elementos primeros
			if(j=="e"){																	//Si incluye el conjunto vacío
				for(int k=0; k<gramatica.size();k++){										//desde k hasta el tamaño de la derivación
					for(int l=0;l<gramatica[k].second.size();l++){								//desde 0 hasta el tamaño de los terminales en la derivación.
						if(setsPrimeros[i].first==gramatica[k].second[l] && setsPrimeros[i].first!=gramatica[k].first){		//Si los no terminales de i==Terminal de l de la derivación k y es diferente del no Terminal en la derivación K.
							set<string> auxSet;
							auxSet=calcularSiguientes(gramatica[k],setsPrimeros[i].first);				//gramatica de k y no terminales de i
							setsSiguientes.push_back(make_pair(setsPrimeros[i].first,auxSet));			//Hace pares entre los primeros y los siguientes.
						}
					}
				}
			}
		}
	}

	crearTabla();

	return 0;
}