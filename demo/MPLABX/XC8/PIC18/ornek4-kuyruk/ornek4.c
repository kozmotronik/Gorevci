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

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "devcfg.h"
#include "osilator.h"
#include "gorev.h"
#include "bayrak.h"
#include "port.h"
#include "uckuyruk.h"

unsigned char k1[8];
uckuyruk_t kuyruk1;

// 7 parcali led göstergenin BCD haneleri
char gosterge[] = { 0, 0, 0, 0 }; 

/* Kuyruk2' ye erişimi organize edecek bayrak (semaphore) */
bayrak_t kuyruk1Erisim;


/**
 * uretici1, 0' dan 252' ye kadar sıralı çift sayılar üretir, kuyruk1' in 
 * kilidini alır, sayıyı kuyruk1' e kuyruklar, kilidi salar ve ardından 
 * kontrolü sonraki göreve bırakır.
 */
char uretici1(gorevTutucu_t tutucu) {
    
	grvBASLA(tutucu);
    static char cift = 0;
    
    for(;;) {
        
        /* Kuyruk 1' in kilidini al. */
        grvBAYRAK_BEKLE(tutucu, &kuyruk1Erisim);
        
        grvBU_KOSULDA_BEKLE(tutucu, uckuyrukDolu(&kuyruk1));
        /* Çoklu kuyruklama başarısızsa kuyrukta yer açılmasını bekle */
        grvBU_KOSULDA_BEKLE(tutucu, uckuyrukKuyrukla(&kuyruk1, cift) == 0);
        
        /* Kuyruk 1' in kilidini sal. */
        grvBAYRAK_IMLE(&kuyruk1Erisim);
        
        cift += 2;
        if(cift >= 254) cift = 0;
        
        /* Başka görevlerin çalışması için kontrolü ver. */
        grvVAZGEC(tutucu);
	}
    
	grvBITIR(tutucu);
}

/**
 * uretici2, 1' den 253' e kadar sıralı tek sayılar üretir, kuyruk1' in 
 * kilidini alır, sayıyı kuyruk1' e kuyruklar, kilidi salar ve ardından
 * kontrolü sonraki göreve bırakır.
 */
char uretici2(gorevTutucu_t tutucu) {
    
	grvBASLA(tutucu);
    static char tek = 1;
    
    for(;;) {
        
        /* Kuyruk 1' in kilidini al. */
        grvBAYRAK_BEKLE(tutucu, &kuyruk1Erisim);
        
        grvBU_KOSULDA_BEKLE(tutucu, uckuyrukBos(&kuyruk1));
        /* Kuyruk 1 de yeteri kadar veri olmasını bekle. */
        grvBU_KOSULDA_BEKLE(tutucu, uckuyrukKuyrukla(&kuyruk1, tek) == 0);
        
        /* Kuyruk 1' in kilidini sal. */
        grvBAYRAK_IMLE(&kuyruk1Erisim);
        
        tek += 2;
        if(tek >= 255) tek = 1;
        
        /* Başka görevlerin çalışması için kontrolü ver. */
        grvVAZGEC(tutucu);
	}
    
	grvBITIR(tutucu);
}

/**
 * Bu görev uretici1 ve uretici2' den kuyruklanan sayı değerlerini 7 parçalı
 * LED göstergede göstermek için 4 hanelik BCD formatına dönüştürür. Güncelleme
 * aralığı 500 ms olduğundan ve bu görev kuyruk1' in tek tüketicisi
 * olduğundan, kuyruk1 bu süre boyunca dolu kalacak ve uretici görevler
 * bloklanacaktır.
 */
char gostergeDegerleriGuncelle(gorevTutucu_t tutucu) {
    
    grvBASLA(tutucu);
    
    static sure_t s2;
    while(1){
        grvBU_KOSULDA_BEKLE(tutucu, uckuyrukBos(&kuyruk1));
        
        unsigned b = (unsigned) uckuyrukKuyruktanAl(&kuyruk1);
        
        gosterge[3] = 0;
        while(b > 999){
            b -= 1000;
            gosterge[3]++;
        }
        gosterge[2] = 0;
        while(b > 99){
            b -= 100;
            gosterge[2]++;
        }
        gosterge[1] = 0;
        while(b > 9){
            b -= 10;
            gosterge[1]++;
        }
        gosterge[0] = (char)(b & 0xFF);
        
        grvGECIK_MS(tutucu, &s2, 500);
    }
    grvBITIR(tutucu);
}

/**
 * Basit bir 4 x 7 parçalı LED gösterge sürme görevidir. Dört haneyi 5 
 * milisaniyede bir çoğullar. Bu uygulamada göstergede yarımşar saniye
 * aralıklarla sırasıyla 0 - 253 değerleri görülür.
 */
char gostergeTara(gorevTutucu_t tutucu) {
#define GOSTERGE_ORTAK_ANOT 1
    
#if GOSTERGE_ORTAK_ANOT == 1
    static const uint8_t BCDdenOrtakAnoda[] = {
      0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80,
      0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xBF
    };
#else
    static const uint8_t BCDdenOrtakKatoda[] = {
      0x3F, 0x6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F,
      0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x40
    };
#endif
    
    static sure_t s5;
    
    grvBASLA(tutucu);
    
    // PORTC ilkle
    LATC = 0;
    ANSELC = 0; // Analog giriş etkisiz
    TRISC = 0; // PORTC çıkış
    
    // B0 B1 B2 B3 Denetim uçlarını ilkle
    LATB = 0;
    ANSELB = 0xF0;
    TRISB = 0xF0;
    
    while(1){
        // Birler hanesini güncelle
        LATB &= 0xF0; // Denetim bitlerini sıfırla
        LATC = BCDdenOrtakAnoda[gosterge[0]];
        LATBbits.LATB0 = 1;
        grvGECIK_MS(tutucu, &s5, 5u);
        
        // Onlar hanesini güncelle
        if( (gosterge[3] != 0 || gosterge[2] != 0) || gosterge[1] != 0){
            LATB &= 0xF0; // Denetim bitlerini sıfırla
            LATC = BCDdenOrtakAnoda[gosterge[1]];
            LATBbits.LATB1 = 1;
            grvGECIK_MS(tutucu, &s5, 5u);
        }
        
        // Yüzler hanesini güncelle
        if(gosterge[3] != 0 || gosterge[2] != 0){
            LATB &= 0xF0; // Denetim bitlerini sıfırla
            LATC = BCDdenOrtakAnoda[gosterge[2]];
            LATBbits.LATB2 = 1;
            grvGECIK_MS(tutucu, &s5, 5u);
        }
        
        // Binler hanesini güncelle
        if(gosterge[3] != 0){
            LATB &= 0xF0; // Denetim bitlerini sıfırla
            LATC = BCDdenOrtakAnoda[gosterge[3]];
            LATBbits.LATB3 = 1;
            grvGECIK_MS(tutucu, &s5, 5u);
        }
        
    }
    
    grvBITIR(tutucu);
}


void main(void) {
    // Sistem osilatörünü ilkle.
    sisOsilatorIlkle();
    
    uckuyrukIlkle((unsigned char)sizeof(k1), &kuyruk1, k1);
    
    /* Senkronizasyon bayrağını ilkle. */
    /* Aynı anda yalnızca bir görev erişebilir. */
    grvBAYRAK_ILKLE(&kuyruk1Erisim, 1); 
    
    /* Çalışacak görevleri görevciye tanıt. Çalışacak görevlerin sayısı belli
     * olduktan sonra gorevciypl.h dosyasında "MAX_GOREV_SAYISI" bu görev sayısı
     * kadar veya bir fazlası olarak tanımlanmalıdır. Bu sayı tanımlanmadığı
     * takdirde görevci katmanı varsayılan olarak 8 görev tutacak bir görev
     * sayısı tanımlayacaktır.
     */
    grvOlustur(uretici1);
    grvOlustur(uretici2);
    grvOlustur(gostergeDegerleriGuncelle);
    grvOlustur(gostergeTara);
    
    // Görevciyi çalıştır, bu çağrıdan geri dönmemeli.
    portGorevciyiBaslat();
    
    // Buraya asla ulaşmamalı.
}



