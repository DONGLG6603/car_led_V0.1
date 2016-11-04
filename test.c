/*NECЭ�����ͨ��
*/
#include <reg52.h>
#include <intrins.h>
/*====================================
 �Զ���������
====================================*/
typedef unsigned char INT8U;
typedef unsigned char uchar;

typedef unsigned int INT16U;
typedef unsigned int uint;


uchar IRtime;	  //���������ߵ͵�ƽ����ʱ��
uchar IRcord[4];  //���������4���ֽ�����
uchar IRdata[33]; //������ʼ�����ڵ�33λ����
bit IRpro_ok;	  //�����4���ֽ����ݽ�����ɱ�־λ
bit IRok;		  //33λ���ݽ�����ɱ�־


sbit LED1 = P1^0;
sbit LED2 =	P1^1;
sbit LED3 = P1^2;
sbit LED4 =	P1^3;
sbit LED5 = P1^4;
sbit LED6 =	P1^5;
sbit LED7 = P1^6;
sbit LED8 =	P1^7;
//��ʼ����ʱ��0���ⲿ�ж�0������

void delay(uint z)//��ʱ����
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=114;y>0;y--);
}

void init()
{
	TMOD |= 0x02;//���ö�ʱ��0����ģʽ2,8λ�Զ���װ
	TL0 = TH0 = 0;//��ʼ����ʱ��0�Ĵ�������ʱ��0���һ��ʱ��Ϊ256����������
	EA = 1;//�����ж�
	ET0 = 1;//����ʱ��0�ж�
	TR0 = 1;//������ʱ��0

	IT0 = 1;//�����ⲿ�½��ش�����ʽ
	EX0 = 1;//���ⲿ�ж�0�ж�
}

//��ʱ��0�жϣ�ÿ�ж�һ����Ҫ256*1.085us =  277.76us
void time0() interrupt 1
{
	IRtime++;//277.76us
}

//�ⲿ�ж�0 ����33������
void int0() interrupt 0
{
	static uchar i;//��̬�������ڴ���33�����ݼ���
	uint startflag;//��ʼ���������־λ
	if(startflag)
	{
		/*�ж������룬����������������ʼ�뿪ʼ��*/
		if((IRtime < 53) && (IRtime >= 32))	i = 0;
		IRdata[i] = IRtime;//��TO����Ĵ�����������������ʱ������������
		IRtime = 0;//��������
		i++;//���������������Լ�
		if(i == 33)	   //i����33��ô�ͱ�ʾ�Ѿ�������33������
		{
			IRok = 1; //���������
			i = 0;	  //�������������׼���´δ���
		}
	}
	else
	{
		IRtime = 0;	  //��ʱ��0��������
		startflag = 1;//��ʼ�����־λ��1
	}

}

//����ȡ��33��������н��� NECЭ��
void IRcordpro()
{
	uchar i;//i�����ڼ�������4���ֽ�
	uchar j;//j���ڼ�������1���ֽڵ�8λ����
	uchar k;//k���ڼ�������33������
	k = 1;//�ӵ�һλ����ʼ����������ʼ��
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 8; j++)
		{
			//��������������0��׼��1125us��ô���ж�Ϊ����1
			if(IRdata[k] > 5) IRcord[i] |= 0x80;//д1
			//ֻ������7�Σ��������8�����ѵ�һλ�����Ƴ�ȥ
			if(j < 7) IRcord[i] >>= 1;
			k++; //������һ������
		}
	}
	IRpro_ok = 1;//�������
}


void LED2k()//LED2�ƿ��Ƴ��� Ƶ����
{	
	if(IRcord[2] == 0x18)
	{
				init();	
				while(1)
				{	
					uint i;
					uint j;
					if(LED6 == 0)
					{LED6 = 0;LED2 = 0;delay(500);LED6 = 1;LED2 = 1;delay(500);j++;}
					if(LED6 == 1 & j ==0)
					{LED2 = 0;LED6 = 1;delay(500);LED2 = 1;LED6 = 1;delay(500);}
					if(j == 1)
					{LED2 = 0;LED6 = 0;delay(500);LED2 = 1;LED6 = 1;delay(500);}
					
					if(IRok)//�ж�33�������Ƿ���ȡ���
					{
					 	IRcordpro();//������������4���ֽڵ�����
						switch(IRcord[2])//�жϵ���������ֵ
				         {
						 case 0x0c:LED1 = ~LED1;;break;//1 ��ʾ��Ӧ�İ���ֵ
						 case 0x18:i = 1;break;//2
						 case 0x5e:LED3 = ~LED3;break;//3
						 case 0x08:LED4 = ~LED4;break;//4
						 case 0x1c:LED5 = ~LED5;break;//5
						 case 0x5a:LED6 = ~LED6;break;//6
						 case 0x42:LED7 = ~LED7;break;//7
						 case 0x52:LED8 = ~LED8;break;//8
						 case 0x4a:P1= 0xff;break;//9
			             default:P1 = 0x00;break;
						 }
			
					 	 IRpro_ok=0;//������ɱ�־
						 IRok = 0;//������������ɱ�־λ�ȴ���һ��������
					}
					if(i == 1)
					{
						LED2 = 1;
						i = 0;
						j = 0;
						break;	
					}
				}
				return;
	}
	else if(IRcord[2] == 0x5a)
	{
		init();	
				while(1)
				{	
					uint i;
					uint j;
					if(LED2 == 0)
					{LED6 = 0;LED2 = 0;delay(500);LED6 = 1;LED2 = 1;delay(500);j++;}
					if(LED2 == 1 & j ==0)
					{LED6 = 0;LED2 = 1;delay(500);LED6 = 1;LED2 = 1;delay(500);}
					if(j == 1)
					{LED6 = 0;LED2 = 0;delay(500);LED6 = 1;LED2 = 1;delay(500);}
					
					if(IRok)//�ж�33�������Ƿ���ȡ���
					{
					 	IRcordpro();//������������4���ֽڵ�����
						switch(IRcord[2])//�жϵ���������ֵ
				         {
						 case 0x0c:LED1 = ~LED1;;break;//1 ��ʾ��Ӧ�İ���ֵ
						 case 0x18:LED2 = ~LED2;break;//2
						 case 0x5e:LED3 = ~LED3;break;//3
						 case 0x08:LED4 = ~LED4;break;//4
						 case 0x1c:LED5 = ~LED5;break;//5
						 case 0x5a:i = 1;break;//6
						 case 0x42:LED7 = ~LED7;break;//7
						 case 0x52:LED8 = ~LED8;break;//8
						 case 0x4a:P1= 0xff;break;//9
			             default:P1 = 0x00;break;
						 }
			
					 	 IRpro_ok=0;//������ɱ�־
						 IRok = 0;//������������ɱ�־λ�ȴ���һ��������
					}
					if(i == 1)
					{
						LED6 = 1;
						i = 0;
						j = 0;
						break;	
					}
				}
				return;	
	}			
}

void Ir_work(void)//�����ֵɢת����
{
       switch(IRcord[2])//�жϵ���������ֵ
	         {
			 case 0x0c:LED1 = ~LED1;;break;//1 ��ʾ��Ӧ�İ���ֵ
			 case 0x18:LED2k();break;//2
			 case 0x5e:LED3 = ~LED3;break;//3
			 case 0x08:LED4 = ~LED4;break;//4
			 case 0x1c:LED5 = ~LED5;break;//5
			 case 0x5a:LED2k();break;//6
			 case 0x42:LED7 = ~LED7;break;//7
			 case 0x52:LED8 = ~LED8;break;//8
			 case 0x4a:P1= 0xff;break;//9
             default:P1 = 0x00;break;
			 }

		  IRpro_ok=0;//������ɱ�־

}

void main()
{
	init();//��ʼ��
	while(1)
	{
		if(IRok)//�ж�33�������Ƿ���ȡ���
		{
			IRok = 0;//������������ɱ�־λ�ȴ���һ��������
		 	IRcordpro();//������������4���ֽڵ�����
			Ir_work();
		}
	}

}