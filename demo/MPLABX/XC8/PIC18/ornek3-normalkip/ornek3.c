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

#include "port.h"
#include "gorev.h"

// Değişkenler birden çok görevin erişebilmesi için global olarak tanımlandı.
uint16_t gercekDeger;
uint8_t karasimsekHiziOkundu, gostergeDegeriHesaplandi;
char gosterge[] = { 0, 0, 0, 0 }; // 7 parcali led göstergenin BCD haneleri
unsigned int karaSimsekHizi;

/**
 * Bu örnek görev 50 milisaniyeden 1 saniyeye sırasıyla değişen oranlarda
 * RB1e bağlı ledi çakarlar.
 */
char ledCakarlama1(gorevTutucu_t tutucu) {
    
	GOREV_BASLA(tutucu);
    
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
    LATEbits.LATE1 = 0;
    ANSELEbits.ANSE1 = 0; // Analog giriş etkisiz
    TRISEbits.TRISE1 = 0; // RE1 çıkış
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
      
        PORTEbits.RE1 = !PORTEbits.RE1;

        GOREV_GECIK_MS(tutucu, &s1, sureler[secici]);

        PORTEbits.RE1 = !PORTEbits.RE1;

        GOREV_GECIK_MS(tutucu, &s1, sureler[secici]);
        
	}
    
	GOREV_BITIR(tutucu);
}


/**
 * Bu örnek görev RE2 ucuna bağlı LEDi 3 kez 50 milisaniye çakarlar, 500
 * milisaniye bekler ve yineler.
 */
char ledCakarlama2(gorevTutucu_t tutucu) {
    
	GOREV_BASLA(tutucu);
    static sure_t s2;
    
    // RE2 ilkle
    LATEbits.LATE2 = 0;
    ANSELEbits.ANSE2 = 0; // Analog giriş etkisiz
    TRISEbits.TRISE2 = 0; // RE2 çıkış
	
    for(;;) {
       
        PORTEbits.RE2 = !PORTEbits.RE2;
            
		GOREV_GECIK_MS(tutucu, &s2, 50u);
       
        PORTEbits.RE2 = !PORTEbits.RE2;
            
		GOREV_GECIK_MS(tutucu, &s2, 50u);
       
        PORTEbits.RE2 = !PORTEbits.RE2;
            
		GOREV_GECIK_MS(tutucu, &s2, 50u);
       
        PORTEbits.RE2 = !PORTEbits.RE2;
            
		GOREV_GECIK_MS(tutucu, &s2, 50u);
       
        PORTEbits.RE2 = !PORTEbits.RE2;
            
		GOREV_GECIK_MS(tutucu, &s2, 50u);
       
        PORTEbits.RE2 = !PORTEbits.RE2;
            
		GOREV_GECIK_MS(tutucu, &s2, 1000u);
	}
    
	GOREV_BITIR(tutucu);
}

/**
 * Bu örnek PORTD ye bağlı 8 LED üzerinde basit bir karaşimşek animasyonu
 * oynatır. Animasyon hızı, AN0 ucuna bağlı bir POT ile okunan değere
 * bağlı olarak 10ms - 200ms aralığında değişir.
 */
char karaSimsek(gorevTutucu_t tutucu) {
    
	GOREV_BASLA(tutucu);
    
    static sure_t s3;
    static bool sola;
    
    // PORTD ilkle
    LATD = 0;
    ANSELD = 0; // Analog giriş etkisiz
    TRISD = 0; // PORTD çıkış
    LATD = 0x1; // İlk ledi yak
    sola = true; // İlk çalışmada sola kayacak
    karaSimsekHizi = 50; // Hızı ilkle
    
	for(;;) {
       
        if(sola){
            PORTD <<= 1;
            // Sol yönde son bite ulaştıysa yönü değiştir.
            if(PORTD == 0x80)
                sola = false;
        }
        else{
            PORTD >>= 1;
            // Sağ yönde son bite ulaştıysa yönü değiştir.
            if(PORTD == 1)
                sola = true;
        }
        
		GOREV_GECIK_MS(tutucu, &s3, karaSimsekHizi);
	}
    
	GOREV_BITIR(tutucu);
}

/**
 * Yarım saniyede bir analog girişi (POT) okuyup karaşimşek hızını günceller.
 */
char karaSimsekHiziOkuma(gorevTutucu_t tutucu){
    
    GOREV_BASLA(tutucu);
    
    static sure_t s4;
    static uint16_t oncekiSonuc = 0; // Yerel kullanım için
    
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
        BU_KOSULDA_BEKLE(tutucu, (ADCON0bits.GO));
//        while(ADCON0bits.GO); 
        uint16_t sonuc;
        sonuc = 0;
        sonuc |= (uint16_t)ADRESH << 8;
        sonuc |= (uint16_t)ADRESL;
        if(sonuc != oncekiSonuc){
            gercekDeger = sonuc;

            // Her 5'i 1 milisaniyeye ölçekle
            if(sonuc <= 50){
                //0 - 50 arası enaz değer olan 10ms
                karaSimsekHizi = 10;
            } else{
                karaSimsekHizi = sonuc / 5;
            }
            karasimsekHiziOkundu = 1; // Hesaplama için sinyal ver
            oncekiSonuc = sonuc; // Son sonucu sakla
        }
        GOREV_GECIK_MS(tutucu, &s4, 300u);
    }
    
    GOREV_BITIR(tutucu);
}

/**
 * Göstergede gösterilecek süre değerini hesaplar. Karaşimşek animasyon hızı
 * okunana dek bu görev bloklanır. Analog girişten yeni hız değeri okunduğunda
 * bu görev çalışıp 0 - 1024 değerini 10 - 200 ms süre değerine ölçekler ve
 * ardından bu değeri BCD formatında gösterge hane hücrelerine saklar.
 */
char gostergeDegeriGuncelle(gorevTutucu_t tutucu){
    
// Gösterge ölçek makroları
#define GDEGER_ENUST    1024u
#define GDEGER_ENALT    0u
#define ODEGER_ENUST    214u // 5e en yakın oran değerini elde etmek için
#define ODEGER_ENALT    10u

#define G_ENUST         (GDEGER_ENUST - GDEGER_ENALT)
#define O_ENUST         (ODEGER_ENUST - ODEGER_ENALT)
#define OLCEKLEME_ORANI (G_ENUST / O_ENUST)

    GOREV_BASLA(tutucu);
    while(1){
        KOSUL_BEKLE(tutucu, karasimsekHiziOkundu == 1);
        karasimsekHiziOkundu = 0;
        // Önce ölçekli değeri hesapla
        uint16_t olcekliDeger = (uint16_t) (gercekDeger / (uint8_t) OLCEKLEME_ORANI);
        // Sonra BCD formatına dönüştür ve sakla
        if(olcekliDeger < ODEGER_ENALT) {
            olcekliDeger = ODEGER_ENALT;
        }
        gosterge[3] = 0;
        while(olcekliDeger > 999){
            olcekliDeger -= 1000;
            gosterge[3]++;
        }
        gosterge[2] = 0;
        while(olcekliDeger > 99){
            olcekliDeger -= 100;
            gosterge[2]++;
        }
        gosterge[1] = 0;
        while(olcekliDeger > 9){
            olcekliDeger -= 10;
            gosterge[1]++;
        }
        gosterge[0] = (char)(olcekliDeger &0xFF);
    }
    GOREV_BITIR(tutucu);
}

/**
 * 7 parçalı (7 segment) LED çoğullayıcı (multiplexer) görevi
 */
char gostergeGuncelle(gorevTutucu_t tutucu){
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
    
    GOREV_BASLA(tutucu);
    
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
        GOREV_GECIK_MS(tutucu, &s5, 5u);
        
        // Onlar hanesini güncelle
        if( (gosterge[3] != 0 || gosterge[2] != 0) || gosterge[1] != 0){
            LATB &= 0xF0; // Denetim bitlerini sıfırla
            LATC = BCDdenOrtakAnoda[gosterge[1]];
            LATBbits.LATB1 = 1;
            GOREV_GECIK_MS(tutucu, &s5, 5u);
        }
        
        // Yüzler hanesini güncelle
        if(gosterge[3] != 0 || gosterge[2] != 0){
            LATB &= 0xF0; // Denetim bitlerini sıfırla
            LATC = BCDdenOrtakAnoda[gosterge[2]];
            LATBbits.LATB2 = 1;
            GOREV_GECIK_MS(tutucu, &s5, 5u);
        }
        
        // Binler hanesini güncelle
        if(gosterge[3] != 0){
            LATB &= 0xF0; // Denetim bitlerini sıfırla
            LATC = BCDdenOrtakAnoda[gosterge[3]];
            LATBbits.LATB3 = 1;
            GOREV_GECIK_MS(tutucu, &s5, 5u);
        }
        
    }
    
    GOREV_BITIR(tutucu);
}


void main(void) {
    // Genel sistemi ilkle.
    sisOsilatorIlkle();
    
    // Çalışacak görevleri ilkle.
    gorevOlustur(ledCakarlama1);
    gorevOlustur(ledCakarlama2);
    gorevOlustur(karaSimsek);
    gorevOlustur(karaSimsekHiziOkuma);
    gorevOlustur(gostergeDegeriGuncelle);
    gorevOlustur(gostergeGuncelle);
    
    /* Görevciyi başlat, görevciden asla dönmemeli */
    portGorevciyiBaslat();
    
    /* Buraya asla ulaşmamalı */
}



