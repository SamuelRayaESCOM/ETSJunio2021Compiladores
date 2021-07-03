#include<bits/stdc++.h>
#include "Biblioteca.cpp"
using namespace std;

int i,j=0;
struct transicion{
	char simbolo;
	int deDonde;
	int aDonde;
};

class AFN{
	public:
	int final;
	vector<int> estados;
	vector<transicion> transiciones;
	
	void numEstados(int num){
		for(int i=0; i<num ;i++)
			estados.push_back(i);
	}
	
	void Transicion(int a, int b, char simbolo){
		transicion t;
		t.deDonde=a;
		t.aDonde=b;
		t.simbolo=simbolo;
		transiciones.push_back(t);
	}

	void mostrar(){
		transicion trans;
		ofstream f("afd.vg");
		f<<"digraph AFN{\nrankdir = LR;"<<endl;
		f<<"node [shape = doublecircle];"<<" "<<final<<";"<<endl;
		f<<"node [shape = point ];"<<" qi"<<endl;
		f<<"node [shape = circle];"<<endl;
		f<<"qi"<<" -> "<<"0"<<endl;
		for(j = 0; j < estados.size(); j++){
			for(i = 0; i < transiciones.size(); i++) {
				trans = transiciones[i];
				if(trans.deDonde == j){
					if(trans.simbolo == 'e'){
						f<<trans.deDonde<<" -> "<<trans.aDonde<<" [label =\"&#949;\"];"<<endl;
					}
					else{
						f<<trans.deDonde<<" -> "<<trans.aDonde<<" [label =\""<<trans.simbolo<<"\"];"<<endl;
					}
					
				}
			}
		}
		f<<"}"<<endl;
		system("dot afd.vg -Tpng > afd.png");
		cout<<"Archivo guardado como afd.vg, quiere ver la imagen de salida? S/N?\n ";
		char res;
		cin>>res;
		if(res=='S' || res=='Y' ||res=='s' || res=='y')
			system("AFD.png");
	}
};

AFN concatenar(AFN oper1, AFN oper2) {
	AFN aux;
	aux.numEstados(oper1.estados.size() + oper2.estados.size()-1);			//Menos el final

	for(i=0; i<oper1.transiciones.size(); i++)
		aux.Transicion(oper1.transiciones[i].deDonde , oper1.transiciones[i].aDonde, oper1.transiciones[i].simbolo);

	for(i=0; i<oper2.transiciones.size(); i++)
		aux.Transicion(oper2.transiciones[i].deDonde + oper1.estados.size()-1, oper2.transiciones[i].aDonde + oper1.estados.size()-1, oper2.transiciones[i].simbolo);

	aux.final=oper2.estados.size()+ oper1.estados.size()-2;
	return aux;
}

AFN Disyuncion(AFN oper1, AFN oper2) {
	AFN aux;
	aux.numEstados(oper1.estados.size() + oper2.estados.size()+2);			//Estados, tanto del lado izquierdo, como el derecho, +2 por el inicio y fin.
	aux.Transicion(0,1,'e');
	for(i=0; i<oper1.transiciones.size();i++)
		aux.Transicion(oper1.transiciones[i].deDonde + 1, oper1.transiciones[i].aDonde + 1, oper1.transiciones[i].simbolo);
	aux.final=oper1.estados.size()+oper2.estados.size()+1;			//Nodo final
	aux.Transicion(oper1.estados.size(),aux.final,'e');
	aux.Transicion(0, oper1.estados.size() + 1,'e');
	for(i=0; i<oper2.transiciones.size();i++)
		aux.Transicion(oper2.transiciones[i].deDonde + oper1.estados.size() + 1, oper2.transiciones[i].aDonde + oper1.estados.size() + 1,oper2.transiciones[i].simbolo);
	aux.Transicion(oper1.estados.size() + oper2.estados.size() , aux.final,'e');
	return aux;
}

AFN estrella(AFN origen) {
	AFN aux;
	aux.numEstados(origen.estados.size()+2);			//+2 -> El de entrada y salida.
	aux.Transicion(0, 1, 'e');
	for(i=0; i<origen.transiciones.size(); i++) 
		aux.Transicion(origen.transiciones[i].deDonde + 1,origen.transiciones[i].aDonde+1, origen.transiciones[i].simbolo);
	int num_estados=origen.estados.size();
	aux.Transicion(num_estados,num_estados+1,'e');		//Penúltimo al final
	aux.Transicion(num_estados,1, 'e');					//Penúltimo al segundo
	aux.Transicion(0,num_estados+1, 'e');					//principio a fin/aceptación
	aux.final = num_estados+1;								//Aceptación; 1 + número de estados totales
	return aux;
}


AFN Proceso(string exp){
	stack<AFN> afnP;
	AFN operando1;
	AFN operando2;
	AFN aux;
	for(char i:exp){
		cout<<i;
		if(i!='*' && i!='|' &&i!='.'){
			AFN *afn = new AFN;
			afn->numEstados(2);
			afn->Transicion(0,1,i);
			afn->final=2;
			afnP.push(*afn);			//Mete en la pila de tipo AFN el AFN creado 
		}else if(i=='*'){
			operando1 = afnP.top();
			afnP.pop();
			afnP.push(estrella(operando1));
		}else if(i=='|'){
			operando2 = afnP.top();
			afnP.pop();
			operando1 = afnP.top();
			afnP.pop();
			afnP.push(Disyuncion(operando1,operando2));
		}else{
			operando2 = afnP.top();
			afnP.pop();							//Quita de la PILA 			
			operando1 = afnP.top();
			afnP.pop();
			afnP.push(concatenar(operando1,operando2));
		}
	}
	return afnP.top();
}

int main(){
	Biblioteca bl;
	string post;
	string expresion;
	cout<<"Ingrese la expresion\n";
	cin>>expresion;
	int longt=expresion.size();
	expresion=bl.Agregando_simbolos(longt,expresion);
	post=bl.postfija(expresion);
	AFN respuesta;
	respuesta=Proceso(post);
	respuesta.mostrar();
	return 0;
}
