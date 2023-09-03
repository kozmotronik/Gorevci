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
#define PORTMACRO_H

#include <xc.h>

/// Kesme öncelik kontrol biti numarası.
#define portKESME_ONCELIK_KONTROL_BITI 0x7
/// Kesme öncelik kontrol biti için maskeleme.
#define portKESME_ONCELIK_KONTROL_MASK 0x80
/// Küresel kesme yetki biti numarası.
#define portGLOBAL_KESME_YETKI_BITI 0x7
/// Küresel kesme yetki biti için maskeleme.
#define portGLOBAL_KESME_YETKI_MASK 0x80
/// Çevresel kesme yetki biti numarası.
#define portCEVRESEL_KESME_YETKI_BITI 0x6
/// Çevresel kesme yetki biti için maskeleme.
#define portCEVRESEL_KESME_YETKI_MASK 0x40
/// Yükesk öncelikli kesme yetki biti numarası.
#define portGLOBAL_YUKSEK_ONCELIK_KESME_YETKI_BITI 0x7
/// Yüksek öncelikli kesme yetki biti için maskeleme.
#define portGLOBAL_YUKSEK_ONCELIK_KESME_YETKI_MASK 0x80
/// Düşük öncelikli kesme yetki biti numarası.
#define portGLOBAL_DUSUK_ONCELIK_KESME_YETKI_BITI 0x6
/// Düşük öncelikli kesme yetki biti için maskeleme.
#define portGLOBAL_DUSUK_ONCELIK_KESME_YETKI_MASK 0x40

/// Küresel kesme yetki bitini etkisizleştirir.
#define portGLOBAL_KESME_KAPAT() INTCON &= ~(1 << portGLOBAL_KESME_YETKI_BITI)
/// Küresel kesme yetki bitini etkinleştirir.
#define portGLOBAL_KESME_AC() INTCON |= 1 << portGLOBAL_KESME_YETKI_BITI
/// Çevresel kesme yetki bitini etkisizleştirir.
#define portCEVRESEL_KESME_KAPAT() INTCON &= ~(1 << portCEVRESEL_KESME_YETKI_BITI)
/// Çevresel kesme yetki bitini etkinleştirir.
#define portCEVRESEL_KESME_AC() INTCON |= 1 << portCEVRESEL_KESME_YETKI_BITI

/**
 * Kritik bölüm giriş işleyicisi.
 *
 * Kesme yöneticisini yedekleyip tüm kesmeleri etkisizleştirir. Bu işlemden
 * sonra işlemler atomik (bölünemez) olarak gerçekleştirilebilir.
 * \hideinitializer
 */
#define portKRITIK_BOLUM_GIRISI() \
    intconYedek = INTCON;         \
    portKESMELERI_ETKISIZLESTIR()

/**
 * Kritik bölüm çıkış işleyicisi
 *
 * Kritik bölüme girişte küresel kesme etkin idiyse yeniden etkinleştirir.
 * Bu aşamadan sonra artık atomiklik (bölünemezlik) devre dışıdır.
 * \hideinitializer
 */
#define portKRITIK_BOLUM_CIKISI()                    \
    if (intconYedek & portGLOBAL_KESME_YETKI_BITI) { \
        portKESMELERI_ETKINLESTIR();                 \
    }

/// Hedef porta özel NOP gerçeklemesi.
/// \hideinitializer
#define portNOP() asm("NOP")

#endif /* PORTMACRO_H */

/// @}
