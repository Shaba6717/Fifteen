#include <CMSIS/stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
// типовой вариант функции assert
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
    ;
  }
}

//Глобальные переменные 

volatile uint16_t delay_count=0;
volatile bool  flag = 0;

// p - pause
volatile unsigned char buffer = 'p';

//char Send_buffer[100];
//uint8_t Send_count = 0;

//Положение пустой клетки 
uint8_t tec_i = 2;
uint8_t tec_j = 0;

//--------------------------------------------

//--------------------------------------------

void SysTick_Handler(void)
{
	if(delay_count > 0)
	{
		delay_count--;
	}
}

//--------------------------------------------

//--------------------------------------------

void delay_ms(uint32_t delay_temp)
{
	delay_count = delay_temp;

	while(delay_count){}
}

//--------------------------------------------

//--------------------------------------------

static void USART_ini(void)
{
	//Настраиваем пины
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef USART_PinTx;
	GPIO_InitTypeDef USART_PinRx;

	USART_PinTx.GPIO_Mode = GPIO_Mode_AF_PP;
	USART_PinTx.GPIO_Pin = GPIO_Pin_9;
	USART_PinTx.GPIO_Speed = GPIO_Speed_50MHz;
	
	USART_PinRx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	USART_PinRx.GPIO_Pin = GPIO_Pin_10;
		
	GPIO_Init(GPIOA, &USART_PinTx);
	GPIO_Init(GPIOA, &USART_PinRx);

	
	
	//Настраиваем USART
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef USART_InitPlayer;
	
	USART_InitPlayer.USART_BaudRate = 9600;
	USART_InitPlayer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitPlayer.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_InitPlayer.USART_Parity = USART_Parity_No;
	USART_InitPlayer.USART_StopBits = USART_StopBits_1;
	USART_InitPlayer.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, &USART_InitPlayer);

	NVIC_EnableIRQ(USART1_IRQn);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

//--------------------------------------------

//--------------------------------------------

static void send_Uarti(uint16_t m)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)== RESET){}
	USART_SendData(USART1, m);
}

//--------------------------------------------

//--------------------------------------------

static void send_Uartc(unsigned char c)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)== RESET){}
	USART_SendData(USART1, c);
}

//--------------------------------------------

//--------------------------------------------

static void send_Uart_str(char* str_p)
{
	uint16_t i = 0;
	while(str_p[i] != 0)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)== RESET){}
		USART_SendData(USART1, str_p[i]);
		i++; 
		//Send_count++;
		//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
}

//--------------------------------------------

//--------------------------------------------

static void create_pole(unsigned char arr[][4])
{
	for(uint8_t i=0; i <=3;i++)
	{
		for(uint8_t j=0; j <= 3; j++)
		{
			send_Uartc(arr[i][j]);
//		send_Uartc(*(arr+i*4+j));
//		Send_buffer[Send_count] = (*(arr+i*4+j));
//		Send_count++;
		}
		send_Uartc('\n');
//	Send_buffer[Send_count] = '\n';
//	Send_count++;
	} 
	send_Uartc('\n');
//Send_buffer[Send_count] = '\n';
//Send_count = 0;
//	
//USART_SendData(USART1, Send_buffer[Send_count]);
//Send_count++;
//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

//--------------------------------------------

//--------------------------------------------
static void Error(void)
{
	char str[64] = "Error\nInvalid symbol\nTry again,please\n\n";
	send_Uart_str(str);
	buffer = 'p';
};

//--------------------------------------------

//--------------------------------------------

static void W_Up(unsigned char arr[][4])
{
	flag = 0;
	unsigned char tec= ' ';
	if(tec_i == 0) Error();
	else 
	{
		tec = arr[tec_i][tec_j];
		arr[tec_i][tec_j] = arr[tec_i-1][tec_j];
		arr[tec_i-1][tec_j] = tec;
		tec_i = tec_i - 1;
		buffer = 'p';
		create_pole(arr);
	}
}
//--------------------------------------------

//--------------------------------------------
static void S_Down(unsigned char arr[][4])

{
	flag = 0;
	unsigned char tec= ' ';
	if(tec_i == 3) Error();
	else
	{
		tec = arr[tec_i][tec_j];
		arr[tec_i][tec_j] = arr[tec_i+1][tec_j];
		arr[tec_i+1][tec_j] = tec;
		tec_i = tec_i + 1;
		buffer = 'p';
		create_pole(arr);
	}
}

//--------------------------------------------

//--------------------------------------------

static void A_Left(unsigned char arr[][4])
{
	flag = 0;
	unsigned char tec= ' ';
	if(tec_j == 0) Error();
	else
	{
		tec = arr[tec_i][tec_j];
		arr[tec_i][tec_j] = arr[tec_i][tec_j-1];
		arr[tec_i][tec_j-1] = tec;
		tec_j = tec_j - 1;
		buffer = 'p';
		create_pole(arr);
	}
}

//--------------------------------------------

//--------------------------------------------

static void D_Right(unsigned char arr[][4])
{
	flag = 0;
	unsigned char tec= ' ';
	if(tec_j == 3) Error();
	else
	{
		tec = arr[tec_i][tec_j];
		arr[tec_i][tec_j] = arr[tec_i][tec_j+1];
		arr[tec_i][tec_j+1] = tec;
		tec_j = tec_j + 1;
		buffer = 'p';
		create_pole(arr);
	}
}

//--------------------------------------------

//--------------------------------------------

void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)== SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		buffer = USART_ReceiveData(USART1);
		flag = 1;
	};

//	if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)
//	{
//		USART_ClearITPendingBit(USART1, USART_IT_TXE);
//		if(Send_buffer[Send_count] != '0')
//		{
//			USART_SendData(USART1, Send_buffer[Send_count]);
//			Send_count++;
//		} else {
//			Send_count = 0;
//			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//		}
//	}
}

//--------------------------------------------

//--------------------------------------------

int main(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);

	char message[64] = ("\nSend 'p' to pause\nSend w/a/s/d to move\n\n");
	uint8_t win_count = 0;
	
	unsigned char Pole[4][4] = 
	{
    { '1', '4', '3', '8'},
    { '5', '2', '7', 'B'},
    { '*', 'A', '6', 'C' },
		{ 'D', 'E', 'F', '9' }
	};

	unsigned char Pole_Win[4][4] = 
	{
    { '1', '2', '3', '4'},
    { '5', '6', '7', '8'},
    { '9', 'A', 'B', 'C' },
		{ 'D', 'E', 'F', '*' }
	};


	USART_ini();
	create_pole(Pole);
	send_Uart_str(message);
	sprintf(message, "\nCongratulations! You won!\n");
	while(1)
	{
		//Сравнение текущего состояние поля с победным
		win_count = memcmp ( Pole, Pole_Win, 16);

		//Если все элементы совпали, то выводим поздравление и выходим из цикла
		if (win_count == 0)
		{
			send_Uart_str(message);
			create_pole(Pole);
			break;
		};

		//Обработка полученного символа
		//Перемещение - w/a/s/d
		//Ожидание - p
		//Все остальное - сообщение об ошибке
		if (flag == 1)
		{
			if (buffer == 'w')
			{
				W_Up(Pole);
			} 
			else if (buffer == 's')
			{
				S_Down(Pole);
			}
			else if (buffer == 'a')
			{
				A_Left(Pole);
			}
			else if (buffer == 'd')
			{
				D_Right(Pole);
			}
			else if (buffer == 'p')
			{
				continue; 
			}	else Error();
		}
		
	};

	return 0;
}
