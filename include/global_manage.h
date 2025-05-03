#ifndef GLOBAL_MANAGE_H
#define GLOBAL_MANAGE_H

typedef enum 
{
    VERMELHO=0,
    AMARELO=1,
    VERDE=2,
    CINZA=3
} EstadoSemaforo;

typedef enum 
{
    NOITE=0,
    DIA=1
} EstadoDia;

void set_vermelho();

void set_verde();

void set_amarelo();

void set_cinza();

EstadoSemaforo get_cor_sem();

void set_dia();

void set_noite();

EstadoDia get_dia_est();

#endif