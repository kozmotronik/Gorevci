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
 * 
 * port.c kaynak dosyasında görevci tarafından kullanılacak sistem donanımları
 * ilklenir. Herbir portun görevcisi kendi port.c dosyasında gerçeklenmiş olan
 * gorevciyiBaslat() işlevini çağırarak başlatılır.
 */


/* Görevci başlık dosyaları */
#include <xc.h>
#include "gorev.h"

/* MPLAB kütüphanesi başlık dosyaları */
#include "interrupt.h" // Görevci için sistem kesmeleri buradan borulanır
#include "sistimer.h" // Sistem tiki üretmek için ilgili portun timer kütüphanesi

/********************************************************************
 * portable.h içinde tanımlanmış işlevlerin PIC18 portu için gerçeklemeleri
 ********************************************************************
 */

#if grvCALISMA_KIPI == 1
/*
 * Sistem tikini işlemek için kesme servisi
 */
void portGorevciyiBaslat() {
	/* Kesme modülünü ilkle */
	interruptIlkle();
	
	/* sysTikKesmeIsleyici gorev.h içerisinde tanımlıdır. */
	sisTimerIlkle(grvTikKesmeIsleyici); // systimer modülüne kesme işleyiciyi ver.
	
	/* Kesmeleri etkinleştir */
	portKESMELERI_ETKINLESTIR();
	
	/* Görevciyi başlat, buradan asla çıkmamalı */
	grvGorevciyiBaslat();
	
}
#endif

