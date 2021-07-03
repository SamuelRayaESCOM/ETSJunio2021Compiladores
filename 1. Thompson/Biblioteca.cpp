#include<bits/stdc++.h>
#include "Biblioteca.h"
using namespace N;
using namespace std;

int Biblioteca::preced(char caracter){
    if(caracter=='*')
        return 3;
    else if(caracter=='.')
        return 2;
    else if(caracter=='|')
        return 1;
    else
        return 0;
}
 
string Biblioteca::postfija(string exp){	/* Pone la cadena en formato postfijo '*/
    stack<char> p;											//PILA p
    string post;
    for(char i: exp){										//Desde el inicio al final
        if(i!='(' && i!=')' && i!='.' &&i!='*' && i!='|')	//Si es caracter a_zA_Z0_9
        	post+= i;
        else if(i == '(')
        	p.push('(');
        else if(i == ')'){
        	while(p.top()!='(' && !p.empty()){
                post += p.top();							//
                p.pop();
            }
            p.pop();
        }else{												//Cualquier operando
            while(!p.empty() && (preced(i)<=preced(p.top()))){
                post+= p.top();
                p.pop();			//Saca de la pila 
            }
            p.push(i);		//Incluye el caracter actual
        }
    }

    while(!p.empty()) {			//Incluye en la cadena final cada caracter de la pila
        post+= p.top();			
        p.pop();
    }
 	return post;
}


string Biblioteca::Agregando_simbolos(int longt, string ex){
    string S_concatenado;
    for(int i=0; i<longt;i++){
        S_concatenado+=ex[i];
        if(ex[i]==')' || ex[i]=='*'){
            if(i+1<longt){
                if(ex[i+1]!='*' && ex[i+1]!='|' && ex[i+1]!=')'){
                    S_concatenado+='.';
                }
            }
        }
        else{
            if((ex[i]>='0' && ex[i]<='z')){
                if(ex[i+1]!='*' && ex[i+1]!='|' && ex[i+1]!=')' && i+1<longt){
                    S_concatenado+='.';
                }
            }
        }
    }
    return S_concatenado;
}