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
 */

#include <xc.h>
#include "gorev.h"

#if grvCALISMA_KIPI == 1
void portGorevciyiBaslat() {
  /* Kesmeleri etkinleştir */
  portGLOBAL_KESME_AC();

  /* Görevciyi başlat, buradan asla çıkmamalı */
  grvGorevciyiBaslat();
}
#endif

void portKritikBolumGirisi() {
  intconYedek = INTCON;
  if(RCON & portKESME_ONCELIK_KONTROL_MASK) {
    portCEVRESEL_KESME_KAPAT();
  }
  portGLOBAL_KESME_KAPAT();
}

void portKritikBolumCikisi() {
  if(RCON & portKESME_ONCELIK_KONTROL_MASK) {
    INTCON |=
        (intconYedek & ((1 << portGLOBAL_YUKSEK_ONCELIK_KESME_YETKI_BITI) |
                        (1 << portGLOBAL_DUSUK_ONCELIK_KESME_YETKI_BITI)));
  }
  else {
    INTCON |= intconYedek & portGLOBAL_KESME_YETKI_MASK;
  }
}
/// @}
