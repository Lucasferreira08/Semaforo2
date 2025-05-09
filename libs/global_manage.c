#include "global_manage.h"   // Declara protótipos de tipos e funções de gestão de estado

// Variáveis estáticas para controlar o estado global do semáforo e do ciclo dia/noite.
// “static” aqui garante escopo somente neste arquivo (linkagem interna).
static EstadoSemaforo estadoSemaforo;
static EstadoDia      estadoDia;

// Define o semáforo para vermelho
void set_vermelho()
{
    estadoSemaforo = VERMELHO;
}

// Define o semáforo para verde
void set_verde()
{
    estadoSemaforo = VERDE;
}

// Define o semáforo para amarelo
void set_amarelo()
{
    estadoSemaforo = AMARELO;
}

// Define o semáforo para cinza (apagado/inativo, usado no modo noite)
void set_cinza()
{
    estadoSemaforo = CINZA;
}

// Retorna o estado atual do semáforo
EstadoSemaforo get_cor_sem()
{
    return estadoSemaforo;
}

// Define o ciclo para dia
void set_dia()
{
    estadoDia = DIA;
}

// Define o ciclo para noite
void set_noite()
{
    estadoDia = NOITE;
}

// Retorna o estado atual do ciclo (dia ou noite)
EstadoDia get_dia_est()
{
    return estadoDia;
}
