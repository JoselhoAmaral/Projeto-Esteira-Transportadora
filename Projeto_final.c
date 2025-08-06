#define On_Off pin_b0
#define SentidoMotores pin_b1

#define LedVermelho pin_d7
#define LedVerde pin_d1
#define LedAmarelo pin_d0

#define motorgo pin_d3
#define motorback pin_d2

#include <18F4550.h>
#fuses HS, CPUDIV1, PLL5, USBDIV
#use delay(clock = 20MHz)


void main(){
   port_b_pullups(true);   // Liga os pull ups
   output_d(0b00000000);   // Desliga todas as portas D
   SET_TRIS_B(0xFF);       // Seta o grupo B como entrada.
   SET_TRIS_D(0x00);       // Seta o grupo D como saída.
   
   boolean estado = 0;
   int sentido = 0;
   
   /////////////////////////////
   int valor = 90;
   
   setup_ccp1(CCP_PWM); //habilita o uso do PWM
   setup_timer_2(T2_DIV_BY_4, 249, 1); //timer do pwm
   /////////////////////////////
   
while(true){
   //////////////////////
   set_pwm1_duty(valor); //aqui é mandado a velocidade do pwm para o pwm1.
   
// ajuste de velocidade do pwm ----------------------------------------------//
// if que adiciona mais ou menos 10% ao valor total do pwm
  
      if (valor < 50 ){valor = 100;}
      if((input(pin_b2)) == 0){
      valor = valor + 25;
      delay_ms(200);
      
         if (valor > 150){
            valor = 0;
         }
      }
   /////////////////////
   
   if (input(On_Off) == 0){ //Liga os motres se apertar b0
      estado = !estado; //inverte o valor do estado
      delay_ms(300);
   }
   
   if ((input(SentidoMotores)) == 0){ //Muda como os motores se comporta cada
      sentido++;                      //vez que aperta o botão b1
      
      // Para os motores
      output_bit(motorgo, 0);
      output_bit(motorback, 0);
      output_bit(LedVermelho, 1); //liga o led vermelho
      delay_ms(3000);
      output_bit(LedVermelho, 0); //desliga o led vermelho
      
      // Comando para reiniciar o contador do sentido.
      if(sentido == 2){sentido = 0;}
      delay_ms(500);
   }
   
   if (estado == 1){ //Se estado for igual a 1, os motores ligam.
      if (sentido == 0){ //Indo para frente
         output_bit(motorgo, 1);
         output_bit(motorback, 0);
         
         output_bit(LedAmarelo, 0); //Desliga pino amarelo
      }
      
      if (sentido == 1){ //Indo para trás
         output_bit(motorgo, 0);
         output_bit(motorback, 1);
      
         output_bit(LedAmarelo, 1); //Liga pino amarelo
      }
   }
   
   // Leds de estados....
   if(estado == 0) { // Para indicar que está desligado
      output_bit(LedVermelho,!estado);
      output_bit(LedVerde,estado);   

      output_bit(motorgo, 0);
      output_bit(motorback, 0);
      delay_ms(500);
   }
   if(estado == 1) { // Para indicar que está ligado
      output_bit(LedVerde,estado);
      output_bit(LedVermelho,!estado);
   }
   output_bit(pin_d0,0);
   output_bit(pin_d6,0);
} //Fim while()
} //Fim main()
