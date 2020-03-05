#include "iostream"
#include "vector"
#include "chrono"
#include "random"
#include "string.h"

class Equipo{
public:
    float m_promedio_goles;
    int m_puntos;
    int m_GF;
    int m_GC;
    std::string m_nombre;
    int m_titulos;

    Equipo(float promedio_goles, int puntos, int GF, int GC, std::string nombre, int titulos){
        m_promedio_goles = promedio_goles;
        m_puntos = puntos;
        m_GF = GF;
        m_GC = GC;
        m_nombre = nombre;
        m_titulos = titulos;

    }
    
    void print_datos(){
        std::cout << "Equipo: " << m_nombre << "\n";
        std::cout << "Promedio de goles: " << m_promedio_goles << "\n";
        std::cout << "Puntos: " << m_puntos << "\n";
        std::cout << "Goles a favor: " << m_GF << "\n";
        std::cout << "Goles en contra: " << m_GC << "\n";
        std::cout << "Titulos: " << m_titulos << "\n";
        std::cout << std::endl;
    }
};


int poisson(float lambda){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::poisson_distribution<int> d(lambda);
    return d(generator);
}
void calcula_puntos(int n, int m, Equipo *equipo1, Equipo *equipo2){
   if(n == m){
        equipo1->m_puntos += 1;
        equipo2->m_puntos += 1;
    }
    else if(n < m){
        equipo2->m_puntos += 3;
    }
    else if(n > m){
        equipo1->m_puntos += 3;
    }
    equipo1->m_GC += m;
    equipo2->m_GC += n;
    equipo1->m_GF += n;
    equipo2->m_GF += m;   
}

void matches(std::vector<Equipo> &Equipos){
    for(int i = 0; i < Equipos.size(); i++){
        for(int j = i+1; j < Equipos.size();j++){
            unsigned int n = poisson(Equipos[i].m_promedio_goles);
            unsigned int m = poisson(Equipos[j].m_promedio_goles);
            calcula_puntos(n,m,&Equipos[i],&Equipos[j]);
        }
    }
}
void reset(std::vector<Equipo> &Equipos){
    for(int i = 0; i < Equipos.size(); i++){
        Equipos[i].m_puntos = 0;
        Equipos[i].m_GF = 0;
        Equipos[i].m_GC = 0;
    }    
}
void liguilla(std::vector<Equipo> &Equipos){
    reset(Equipos);
    /*cuartos de final */
    std::vector<Equipo> semis;
    for(int j = 11, k = 18; j < 15, k >14; j++, k--){
        unsigned int n = poisson(Equipos[j].m_promedio_goles);
        unsigned int m = poisson(Equipos[k].m_promedio_goles);
        calcula_puntos(n,m,&Equipos[j],&Equipos[k]);
        n = poisson(Equipos[k].m_promedio_goles);
        m = poisson(Equipos[j].m_promedio_goles);
        calcula_puntos(n,m,&Equipos[k],&Equipos[j]);
        if(Equipos[j].m_GF < Equipos[k].m_GF || Equipos[j].m_GF == Equipos[k].m_GF)
            semis.push_back(Equipos[k]);
        else if(Equipos[j].m_GF > Equipos[k].m_GF)
            semis.push_back(Equipos[j]);
    }
    /*semifinales */
    reset(semis);
    std::vector<Equipo> final;
    for(int j = 0, k = 3; j < 2, k > 1; j++, k--){
        unsigned int n = poisson(semis[j].m_promedio_goles);
        unsigned int m = poisson(semis[k].m_promedio_goles);
        calcula_puntos(n,m,&semis[j],&semis[k]);
        n = poisson(semis[k].m_promedio_goles);
        m = poisson(semis[j].m_promedio_goles);
        calcula_puntos(n,m,&semis[k],&semis[j]);
        if(semis[j].m_GF < semis[k].m_GF || semis[j].m_GF == semis[k].m_GF)
            final.push_back(semis[k]);
        else if(semis[j].m_GF > semis[k].m_GF)
            final.push_back(semis[j]);
    }
    /*final */
    unsigned int n = poisson(final[0].m_promedio_goles);
    unsigned int m = poisson(final[1].m_promedio_goles);
    while(n == m){
        n = poisson(final[0].m_promedio_goles);
        m = poisson(final[1].m_promedio_goles);
    }
    if(n < m){
        for(int i = 0; i < Equipos.size(); i++){
            if(Equipos[i].m_nombre == final[1].m_nombre)
                Equipos[i].m_titulos += 1;
        }
    }
    else if(n > m){
        for(int i = 0; i < Equipos.size(); i++){
            if(Equipos[i].m_nombre == final[0].m_nombre)
                Equipos[i].m_titulos += 1;
        }
    }      
}

void cambiar(Equipo *equipo1, Equipo *equipo2){
    Equipo token = *equipo1;
    *equipo1 = *equipo2;
    *equipo2 = token;

}
void ordenar(std::vector<Equipo> &equipos){
    for(int i = 0 ; i < equipos.size() - 1; i++){
        for(int j = 0; j < equipos.size() - i -1; j++){
            if(equipos[j].m_puntos > equipos[j+1].m_puntos){
                cambiar(&equipos[j], &equipos[j+1]);
            }

        }
    }
}
void ordenarT(std::vector<Equipo> &equipos){
    for(int i = 0; i < equipos.size() - 1; i++){
        for(int j = 0; j < equipos.size() - i - 1; j++){
            if(equipos[j].m_titulos < equipos[j+1].m_titulos){
                cambiar(&equipos[j], &equipos[j+1]);
            }
        }
    }
}
int main() {

    Equipo America(1.777, 0, 0, 0, "America",0);
    Equipo Atlas(1.118, 0, 0, 0, "Atlas",0);
    Equipo ASL(1.235, 0, 0, 0, "Atl. San Luis",0);
    Equipo Cruz_Azul(1.294, 0, 0, 0, "Cruz Azul",0);
    Equipo Chivas(1.471,0,0,0, "Guadalajara",0);
    Equipo CdJuarez(0.941, 0, 0, 0, "Cd. Juarez",0);
    Equipo Leon(2.117, 0, 0, 0, "Leon",0);
    Equipo Monterrey(1.471, 0, 0, 0, "Monterrey",0);
    Equipo Morelia(1.765, 0, 0, 0, "Morelia",0);
    Equipo Necaxa(1.941, 0, 0, 0, "Necaxa",0);
    Equipo Pachuca(1.765, 0, 0, 0, "Pachuca",0);
    Equipo Puebla(1, 0, 0, 0, "Puebla",0);
    Equipo Queretaro(1.647, 0, 0, 0, "Queretaro",0);
    Equipo Santos(2.235, 0, 0, 0, "Santos",0);
    Equipo Tijuana(1.471, 0, 0, 0, "Tijuana",0);
    Equipo Toluca(0.823, 0, 0, 0, "Toluca",0);
    Equipo Tigres(1.412,0,0,0, "Tigres",0);
    Equipo UNAM(1.235, 0, 0, 0, "UNAM",0);
    Equipo Veracruz(0.588, 0, 0, 0, "Veracruz",0);

    std::vector<Equipo> Equipos;
    Equipos.push_back(America);
    Equipos.push_back(Atlas);
    Equipos.push_back(ASL);
    Equipos.push_back(Cruz_Azul);    
    Equipos.push_back(Chivas);
    Equipos.push_back(CdJuarez);
    Equipos.push_back(Leon);
    Equipos.push_back(Monterrey);
    Equipos.push_back(Morelia);
    Equipos.push_back(Necaxa);
    Equipos.push_back(Pachuca);
    Equipos.push_back(Puebla);
    Equipos.push_back(Queretaro);
    Equipos.push_back(Santos);
    Equipos.push_back(Tijuana);
    Equipos.push_back(Toluca);        
    Equipos.push_back(Tigres);
    Equipos.push_back(UNAM);
    Equipos.push_back(Veracruz);

    for(float i = 0; i < 1000000; i++){
    matches(Equipos);
    ordenar(Equipos);
    liguilla(Equipos);
    reset(Equipos);
    printf("%.2lf", i/10000);
    std::cout << "\r";
    
    }
    ordenarT(Equipos);
    std::cout << "Simulación terminada" << "\n";
    getchar();
    for(int i = 0; i < Equipos.size(); i++)
        std::cout << "Equipo: " << Equipos[i].m_nombre << ". Titulos: " << Equipos[i].m_titulos << "\n"; 
    return 0;
}