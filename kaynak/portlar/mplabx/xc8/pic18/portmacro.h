/**
 * \file
 * \author İsmail Sahillioğlu (Kozmotronik)
 * \copyright \ref lisans "MIT Lisansı"
 * 
 * \ingroup mplab_xc8_pic18
 * @{
 * Mimari portuna özel tanımlamalar.
 * 
 * Bu dosyadaki tanımlamalar Görevcinin çalıştığı donanım için düzgünce
 * yapılandırılmasını sağlar. Bu yüzden bu ayarlar değiştirilmemelidir.
 */

#ifndef PORTMACRO_H
#define	PORTMACRO_H

#include <xc.h>

/// Küresel kesme yetki biti için maskeleme.
/// \hideinitializer
#define portGLOBAL_KESME_YETKI_BITI 0x80

/// Küresel kesme yetki bitini etkisizleştirir.
/// \hideinitializer
#define portKESMELERI_ETKISIZLESTIR()   INTCONbits.GIEH = 0;
/// Küresel kesme yetki bitini etkinleştirir.
/// \hideinitializer
#define portKESMELERI_ETKINLESTIR()     INTCONbits.GIEH = 1;

static unsigned char intconYedek; ///< Global kesme bitini saklamak için.


/**
 * Kritik bölüm giriş işleyicisi.
 * 
 * Kesme yöneticisini yedekleyip tüm kesmeleri etkisizleştirir. Bu işlemden
 * sonra işlemler atomik (bölünemez) olarak gerçekleştirilebilir.
 * \hideinitializer
 */
#define portKRITIK_BOLUM_GIRISI()   \
    intconYedek = INTCON;           \
    portKESMELERI_ETKISIZLESTIR()

/**
 * Kritik bölüm çıkış işleyicisi
 * 
 * Kritik bölüme girişte küresel kesme etkin idiyse yeniden etkinleştirir.
 * Bu aşamadan sonra artık atomiklik (bölünemezlik) devre dışıdır.
 * \hideinitializer
 */
#define portKRITIK_BOLUM_CIKISI()                   \
    if(intconYedek & portGLOBAL_KESME_YETKI_BITI) { \
        portKESMELERI_ETKINLESTIR();                \
    }
    
/// Hedef porta özel NOP gerçeklemesi.
/// \hideinitializer
#define portNOP() asm("NOP") 

#endif /* PORTMACRO_H */

/// @}
