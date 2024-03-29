/*
 * Copyright (C) 2023 Kozmotronik (İsmail Sahillioğlu)
 * Hiçbir ücret talep edilmeden burada işbu yazılımın bir kopyasını ve 
 * belgelendirme dosyalarını (“Yazılım”) elde eden herkese verilen izin; 
 * kullanma, kopyalama, değiştirme, birleştirme, yayımlama, dağıtma, 
 * alt lisanslama, ve/veya yazılımın kopyalarını satma eylemleri de dahil 
 * olmak üzere ve bununla kısıtlama olmaksızın, yazılımın sınırlama olmadan 
 * ticaretini yapmak için verilmiş olup, bunları yapmaları için yazılımın 
 * sağlandığı kişilere aşağıdakileri yapmak koşuluyla sunulur:
 * 
 * Yukarıdaki telif hakkı bildirimi ve işbu izin bildirimi yazılımın tüm 
 * kopyalarına veya önemli parçalarına eklenmelidir.
 * 
 * YAZILIM “HİÇBİR DEĞİŞİKLİK YAPILMADAN” ESASINA BAĞLI OLARAK, TİCARETE 
 * ELVERİŞLİLİK, ÖZEL BİR AMACA UYGUNLUK VE İHLAL OLMAMASI DA DAHİL VE BUNUNLA 
 * KISITLI OLMAKSIZIN AÇIKÇA VEYA ÜSTÜ KAPALI OLARAK HİÇBİR TEMİNAT OLMAKSIZIN 
 * SUNULMUŞTUR. 
 * HİÇBİR KOŞULDA YAZARLAR VEYA TELİF HAKKI SAHİPLERİ HERHANGİ BİR İDDİAYA, 
 * HASARA VEYA DİĞER YÜKÜMLÜLÜKLERE KARŞI, YAZILIMLA VEYA KULLANIMLA VEYA 
 * YAZILIMIN BAŞKA BAĞLANTILARIYLA İLGİLİ, BUNLARDAN KAYNAKLANAN VE BUNLARIN 
 * SONUCU BİR SÖZLEŞME DAVASI, HAKSIZ FİİL VEYA DİĞER EYLEMLERDEN SORUMLU 
 * DEĞİLDİR.
 */

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

/// Hedef porta özel NOP gerçeklemesi.
/// \hideinitializer
#define portNOP() asm("NOP")

#endif /* PORTMACRO_H */

/// @}
