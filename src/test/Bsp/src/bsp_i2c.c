/*
*********************************************************************************************************
*
*	��������: Ӳ��IIC����ģ��
*	�ļ�����: bsp_i2c.c
*	��    ��: v1.0.0
*   ��    д: Ī����
*	˵    ��: Ӳ��I2C����
*
*	�汾��¼: 
*	v1.0.0: 2021��4��13�գ�����
*
*	Copyright (C), 2021-2031, �����з����ƿƼ����޹�˾ https://fxymcu.taobao.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* I2C1��GPIO�ֲ�
SCL: PF1 PA9  PB6 PB8
SDA: PF0 PA10 PB7 PB9
*/
/* I2C1 ʱ���� */
#define I2C1_SCL_PORT GPIOB
#define I2C1_SCL_PIN GPIO_Pins_6
#define I2C1_SCL_SOURCE GPIO_PinsSource6
#define I2C1_SCL_AF GPIO_AF_1
/* I2C1 ������ */
#define I2C1_SDA_PORT GPIOB
#define I2C1_SDA_PIN GPIO_Pins_7
#define I2C1_SDA_SOURCE GPIO_PinsSource7
#define I2C1_SDA_AF GPIO_AF_1
/* I2C1 GPIOʱ�� */
#define I2C1_GPIO_RCC RCC_AHBPERIPH_GPIOB

/* I2C2��GPIO�ֲ�
SCL: PA0 PB10 PB13 PA11 PF6 PA8
SDA: PA1 PB11 PB14 PA12 PF7 PB4
*/
/* I2C2 ʱ���� */
#define I2C2_SCL_PORT GPIOA
#define I2C2_SCL_PIN GPIO_Pins_0
#define I2C2_SCL_SOURCE GPIO_PinsSource0
#define I2C2_SCL_AF GPIO_AF_4
/* I2C2 ������ */
#define I2C2_SDA_PORT GPIOA
#define I2C2_SDA_PIN GPIO_Pins_1
#define I2C2_SDA_SOURCE GPIO_PinsSource1
#define I2C2_SDA_AF GPIO_AF_4
/* I2C2 GPIOʱ�� */
#define I2C2_GPIO_RCC RCC_AHBPERIPH_GPIOA

/* I2C���߳�ʱ */
#define I2C_TIMEOUT_SET ((uint32_t)1000) /* ms */

/* I2C�¼�����־���� */
#define I2C_EVT_CHECK_NONE ((uint32_t)0x00000000)
#define I2C_EVT_CHECK_ACKFAIL ((uint32_t)0x00000001)
#define I2C_EVT_CHECK_STOP ((uint32_t)0x00000002)

/*
*********************************************************************************************************
*	�� �� ��: I2C_ClearADDRFlag
*	����˵��: ���I2C ADDR����ı�־
*	��    ��: I2Cx: I2C�豸
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2C_ClearADDRFlag(I2C_Type *I2Cx)
{
    __IO uint32_t tmpreg;

    tmpreg = I2Cx->STS1;
    tmpreg = I2Cx->STS2;
}

/*
*********************************************************************************************************
*	�� �� ��: I2C_ClearSTOPFlag
*	����˵��: ���I2C STOPF�����־
*	��    ��: I2Cx: I2C�豸
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2C_ClearSTOPFlag(I2C_Type *I2Cx)
{
    __IO uint32_t tmpreg;

    tmpreg = I2Cx->STS1;
    tmpreg = I2Cx->CTRL1 |= I2C_CTRL1_PEN;
}

/*
*********************************************************************************************************
*	�� �� ��: I2C_WaitOnFlagUntilTimeout
*	����˵��: I2Cͨ�ų�ʱ
*	��    ��: 
*	          I2Cx: I2C�豸 
*	          Flag: ָ��Ҫ����I2C��־ 
*	          Status: ��־״̬ 
*	          EventCheck: ����־ 
*	          Timeout: ��ʱʱ��(ms)
*	�� �� ֵ: ״̬����
*********************************************************************************************************
*/
static I2C_StatusType I2C_WaitOnFlagUntilTimeout(I2C_Type *I2Cx, uint32_t Flag, FlagStatus Status, uint32_t ulEventCheck, uint32_t ulTimeout)
{
    ulTimeout *= 100;

    while (I2C_GetFlagStatus(I2Cx, Flag) == Status)
    {
        /* ���Ӧ��ʧ�ܱ�־ */
        if (ulEventCheck & I2C_EVT_CHECK_ACKFAIL)
        {
            if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_ACKFAIL) == SET)
            {
                /* ֹͣλ */
                I2C_GenerateSTOP(I2Cx, ENABLE);
                /* ����Ӧ��ʧ�ܱ�־ */
                I2C_ClearFlag(I2Cx, I2C_FLAG_ACKFAIL);
                return I2C_ERROR;
            }
        }

        /* ���ֹͣλʧ�ܱ�־ */
        if (ulEventCheck & I2C_EVT_CHECK_STOP)
        {
            if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == SET)
            {
                /* ���I2C STOPF�����־ */
                I2C_ClearSTOPFlag(I2Cx);
                return I2C_ERROR;
            }
        }

        /* 10us��ʱ */
        bsp_DelayUs(10);
        /* ����Ƿ�ʱ */
        if ((ulTimeout--) == 0)
        {
            return I2C_TIMEOUT;
        }
    }
    return I2C_OK;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitI2C_Hard
*	����˵��: ����Ӳ��I2C����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitI2C_Hard(void)
{
    GPIO_InitType GPIO_InitStructure;
    I2C_InitType I2C_InitStructure;

#if I2C1_EN
    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(I2C1_GPIO_RCC, ENABLE);
    /* ʹ��I2C1ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_I2C1, ENABLE);

    /* ����GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* ���ù��� */
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_OD;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    /* SCL */
    GPIO_InitStructure.GPIO_Pins = I2C1_SCL_PIN;
    GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStructure);
    /* SDA */
    GPIO_InitStructure.GPIO_Pins = I2C1_SDA_PIN;
    GPIO_Init(I2C1_SDA_PORT, &GPIO_InitStructure);

    /* ���ø��ù��� */
    GPIO_PinAFConfig(I2C1_SCL_PORT, I2C1_SCL_SOURCE, I2C1_SCL_AF);
    GPIO_PinAFConfig(I2C1_SDA_PORT, I2C1_SDA_SOURCE, I2C1_SDA_AF);

    /* ����I2C1���� */
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_BitRate = I2C1_SPEED;              /* I2C�����ٶȣ����400KHz�� */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2CDevice;         /* I2C����ģʽ */
    I2C_InitStructure.I2C_FmDutyCycle = I2C_FmDutyCycle_2_1; /* I2C����ռ�ձ� */
    I2C_InitStructure.I2C_AddrMode = I2C_AddrMode_7bit;      /* 7λ��ַģʽ */
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;              /* ʹ��Ӧ�� */
    I2C_Init(I2C1, &I2C_InitStructure);
    /* ʹ��I2C */
    I2C_Cmd(I2C1, ENABLE);
#endif

#if I2C2_EN
    /* ʹ��GPIOʱ�� */
    RCC_AHBPeriphClockCmd(I2C2_GPIO_RCC, ENABLE);
    /* ʹ��I2C2ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_I2C2, ENABLE);

    /* ����GPIO */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; /* ���ù��� */
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_OD;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
    /* SCL */
    GPIO_InitStructure.GPIO_Pins = I2C2_SCL_PIN;
    GPIO_Init(I2C2_SCL_PORT, &GPIO_InitStructure);
    /* SDA */
    GPIO_InitStructure.GPIO_Pins = I2C2_SDA_PIN;
    GPIO_Init(I2C2_SDA_PORT, &GPIO_InitStructure);

    /* ���ø��ù��� */
    GPIO_PinAFConfig(I2C2_SCL_PORT, I2C2_SCL_SOURCE, I2C2_SCL_AF);
    GPIO_PinAFConfig(I2C2_SDA_PORT, I2C2_SDA_SOURCE, I2C2_SDA_AF);

    /* ����I2C2���� */
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_BitRate = I2C1_SPEED;              /* I2C�����ٶȣ����400KHz�� */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2CDevice;         /* I2C����ģʽ */
    I2C_InitStructure.I2C_FmDutyCycle = I2C_FmDutyCycle_2_1; /* I2C����ռ�ձ� */
    I2C_InitStructure.I2C_AddrMode = I2C_AddrMode_7bit;      /* 7λ��ַģʽ */
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;              /* ʹ��Ӧ�� */
    I2C_Init(I2C2, &I2C_InitStructure);
    /* ʹ��I2C2 */
    I2C_Cmd(I2C2, ENABLE);
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_I2C_ReadBuffer
*	����˵��: I2C���߶�ȡ����
*	��    ��: 
*	          I2Cx: I2C�豸
*	          ucSlaveAddr: �ӻ���ַ
*	          pByteBuf: ���ݻ���
*	          usDataLen: ���ݳ���
*	�� �� ֵ: ״̬
*********************************************************************************************************
*/
I2C_StatusType bsp_I2C_ReadBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pByteBuf, uint16_t usDataLen)
{
    /* �ȴ�I2C���߿��б�־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        return I2C_ERROR_STEP_1;
    }

    /* ��ֹPOS */
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);
    /* ʹ��Ӧ�� */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    /* ��ʼλ */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_2;
    }

    /* �豸��ַ */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Receive);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_3;
    }
    /* ����ADDRF��־ */
    I2C_ClearADDRFlag(I2Cx);

    if (usDataLen == 1)
    {
        /* ��ֹӦ�� */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
    }
    else if (usDataLen == 2)
    {
        /*ʹ��POS */
        I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);
        /* ��ֹӦ�� */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
    }
    else
    {
        /* ʹ��Ӧ�� */
        I2C_AcknowledgeConfig(I2Cx, ENABLE);
    }

    while (usDataLen)
    {
        if (usDataLen < 4)
        {
            if (usDataLen == 1)
            {
                /* �ȴ�������־ */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_RDNE, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* ֹͣλ */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_4;
                }

                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
            else if (usDataLen == 2)
            {
                /* �ȴ�������־ */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* ֹͣλ */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_5;
                }

                /* ֹͣλ */
                I2C_GenerateSTOP(I2Cx, ENABLE);

                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
            else
            {
                /* �ȴ�������־ */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* ֹͣλ */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_6;
                }

                /* ��ֹӦ�� */
                I2C_AcknowledgeConfig(I2Cx, DISABLE);

                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);

                /* �ȴ�������־ */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* ֹͣλ */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_7;
                }

                /* ֹͣλ */
                I2C_GenerateSTOP(I2Cx, ENABLE);

                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
        }
        else
        {
            /* �ȴ�������־ */
            if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_RDNE, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
            {
                /* ֹͣλ */
                I2C_GenerateSTOP(I2Cx, ENABLE);
                return I2C_ERROR_STEP_8;
            }

            /* ��ȡ���� */
            (*pByteBuf++) = I2C_ReceiveData(I2Cx);
            usDataLen--;
        }
    }
    return I2C_OK;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_I2C_SendBuffer
*	����˵��: I2C���߷�������
*	��    ��: 
*	          I2Cx: I2C�豸
*	          ucSlaveAddr: �ӻ���ַ
*	          pByteBuf: ���ݻ���
*	          usDataLen: ���ݳ���
*	�� �� ֵ: ״̬
*********************************************************************************************************
*/
I2C_StatusType bsp_I2C_SendBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pByteBuf, uint16_t usDataLen)
{
    /* �ȴ�I2C���߿��б�־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        return I2C_ERROR_STEP_1;
    }

    /* ��ֹPOS */
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

    /* ��ʼλ */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_2;
    }

    /* �豸��ַ */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Transmit);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_3;
    }

    /* ����ADDRF��־ */
    I2C_ClearADDRFlag(I2Cx);

    while (usDataLen)
    {
        /* �ȴ����ͱ�־ */
        if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_TDE, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
        {
            /* ֹͣλ */
            I2C_GenerateSTOP(I2Cx, ENABLE);
            return I2C_ERROR_STEP_4;
        }
        /* �������� */
        I2C_SendData(I2Cx, (*pByteBuf++));
        usDataLen--;
    }

    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_5;
    }

    /* ֹͣλ */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    return I2C_OK;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_I2C_ReadAddrBuffer
*	����˵��: I2C���߶�ȡָ����ַ����
*	��    ��: 
*	          I2Cx: I2C�豸 
*	          ucSlaveAddr: �ӻ���ַ 
*	          pDataAddr: ���ݵ�ַ 
*	          ucAddrLen: ��ַ���� 
*	          pByteBuf: ���ݻ��� 
*	          usDataLen: ���ݳ���
*	�� �� ֵ: ״̬
*********************************************************************************************************
*/
I2C_StatusType bsp_I2C_ReadAddrBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pDataAddr, uint8_t ucAddrLen, uint8_t *pByteBuf, uint16_t usDataLen)
{
    /* �ȴ�I2C���߿��б�־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        return I2C_ERROR_STEP_1;
    }

    /* ��ֹPOS */
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

    /* ��ʼλ*/
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_2;
    }

    /* ���ʹӻ���ַ */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Transmit);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_3;
    }

    /* ����ADDRF��־ */
    I2C_ClearADDRFlag(I2Cx);

    /* ���ݵ�ַ */
    while (ucAddrLen)
    {
        /* �ȴ�������־ */
        if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_TDE, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
        {
            /* ֹͣλ */
            I2C_GenerateSTOP(I2Cx, ENABLE);
            return I2C_ERROR_STEP_4;
        }
        /* �������� */
        I2C_SendData(I2Cx, (*pDataAddr++));
        ucAddrLen--;
    }

    /* ������ʼ�ź� */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_5;
    }

    /* ���ʹӻ���ַ-�� */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Receive);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_6;
    }

    /* ����ADDRF��־ */
    I2C_ClearADDRFlag(I2Cx);

    if (usDataLen == 1)
    {
        /* ��ֹӦ�� */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
    }
    else if (usDataLen == 2)
    {
        /* ʹ��POS */
        I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);
        /* ��ֹӦ�� */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);
    }
    else
    {
        /* ʹ��Ӧ�� */
        I2C_AcknowledgeConfig(I2Cx, ENABLE);
    }
    /* ��ȡ���� */
    while (usDataLen)
    {
        if (usDataLen < 4)
        {
            if (usDataLen == 1)
            {
                /* �ȴ�������־ */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_RDNE, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* ֹͣλ */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_7;
                }

                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
            else if (usDataLen == 2)
            {
                /* �ȴ�������־ */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* ֹͣλ */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_8;
                }

                /* ֹͣλ */
                I2C_GenerateSTOP(I2Cx, ENABLE);

                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
            else
            {
                /* �ȴ�������־ */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* ֹͣλ */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_9;
                }

                /* ��ֹӦ�� */
                I2C_AcknowledgeConfig(I2Cx, DISABLE);

                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);

                /* �ȴ�������־ */
                if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
                {
                    /* ֹͣλ */
                    I2C_GenerateSTOP(I2Cx, ENABLE);
                    return I2C_ERROR_STEP_10;
                }

                /* ֹͣλ */
                I2C_GenerateSTOP(I2Cx, ENABLE);

                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                /* ��ȡ���� */
                (*pByteBuf++) = I2C_ReceiveData(I2Cx);
                usDataLen = 0;
            }
        }
        else
        {
            /* �ȴ�������־ */
            if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_RDNE, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
            {
                /* ֹͣλ */
                I2C_GenerateSTOP(I2Cx, ENABLE);
                return I2C_ERROR_STEP_11;
            }

            /* ��ȡ���� */
            (*pByteBuf++) = I2C_ReceiveData(I2Cx);
            usDataLen--;
        }
    }
    return I2C_OK;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_I2C_SendAddrBuffer
*	����˵��: I2C���߷���ָ����ַ����
*	��    ��: 
*	          I2Cx: I2C�豸 
*	          ucSlaveAddr: �ӻ���ַ 
*	          pDataAddr: ���ݵ�ַ 
*	          ucAddrLen: ��ַ���� 
*	          pByteBuf: ���ݻ��� 
*	          usDataLen: ���ݳ���
*	�� �� ֵ: ״̬
*********************************************************************************************************
*/
I2C_StatusType bsp_I2C_SendAddrBuffer(I2C_Type *I2Cx, uint8_t ucSlaveAddr, uint8_t *pDataAddr, uint8_t ucAddrLen, uint8_t *pByteBuf, uint16_t usDataLen)
{
    /* �ȴ�I2C���߿��б�־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BUSYF, SET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        return I2C_ERROR_STEP_1;
    }

    /* ��ֹPOS */
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

    /* ��ʼλ */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_STARTF, RESET, I2C_EVT_CHECK_NONE, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_2;
    }

    /* �豸��ַ */
    I2C_Send7bitAddress(I2Cx, ucSlaveAddr, I2C_Direction_Transmit);
    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_ADDRF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_3;
    }

    /* ����ADDRF��־ */
    I2C_ClearADDRFlag(I2Cx);

    /* ��ַ */
    while (ucAddrLen)
    {
        /* �ȴ����ͱ�־ */
        if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_TDE, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
        {
            /* ֹͣλ */
            I2C_GenerateSTOP(I2Cx, ENABLE);
            return I2C_ERROR_STEP_4;
        }
        /* �������� */
        I2C_SendData(I2Cx, (*pDataAddr++));
        ucAddrLen--;
    }
    /* ���� */
    while (usDataLen)
    {
        /* �ȴ����ͱ�־ */
        if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_TDE, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
        {
            /* ֹͣλ */
            I2C_GenerateSTOP(I2Cx, ENABLE);
            return I2C_ERROR_STEP_5;
        }
        /* �������� */
        I2C_SendData(I2Cx, (*pByteBuf++));
        usDataLen--;
    }

    /* �ȴ�������־ */
    if (I2C_WaitOnFlagUntilTimeout(I2Cx, I2C_FLAG_BTFF, RESET, I2C_EVT_CHECK_ACKFAIL, I2C_TIMEOUT_SET) != I2C_OK)
    {
        /* ֹͣλ */
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return I2C_ERROR_STEP_6;
    }

    /* ֹͣλ */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    return I2C_OK;
}

/********************************************* End of file **********************************************/
