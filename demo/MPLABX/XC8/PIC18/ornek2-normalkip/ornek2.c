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

// Gösterge ölçek makroları
#define GDEGER_ENUST    1024u
#define GDEGER_ENALT    50u
#define ODEGER_ENUST    7u
#define ODEGER_ENALT    0u

#define G_ENUST         (GDEGER_ENUST - GDEGER_ENALT)
#define O_ENUST         (ODEGER_ENUST - ODEGER_ENALT)
#define OLCEKLEME_ORANI (G_ENUST / O_ENUST)

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "devcfg.h"
#include "osilator.h"
#include "interrupt.h"
#include "sistimer.h"
#include "gorev.h"
#include "port.h"

#define KARASIMSEK  10
#define GOSTERGE  20
    
// Değişkenler birden çok görevin erişebilmesi için global olarak tanımlandı.
char icerik = KARASIMSEK, icerikDegisti = 0;
uint8_t  karasimsek = 0, tamponD = 0;
uint16_t gercekDeger;
uint8_t karasimsekGostergeDegeri = 0;
uint8_t karasimsekHiziOkundu, gostergeDegeriHesaplandi;
unsigned int karaSimsekHizi;

/**
 * Bu örnek görev 50 milisaniyeden 1 saniyeye sırasıyla değişen oranlarda
 * RB1e bağlı ledi çakarlar.
 */
char ledCakarlama1(gorevTutucu_t tutucu) {
    
	grvBASLA(tutucu);
    
    /*
     * Bir görev aynı anda yalnızca tek bir gecikme işleyebileceğinden tüm
     * gecikmeler için aynı sure_t yapısını kullanabiliriz.
     */
    static sure_t s1;
    static char secici;
    static bool artir;
    
    // Milisaniye türünden süre aralıkları
    static const unsigned int sureler[] = {
        50, 60, 70, 80, 90, 100, 200, 300,
        400, 500, 600, 700, 800, 900, 1000
    };
    // Dizinin son indeks sayısını hesapla.
    static const char sonSira = sizeof(sureler) / sizeof(unsigned int) - 1;
    
    // RB1 ilkle
    LATBbits.LATB1 = 0;
    ANSELBbits.ANSB1 = 0; // Analog giriş etkisiz
    TRISBbits.TRISB1 = 0; // RB1 çıkış
    secici = 0;
    artir = true;
    
	for(;;) {
       
        if(artir){
            if(secici == sonSira){
                artir = false;
            } else{
                secici++;
            }
        } else{
            if(secici == 0){
                artir = true;
            } else{
                secici--;
            }
        }
      
        PORTBbits.RB1 = !PORTBbits.RB1;

        grvGECIK_MS(tutucu, &s1, sureler[secici]);

        PORTBbits.RB1 = !PORTBbits.RB1;

        grvGECIK_MS(tutucu, &s1, sureler[secici]);
        
	}
    
	grvBITIR(tutucu);
}


/**
 * Bu örnek görev RB5 ucuna bağlı LEDi 3 kez 50 milisaniye çakarlar, 500
 * milisaniye bekler ve yineler.
 */
char ledCakarlama2(gorevTutucu_t tutucu) {
    
	grvBASLA(tutucu);
    static sure_t s2;
    
    // RB5 ilkle
    LATBbits.LATB5 = 0;
    ANSELBbits.ANSB5 = 0; // Analog giriş etkisiz
    TRISBbits.TRISB5 = 0; // RB5 çıkış
	
    for(;;) {
       
        PORTBbits.RB5 = !PORTBbits.RB5;
            
		grvGECIK_MS(tutucu, &s2, 50u);
       
        PORTBbits.RB5 = !PORTBbits.RB5;
            
		grvGECIK_MS(tutucu, &s2, 50u);
       
        PORTBbits.RB5 = !PORTBbits.RB5;
            
		grvGECIK_MS(tutucu, &s2, 50u);
       
        PORTBbits.RB5 = !PORTBbits.RB5;
            
		grvGECIK_MS(tutucu, &s2, 50u);
       
        PORTBbits.RB5 = !PORTBbits.RB5;
            
		grvGECIK_MS(tutucu, &s2, 50u);
       
        PORTBbits.RB5 = !PORTBbits.RB5;
            
		grvGECIK_MS(tutucu, &s2, 1000u);
	}
    
	grvBITIR(tutucu);
}



/**
 * Bu örnek PORTD ye bağlı 8 LED üzerinde basit bir karaşimşek animasyonu
 * oynatır. Animasyon hızı, AN0 ucuna bağlı bir POT ile okunan değere
 * bağlı olarak 10ms - 200ms aralığında değişir.
 */
char karaSimsek(gorevTutucu_t tutucu) {
    
	grvBASLA(tutucu);
    
    static sure_t s3;
    static bool sola;
    
    karasimsek = 1; // İlk biti etkinleştir.
    sola = true; // İlk çalışmada sola kayacak
    karaSimsekHizi = 50; // Hızı ilkle
    
	for(;;) {
        // Karaşimşeğin içeriği gösterilmiyorsa blokla
        grvKOSUL_BEKLE(tutucu, icerik == KARASIMSEK);
        if(sola){
            karasimsek <<= 1;
            // Sol yönde son bite ulaştıysa yönü değiştir.
            if(karasimsek == 0x80)
                sola = false;
        }
        else{
            karasimsek >>= 1;
            // Sağ yönde son bite ulaştıysa yönü değiştir.
            if(karasimsek == 1)
                sola = true;
        }
        if(icerik == KARASIMSEK) icerikDegisti = 1;
        
		grvGECIK_MS(tutucu, &s3, karaSimsekHizi);
	}
    
	grvBITIR(tutucu);
}

/**
 * Yarım saniyede bir analog girişi (POT) okuyup karaşimşek hızını günceller.
 * @param isleyici
 */
char karaSimsekHiziOkuma(gorevTutucu_t tutucu){
    
    grvBASLA(tutucu);
    
    static sure_t s4;
    
    // AN0 kanalı için ADC modülü kurulumu yap.
    PIE1bits.ADIE = 0; // Analog kesmesi etkisiz
    PIR1bits.ADIF = 0;
    ADCON2 = 0xAF; // Sağa hizalı, FRC, 12TAD
    ADCON1 = 0; // Ref=Vdd,Vss
    TRISAbits.TRISA0 = 1; // A0 giriş
    ADCON0bits.ADON = 1; // ADC etkin
    
    for(;;){
        
        ADCON0bits.GO = 1; // Dönüşümü başlat
        // Dönüşümün bitmesini bekle (GO/DONE biti sıfır olana dek)
        grvBU_KOSULDA_BEKLE(tutucu, (ADCON0bits.GO));
        
        uint16_t sonuc;
        sonuc = 0;
        sonuc |= (uint16_t)ADRESH << 8;
        sonuc |= (uint16_t)ADRESL;
        gercekDeger = sonuc;
        
        // Her 5'i 1 milisaniyeye ölçekle
        if(sonuc <= 50){
            //0 - 50 arası enaz değer olan 10ms
            karaSimsekHizi = 10;
        } else{
            karaSimsekHizi = sonuc / 5;
        }
        karasimsekHiziOkundu = 1; // Hesaplama için sinyal ver
        
        grvGECIK_MS(tutucu, &s4, 500u);
    }
    
    grvBITIR(tutucu);
}

char gostergeDegeriHesapla(gorevTutucu_t tutucu){
    grvBASLA(tutucu);
    while(1){
        // Karaşimşek hızının seviye göstergesi gösterilmiyorsa blokla.
        grvKOSUL_BEKLE(tutucu, icerik == GOSTERGE);
        grvKOSUL_BEKLE(tutucu, karasimsekHiziOkundu == 1);
        karasimsekHiziOkundu = 0;
        karasimsekGostergeDegeri = (uint8_t) (gercekDeger / (uint8_t) OLCEKLEME_ORANI);
        icerikDegisti = 1;
    }
    grvBITIR(tutucu);
}

char icerikGuncelleyici(gorevTutucu_t tutucu){
#define DUZEY_7 0xFF
#define DUZEY_6 0x7F
#define DUZEY_5 0x3F
#define DUZEY_4 0x1F
#define DUZEY_3 0xF
#define DUZEY_2 0x7
#define DUZEY_1 0x3
#define DUZEY_0 0x1
    
    grvBASLA(tutucu);
    
    // PORTD ilkle
    LATD = 0;
    ANSELD = 0; // Analog giriş etkisiz
    TRISD = 0; // PORTD çıkış
    LATD = 0x1; // İlk ledi yak
    
    while(1){
        grvKOSUL_BEKLE(tutucu, icerikDegisti == 1);
        icerikDegisti = 0;
        if(icerik == KARASIMSEK){
            LATD = karasimsek;
        } else if(icerik == GOSTERGE){
            switch(karasimsekGostergeDegeri){
                case 0: LATD = DUZEY_7; break;
                case 1: LATD = DUZEY_6; break;
                case 2: LATD = DUZEY_5; break;
                case 3: LATD = DUZEY_4; break;
                case 4: LATD = DUZEY_3; break;
                case 5: LATD = DUZEY_2; break;
                case 6: LATD = DUZEY_1; break;
                case 7: LATD = DUZEY_0; break;
            }
        }
    }
    
    grvBITIR(tutucu);
}

char icerikDegistirici(gorevTutucu_t tutucu){
    static sure_t s5;
    
    grvBASLA(tutucu);
    
    // B4 ucunu giriş olarak ilkle
    ANSELBbits.ANSB4 = 0;
    LATBbits.LATB4 = 0;
    TRISBbits.TRISB4 = 1; // B4 ucu giriş
    
    while(1){
        grvKOSUL_BEKLE(tutucu, PORTBbits.RB4 == 0); // Tuş basılı değilken blokla
        grvGECIK_MS(tutucu, &s5, 100u); // Arkı süz
        /* Giriş kararsızsa yeniden örnekle */
        if(PORTBbits.RB4 != 0) continue;
        
        /* Tuş basımı saptandı çıktının içeriğini değiştir */
        
        /* İçeriği gösterge / karaşimşek içeriğiyle değiştir. */
        icerik = icerik == KARASIMSEK ? GOSTERGE : KARASIMSEK;
        
        /* İçeriğin sürekli değişmemesi için tuşun bırakılmasını bekle */
        grvBU_KOSULDA_BEKLE(tutucu, PORTBbits.RB4 == 0);
    }
    
    grvBITIR(tutucu);
}


void main(void) {
    // Sistemi osilatörünü ilkle.
    sisOsilatorIlkle();
    
    /* Görevleri ilkle */
    grvOlustur(ledCakarlama1);
    grvOlustur(ledCakarlama2);
    grvOlustur(karaSimsek);
    grvOlustur(karaSimsekHiziOkuma);
    grvOlustur(gostergeDegeriHesapla);
    grvOlustur(icerikGuncelleyici);
    grvOlustur(icerikDegistirici);
    
    /* Görevleyiciyi başlat, bu çağrıdan asla geri dönmemeli */
    portGorevciyiBaslat();
    
    /* Buraya asla ulaşmamalı */
}



