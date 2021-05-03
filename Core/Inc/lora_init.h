    /*!
 * \file      sx1272-board.H
 *
 * \brief     Target board SX1272 driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LORA_INIT_H__
#define __LORA_INIT_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define BOARD_WAKEUP_TIME  3 //TCXO
#define MLM_TCXO_ON()   HW_GPIO_Write( RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, 1)
#define MLM_TCXO_OFF()  HW_GPIO_Write( RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, 0)

#define RF_MID_BAND_THRESH                          525000000

/* Exported functions ------------------------------------------------------- */
/*!
 * \brief Initializes the radio I/Os pins interface
 */
void SX1272IoInit( void );

/*!
 * \brief Initializes the radio I/Os Irq
 */
void SX1272IoIrqInit( DioIrqHandler **irqHandlers );

/*!
 * \brief De-initializes the radio I/Os pins interface.
 *
 * \remark Useful when going in MCU lowpower modes
 */
void SX1272IoDeInit( void );

/*!
 * \brief Sets the radio output power.
 *
 * \param [IN] power Sets the RF output power
 */
void SX1272SetRfTxPower( int8_t power );

/*!
 * \brief Gets the board PA selection configuration
 *
 * \param [IN] channel Channel frequency in Hz
 * \retval PaSelect RegPaConfig PaSelect value
 */
uint8_t SX1272GetPaSelect( uint32_t channel );

/*!
 * \brief Set the RF Switch I/Os pins in Low Power mode
 *
 * \param [IN] status enable or disable
 */
void SX1272SetAntSwLowPower( bool status );

/*!
 * \brief Controls the antena switch if necessary.
 *
 * \remark see errata note
 *
 * \param [IN] opMode Current radio operating mode
 */
void SX1272SetAntSw( uint8_t opMode );

/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool SX1272CheckRfFrequency( uint32_t frequency );

/*!
 * Radio hardware and global parameters
 */
extern SX1272_t SX1272;


#ifdef __cplusplus
}
#endif

#endif /* __LORA_INIT_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
