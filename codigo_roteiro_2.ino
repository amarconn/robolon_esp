#include <ESP8266WiFi.h>//Biblioteca do WiFi.
#include <WiFiUdp.h>//Biblioteca do UDP.

/*

  D0 ==> 16
  D1 ==> 5 ==> PWM
  D2 ==> 4 ==> PWM Hardware
  D3 ==> 0 ==> PWM
  D4 ==> 2 ==> PWM LED Azul
  D5 ==> 14 ==> PWM Hardware
  D6 ==> 12 ==> PWM Hardware
  D7 ==> 13 ==> PWM
  D8 ==> 15 ==> PWM Hardware
  D9 ==> 3
  D10 ==> 1

*/
const char *ssid = "RedeRobo01";
const char *password = "rederobo01";

WiFiUDP udp;//Cria um objeto da classe UDP.

String req;//String que armazena os dados recebidos pela rede.
//-------------------------------------------------------------------------------------------------
//                                       setup
//-------------------------------------------------------------------------------------------------

void setup()
{

  Serial.begin(115200);//Habilita a comunicaçao serial para a string recebida ser lida no Serial monitor.

  ///////////////////////// Configura AP /////////////////////////////


  WiFi.mode(WIFI_AP);

  IPAddress Ip(192, 168, 19, 10);// define o endereção de ip do roteador do esp
  IPAddress NMask(255, 255, 255, 0);


  WiFi.softAPConfig(Ip, Ip, NMask);//WiFi.softAPConfig(IP do Ponto de acesso, IP do gateway, Mascara)
  WiFi.softAP(ssid, password);


  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  analogWriteRange(255);

  ///////////////////////// fim Configura AP /////////////////////////////  
 
  // ************************************************  PORTA UDP ***********************************************

  udp.begin(5001);//Inicializa a recepçao de dados UDP na porta 555

  // ************************************************  fim PORTA UDP ***********************************************
  
 pinMode(D0, OUTPUT);
 pinMode(D1, OUTPUT);
 pinMode(D2, OUTPUT);
  
}
void loop()
{

  listen();//Sub-rotina para verificar a existencia de pacotes UDP.

}

void listen()//Sub-rotina que verifica se há pacotes UDP's para serem lidos.
{
  int  Valor;
  char Texto;

  if (udp.parsePacket() > 0)//Se houver pacotes para serem lidos
  {

    req = "";//Reseta a string para receber uma nova informaçao
    while (udp.available() > 0)//Enquanto houver dados para serem lidos
    {
      char z = udp.read();//Adiciona o byte lido em uma char
      req += z;//Adiciona o char à string
    }

    //Após todos os dados serem lidos, a String estara pronta.
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          Serial.println(req);//Printa a string recebida no Serial monitor.
          Serial.println(req[0]);
          Serial.println(req[1]-48);
          Serial.println(req[2]-48);
          Serial.println(req[3]-48);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
    Texto = req[0];

    //transforma os caracteres numericos em inteiros

    if ((req[1] >= 48) && (req[2] >= 48) && (req[3] >= 48))
    {
      Valor = 100 * (req[1] - 48);
      Valor += 10 * (req[2] - 48);
      Valor += (req[2] - 48);
    }
    else
    {
      if ((req[1] >= 48) && (req[2] >= 48))
      {
        Valor = 10 * (req[1] - 48);
        Valor += (req[2] - 48);
      }
      else
      {
        Valor = (req[1] - 48);
      }
    }

    Serial.print("Texto ");
    Serial.println(Texto);
    Serial.print("Valor ");
    Serial.println(Valor);

    if(Texto=='a')  
      {
        if(Valor==0)
          {
            digitalWrite(D0, HIGH);
          }
        else
          {
            digitalWrite(D0, LOW);
          }
      }
    else if(Texto=='b')
      {
        if(Valor == 0)
        {
          digitalWrite(D1, HIGH);
        }
        else
        {
          digitalWrite(D1, LOW);
        }
      }
    else if(Texto=='c')
      {
        if(Valor == 0)
        {
          digitalWrite(D2, HIGH);
        }
        else
        {
          digitalWrite(D2, LOW);
        }
      }
  }
}
