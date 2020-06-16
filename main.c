#include <tm4c123gh6pm.h>
#include <inttypes.h>

int start = 0, level = 0, resultado = 0, teste =0;
uint32_t segundo = 0 ;
int jogo_proposto[20], jogada_usuario[20] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5} ;

void delay_ms(int ms)
{
    uint32_t aux, aux2;
    for (aux = 0; aux < ms;aux++)
        for(aux2 = 0; aux2 < 1600; aux2++) { }
}

void tempo()
{
    segundo++;
}

void trataPortf()
{
    delay_ms(50);
    int i = 0;
    if(level == 0)
    {
        for( i = 0; i < 20; i++)
        {
            jogo_proposto[i] = NVIC_ST_CURRENT_R % 4; // 0 - 1600000000000
            level = 1;
        }
        start = 1;
    }
    GPIO_PORTF_ICR_R = 0x10; // Limpar a interrupção - Necessário
}

void indicador_LED()
{
    int j = 0;
    for(j = 0; j < level; j++)
    {
        turnOn(jogo_proposto[j]);
    }
}

void result_game()
{
   if (resultado == 1) // ganhou
   {
       turnOn(5);
       resultado = 0;
   }
   if (resultado == 2) // perdeu
   {
       turnOn(4);
       resultado = 0;
   }
}

void turnOn (int cor)
{
    segundo = 0;
    switch(cor){
    case 0 :// Amarelo PB4
        while(segundo <= 1)
        {
            GPIO_PORTB_DATA_R = 0x10;
        }
        GPIO_PORTB_DATA_R = 0x00;
        break;
    case 1: // Azul PB5
        while(segundo <= 1)
        {
            GPIO_PORTB_DATA_R = 0x20;
        }
        GPIO_PORTB_DATA_R = 0x00;
        break;
    case 2: // Verde PB6
        while(segundo <= 1)
        {
            GPIO_PORTB_DATA_R = 0x40;
        }
        GPIO_PORTB_DATA_R = 0x00;
        break;
    case 3: // Vermelho PB7
        while(segundo <= 1)
        {
            GPIO_PORTB_DATA_R = 0x80;
        }
        GPIO_PORTB_DATA_R = 0x00;
        break;
    case 4: // Vermelho Tiva PF1
            while(segundo <= 1)
            {
                GPIO_PORTF_DATA_R = 0x02;
            }
            GPIO_PORTF_DATA_R = 0x00;
            break;
    case 5: // Verde Tiva PF3
            while(segundo <= 1)
            {
                GPIO_PORTF_DATA_R = 0x08;
            }
            GPIO_PORTF_DATA_R = 0x00;
            break;
    }
}

void verifica_jogada()
{
    int i = 0;
    for(i = 0; i < level; i++)
    {
        if(jogo_proposto[i] != jogada_usuario[i])
        {
                resultado = 2;
                result_game();
                break;
        }

    }

    level++;

    if (level == 19)
    {
        resultado = 1;
        result_game();
    }

}

void leitura_BT()
{

    //segundo = 0;
    //while(segundo <= 1){}
        int i = 0;
        for( i = 0; i < level; i++)
        {
            switch (GPIO_PORTB_MIS_R ){
                case 0x01:
                    jogada_usuario[i] = 0;
                    break;
                case 0x02:
                    jogada_usuario[i] = 1;
                    break;
                case 0x04:
                    jogada_usuario[i] = 2;
                    break;
                case 0x08:
                    jogada_usuario[i] = 3;
                    break;

            }
            /*if (GPIO_PORTB_MIS_R == 0x01) // Botão amarelo PB0
            {
                jogada_usuario[i] = 0;
            }
            else if (GPIO_PORTB_MIS_R == 0x02) // Botão azul PB1
            {
                jogada_usuario[i] = 1;
            }
            else if (GPIO_PORTB_MIS_R == 0x04)  // Botão verde PB2
            {
                jogada_usuario[i] = 2;
            }
            else if (GPIO_PORTB_MIS_R == 0x08) // Botão vermelho PB3
            {
                jogada_usuario[i] = 3;
            }*/
        }
        teste = 1;
    GPIO_PORTF_ICR_R = 0x0F; // Limpar a interrupção - Necessário
}

int main(void)
{
    SYSCTL_RCGCGPIO_R = 0x22;

    // Configurações do PORT F
    GPIO_PORTF_DIR_R = 0x0A;  // Pinos 1 e 3 sendo direcionados OUTPUT e 4 e 5 sendo direcionados INPUT
    GPIO_PORTF_IS_R = 0x00;   // Explicitar o valor defaut da Interruption Sense - sensibilidade da interrupção como borda nos pinos 4 e 5
    GPIO_PORTF_IEV_R = 0x00;  // Eventos da interrupção como de descida na no IS dos pinos 4 e 5
    GPIO_PORTF_IBE_R = 0x00;  // Evento de interrupção em ambas as bordas ou nível - (Seguir o GPIO_ IEV)

    // Habilitações do PORT F
    GPIO_PORTF_DEN_R = 0x3A;  // Pinos 1, 3, 4 e 5 sendo habilitados
    GPIO_PORTF_PUR_R = 0x30;  // Habilitar o Pull-Up do Pinos de INPUT
    GPIO_PORTF_IM_R = 0x30;   // Habilitando a máscara para interrupções no pinos 4 e 5

    // Configurações do PORT B
    GPIO_PORTB_DIR_R = 0xF0;  // Pinos 4, 5, 6 e 7 sendo direcionados OUTPUT e 0, 1, 2 e 3 sendo direcionados INPUT
    GPIO_PORTB_IS_R = 0x00;   // Explicitar o valor defaut da Interruption Sense - sensibilidade de borda de descida nos pinos 1, 2 e 3
    GPIO_PORTB_IEV_R = 0x00;  // Eventos da interrupção como de descida no IS dos pinos 0, 1, 2 e 3
    GPIO_PORTB_IBE_R = 0x00;  // Evento de interrupção em ambas as bordas ou nível - (Seguir o GPIO_ IEV)

    // Habilitações do PORT B
    GPIO_PORTB_DEN_R = 0xFE;  // Pinos 1, 2, 3, 4, 5, 6 e 7 sendo habilitados
    GPIO_PORTB_PUR_R = 0x0E;  // Habilitar o Pull-Up do Pinos 0, 1, 2 e 3
    GPIO_PORTB_IM_R = 0x0E;   // Habilitando a máscara para interrupções no pinos 1, 2 e 3

    NVIC_ST_RELOAD_R = 8000000; // Valor inicial
    NVIC_ST_CTRL_R = 0x07;  // Status de todas as configurações de controle

    NVIC_EN0_R = 0x40000002;

    while(1)
    {
        while(start == 0) {}

        int aux = 0;

        for(aux = 0; aux < 20; aux++)
        {
                indicador_LED();

                while(teste == 0) {
                }
                //leitura_BT();
                verifica_jogada();
                teste = 0;
        }
        start = 0;
        level = 0;

    }
    return 0;
}
