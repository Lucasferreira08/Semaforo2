#include "global_manage.h"

static EstadoSemaforo estadoSemaforo;
static EstadoDia estadoDia;

void set_vermelho()
{
    estadoSemaforo=VERMELHO;
}

void set_verde()
{
    estadoSemaforo=VERDE;
}

void set_amarelo()
{
    estadoSemaforo=AMARELO;
}

void set_cinza()
{
    estadoSemaforo=CINZA;
}

EstadoSemaforo get_cor_sem() 
{
    return estadoSemaforo;
}

void set_dia() 
{
    estadoDia=DIA;
}

void set_noite() 
{
    estadoDia=NOITE;
}

EstadoDia get_dia_est() 
{
    return estadoDia;
}