/**
 * \file
 * \author İsmail Sahillioğlu (Kozmotronik)
 * \copyright \ref lisans "MIT Lisansı"
 * 
 * \defgroup mplab_xc8_pic18 MPLAB XC8 PIC18 Portu
 * \ingroup port
 * @{
 */


#ifndef PORT_H
#define PORT_H

#include "gorevciypl.h"
#include "gorev.h"

#if grvCALISMA_KIPI == 1
/**
 * Görevlerin çalışacağı sonsuz döngüyü başlatır.
 *
 * Listedeki herbir görev sırasıyla çalıştırılır. Görevler işbirlikçi
 * (cooperative) olarak çalışırlar, çalışan göreve, kendisi kontrolü teslim
 * edinceye dek müdahale edilmez.
 * Her hedef platform (port) kendi portGorevciyiBaslat() işlevini platforma
 * özel bir şekilde gerçeklemiş olduğundan, görevci normal kipte
 * kullanılacaksa, bu işlev ile başlatılmalıdır.
 */
void portGorevciyiBaslat(void);
#endif

#endif

/// @}
