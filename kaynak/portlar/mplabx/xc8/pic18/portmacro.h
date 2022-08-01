/**
 * Görevci v1.0.0
 * Copyright (C) 2021 İsmail Sahillioğlu (aka Kozmotronik). Tüm Hakları Saklıdır.
 * Hiçbir ücret talep edilmeden burada işbu* yazılımın bir kopyasını ve belgelendirme
 * dosyalarını (“Yazılım”) elde eden herkese verilen izin; kullanma, kopyalama,
 * değiştirme, birleştirme, yayımlama, dağıtma, alt lisanslama, ve/veya yazılımın
 * kopyalarını satma eylemleri de dahil olmak üzere ve bununla kısıtlama olmaksızın,
 * yazılımın sınırlama olmadan ticaretini yapmak için verilmiş olup, bunları yapmaları
 * için yazılımın sağlandığı kişilere aşağıdakileri yapmak koşuluyla sunulur:
 * 
 * Yukarıdaki telif hakkı bildirimi ve işbu izin bildirimi yazılımın tüm kopyalarına
 * veya önemli parçalarına eklenmelidir.
 * 
 * YAZILIM “HİÇBİR DEĞİŞİKLİK YAPILMADAN” ESASINA BAĞLI OLARAK, TİCARETE ELVERİŞLİLİK,
 * ÖZEL BİR AMACA UYGUNLUK VE İHLAL OLMAMASI DA DAHİL VE BUNUNLA KISITLI OLMAKSIZIN
 * AÇIKÇA VEYA ÜSTÜ KAPALI OLARAK HİÇBİR TEMİNAT OLMAKSIZIN SUNULMUŞTUR. HİÇBİR KOŞULDA
 * YAZARLAR VEYA TELİF HAKKI SAHİPLERİ HERHANGİ BİR İDDİAYA, HASARA VEYA DİĞER
 * YÜKÜMLÜLÜKLERE KARŞI, YAZILIMLA VEYA KULLANIMLA VEYA YAZILIMIN BAŞKA BAĞLANTILARIYLA
 * İLGİLİ, BUNLARDAN KAYNAKLANAN VE BUNLARIN SONUCU BİR SÖZLEŞME DAVASI, HAKSIZ FİİL
 * VEYA DİĞER EYLEMLERDEN SORUMLU DEĞİLDİR.
 ***************************************************************************************
 * 
 */

#ifndef PORTMACRO_H
#define	PORTMACRO_H

/*----------------------------------------------------------------
 * Mimari portuna özel tanımlamalar.
 * 
 * Bu dosyadaki tanımlamalar Görevcinin çalıştığı donanım için
 * düzgünce yapılandırılmasını sağlar. Bu yüzden bu ayarlar
 * değiştirilmemelidir.
 *----------------------------------------------------------------
 */
#include <xc.h>

/* Donanımla ilgili tanımlamalar. */
#define portGLOBAL_KESME_YETKI_BITI			0x80

/* Kritik bölüm yönetimi */
#define portKESMELERI_ETKISIZLESTIR()       INTCONbits.GIEH = 0;
#define portKESMELERI_ETKINLESTIR()         INTCONbits.GIEH = 1;

static unsigned char intconYedek; // Global kesme bitini saklamak için
/* INTCON yazmacını yığına it, sonra kesmeleri etkisizleştir. */
#define portKRITIK_BOLUM_GIRISI()           intconYedek = INTCON;	\
											portKESMELERI_ETKISIZLESTIR()

/* INTCON yazmacını yığından al, ve kesmeler kaydedilirken etkin idiyse
 * yeniden etkinleştir. INTCON yazmacının herhangi bir diğer bitini;
 * kritik bölgede değiştirilmiş olma olasılığına karşın değiştirme. */
#define portKRITIK_BOLUM_CIKISI()           if(intconYedek & portGLOBAL_KESME_YETKI_BITI){	\
												portKESMELERI_ETKINLESTIR();		\
											}

#define portNOP()							asm("NOP")

#endif	/* PORTMACRO_H */

