#include "iostream"
#include "vector"
#include "string"
#include "operaciones.h"
#include "stdlib.h"
#include "time.h"
#include "chrono"

using namespace std::chrono;

class Nodo {
	
    public:
        
        float m_valor;
        std::vector<int> m_hijos;        
        std::string m_funcion, m_tipo;
        int m_num_hijos, m_nivel, m_feature, m_max_depth;

        Nodo(int nivel, int max_depth) 
        {

            m_nivel = nivel;
            m_max_depth = max_depth;

            differentiation();
            calcular_hijos();
        }

        int get_nivel_nodo() 
        {
                return m_nivel;
        }

        void differentiation()
        {
			
            if(m_nivel < m_max_depth){
				
                std::vector<std::string> T = {"f1","f1","f1","f1","i1","i2"};
                std::vector<std::string> V = {"+","-","*","sin","cos"};
                
                /*srand(time(NULL));*/
                m_tipo = T[rand() % 6];
                
                if(m_tipo == "f1")
					m_funcion = V[rand() % 5];
                else if(m_tipo == "i1")
                    m_feature = rand() % 100;
                else if(m_tipo == "i2")
                    /*use la formula para hacer numeros aleatorios entre M y N: M+(rand()/(RAND_MAX/N-M))*/                   
                    m_valor = -1.0 + (rand()/(RAND_MAX/2.1));
            }
            
            else{
				 
			    std::vector<std::string> T = {"i1","i2"};
                
                //srand(time(NULL));
                m_tipo = T[rand() % 2];
                
                if(m_tipo == "i1")
					m_feature = rand() % 100;                    
                else if(m_tipo == "i2")
                    m_valor = -1.0 + (rand()/(RAND_MAX/2.1));
            }
        }
        
        void calcular_hijos()
        {

            if (m_tipo == "f1") 
            {
                if(m_funcion == "+" || m_funcion == "-" || m_funcion == "*")
                    m_num_hijos = 2;
                else
                    m_num_hijos = 1;
            }
            
            else if(m_tipo == "i1" || m_tipo == "i2")
                m_num_hijos = 0;
        }
};

class Tree {
	
    public:
    
        int m_tree_max_depth;   
        std::vector<Nodo> m_nodos;
        

        Tree(int tree_max_depth)
        {
            m_tree_max_depth = tree_max_depth;
        }

        void agregar_nodo(int nivel, int max_depth){
            Nodo mi_nodo = Nodo(nivel = nivel, max_depth = max_depth);
            
            m_nodos.push_back(mi_nodo);
        }

        void get_niveles_todos()
        {
            for(int i = 0; i < m_nodos.size(); i++)
                std::cout << "Nodo: " << i << " Nivel: " << m_nodos[i].get_nivel_nodo() << "\n";

        }
        
        void print_arbol() {
            for(int i = 0; i < m_nodos.size(); i++){
                std::cout << "Informacion del nodo " << i << ": " << "\n";

                    std::cout << "Funcion: " << m_nodos[i].m_funcion << "\n";
                    std::cout << "Tipo: " << m_nodos[i].m_tipo << "\n";
                    std::cout << "Valor: " << m_nodos[i].m_valor << "\n";
                    if(m_nodos[i].m_hijos.size() != 0)
                        std::cout << "hijos: " << m_nodos[i].m_hijos[0] << " " << m_nodos[i].m_hijos[1] << "\n";
                    std::cout << "numero de hijos: " << m_nodos[i].m_num_hijos << "\n";
                    std::cout << "nivel: " << m_nodos[i].m_nivel << "\n";
                    std::cout << "feature: " << m_nodos[i].m_feature << "\n";
                    std::cout << "max depth: " << m_nodos[i].m_max_depth << "\n";
                    std::cout << "\n";

            }
        }
};

std::vector<int> spring(Tree& tree, int nodo)
{    
    if(tree.m_nodos[nodo].m_num_hijos)
		for (int i = 0; i < tree.m_nodos[nodo].m_num_hijos; i++)
		{
			tree.agregar_nodo(tree.m_nodos[nodo].m_nivel + 1, tree.m_nodos[nodo].m_max_depth);
			tree.m_nodos[nodo].m_hijos.push_back(tree.m_nodos.size() - 1);
		}
 
    return tree.m_nodos[nodo].m_hijos;
}

void grow_nodes(Tree& tree, std::vector<int> nodes_to_grow)
{
    
    if(nodes_to_grow.size())        
        for(int i = 0; i < nodes_to_grow.size(); i++)
        {   
            std::vector<int> children = spring(tree, nodes_to_grow[i]);//  hay que revisar aqui 3/4/19
            
            //tree.m_nodos[children[i]].m_hijos = children;
            
            grow_nodes(tree, children);
        }   
}

void grow_random_tree(Tree& tree, int max_depth) 
{
	// Declaro la profundidad maxima permitida
    tree.m_tree_max_depth = max_depth;
    // Agrego la raiz
    tree.agregar_nodo(0, max_depth);
    // Crezco los hijos de la raiz
    std::vector<int> children = spring(tree, 0);
    // Crezco los hijos de los hijos de la raiz y asi sucesivamente recursivamente
    grow_nodes(tree, children);
}

void update_tree_depth(Tree& tree, Nodo& nodo, int new_depth)
{
    nodo.m_nivel = new_depth;
    for(int i = 0; i < nodo.m_hijos.size(); i++)
        update_tree_depth(tree, tree.m_nodos[nodo.m_hijos[i]],new_depth + 1);
}

float nodo_evaluate(Tree& tree, Nodo& nodo, std::vector<float>& features)
{
    std::vector<float> params;
    if(nodo.m_num_hijos == 0){
        if(nodo.m_tipo == "i1"){
            return features[nodo.m_feature];
        }
        else if(nodo.m_tipo == "i2"){
            return nodo.m_valor;
        }
    }
    else{
        for(int i = 0; i < nodo.m_hijos.size(); i++)
            params.push_back(nodo_evaluate(tree, tree.m_nodos[nodo.m_hijos[i]], features));
         return operacion(nodo.m_funcion, params);
    }
}

int main() {
    
    srand(time(NULL));
    std::vector<Tree> Arboles;                
    int acc;

    /*auto start = high_resolution_clock::now();*/
    //Tree mi_arbol(10);
    for(int i = 0; i < 10; i++)
    {
        Tree mi_arbol(10);
        grow_random_tree(mi_arbol, 10);
        Arboles.push_back(mi_arbol);  
        acc += Arboles[i].m_nodos.size();
        /*mi_arbol.m_nodos.clear();*/
    }
    
    /*auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << "Se hicieron " << Arboles.size() << " arboles " << " en " << duration.count() << " segundos" << " y contiene " << acc << " nodos.\n ";*/
    
    
    /*std::cout << "Información del arbol 0 en la lista: \n";
    Arboles[0].get_niveles_todos();
    Arboles[0].print_arbol();
    std::cout << "Información del arbol 1 en la lista: \n";
    Arboles[1].get_niveles_todos();
    Arboles[1].print_arbol();
    
    std::cout << "Información del arbol 2 en la lista: \n";
    Arboles[2].get_niveles_todos();
    Arboles[2].print_arbol();
    std::cout << "Información del arbol 3 en la lista: \n";
    Arboles[3].get_niveles_todos();
    Arboles[3].print_arbol();
*/
 
    //update_tree_depth(mi_arbol,mi_arbol.m_nodos[0],0);
    /*std::cout << "\n";

    
    std::vector<float> features = {1,2};
    
    
    std::cout << "Se va a evaluar el vector: " << "(" << features[0] << "," << features[1] << ")\n";
    std::cout << nodo_evaluate(mi_arbol, mi_arbol.m_nodos[0],features) << "\n";*/

    std::vector<std::vector<float>> x_train;
    std::vector<float> y_train, x;

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 100; j++){
            float xi = drand48()*(-1) + 1;
            int p = rand() % 2;
            if(p == 0)
                xi = xi*(-1);
            else;
            x.push_back(xi);
        }
        x_train.push_back(x);
        x.clear();
    }    
    
    
    float t;
    /*auto start = high_resolution_clock::now();*/
    for(int j = 0; j < Arboles.size(); j++){
        t=0;
        for(int i = 0; i < x_train.size(); i++){
            t += nodo_evaluate(Arboles[j],Arboles[j].m_nodos[0], x_train[i]);
            
            }
        t = t/100;

        if(j%1000 == 0)
            std::cout << j << "\n";

        //std::cout << t << "\n";
    }
    /*auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << "Tiempo: " << duration.count() << "\n"; 
    Arboles[0].print_arbol();
    std::cout << "Nodos: " << acc << "\n";*/
  
    
    /*for(int i = 0; i < x_train[99].size();i++)
        std::cout << "el elemento " << i << " de x_train en la posicion 99 es " <<  x_train[99][i] << "\n";

    std::cout << "tamaño de x_train es: " << x_train.size() << "\n";*/

    float acc2;
   
    auto start = high_resolution_clock::now();
    for(int i = 0; i < Arboles.size(); i++){
        for(int j = 0; j < x_train.size(); j++){
            //std::cout << nodo_evaluate(Arboles[i],Arboles[i].m_nodos[0], x_train[j]) << "\n";
            acc2 += nodo_evaluate(Arboles[i],Arboles[i].m_nodos[0], x_train[j]);
        }
        acc2 = acc2/100;
        //y_train[i] = acc2;
        y_train.push_back(acc2);
        acc2 = 0;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    std::cout << "Se hicieron " << Arboles.size() << " arboles " << " en " << duration.count() << " segundos" << " y contiene " << acc << " nodos.\n ";
    
    getchar();

    return 0;
}
