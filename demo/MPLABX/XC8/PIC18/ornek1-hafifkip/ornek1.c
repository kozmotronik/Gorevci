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
#include "interrupt.h"
#include "sistimer.h"
#include "port.h"
#include "gorev.h"


// Bu değişkene birden fazla görev erişeceği için global kapsamda tanımlandı.
unsigned int karaSimsekHizi;

/**
 * Bu örnek görev 50 milisaniyeden 1 saniyeye sırasıyla değişen oranlarda
 * RB1 ucuna bağlı bir LEDi çakarlar.
 */
char ledCakarlama1(gorevTutucu_t tutucu) {
    
	grvBASLA(tutucu);
    
    /* Sistem gecikmesi kullanılacak her görevde @sure_t türünde bir süre
     * tutucu değişken tanımlanmalıdır. Görevci bu değişkeni süre takibini
     * yapmak için kullanır.
     * Bir görev aynı anda yalnızca tek bir gecikme işleyebileceğinden tüm
     * gecikmeler için aynı sure_t yapısını kullanabiliriz.
     */
    static sure_t s1;
    
    static char secici;
    static bool artir;
    
    // Milisaniye türünden süre aralıkları
    const unsigned int sureler[] = {
        50, 60, 70, 80, 90, 100, 200, 300,
        400, 500, 600, 700, 800, 900, 1000
    };
    // Dizinin son indeks sayısını hesapla.
    const char sonSira = sizeof(sureler) / sizeof(unsigned int) - 1;
    
    // RB1 ucunu dijital çıkış olarak ilkle
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

        /* Süre dolana dek bloklanır */
        grvGECIK_MS(tutucu, &s1, sureler[secici]);

        PORTBbits.RB1 = !PORTBbits.RB1;

        /* Süre dolana dek bloklanır */
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
    
    /* Görevdeki gecikmeler için süre tutucu */
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
        
        /* Bu görev burada karasimsekHizi içerisindeki milisaniye süre değeri
         * kadar sure bloklanacaktır.
         */
		grvGECIK_MS(tutucu, &s3, karaSimsekHizi);
	}
    
	grvBITIR(tutucu);
}

/**
 * 350 milisaniyede bir AN0 girişini (POT) okur, dönüşüm değerini 10 - 200ms 
 * süre aralığına ölçekler ve karaşimşek animasyon hızını günceller.
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
        /* Dönüşüm bitene dek (GO/DONE biti sıfır olana dek) görev bloklanır
         * Alternatif olarak ADIF bayrağı da yoklanabilir. Bu yapılırsa ADIF
         * bayrağı, yoklandıktan ve koşulu geçtikten sonra temizlenmelidir. */
        grvBU_KOSULDA_BEKLE(tutucu, (ADCON0bits.GO));
        
        /* Dönüşüm bitti, dönüşüm değeriyle yeni animasyon hızını hesapla */
        uint16_t sonuc;
        sonuc = (uint16_t)(ADRESH << 8 | ADRESL);
        
        // Her 5'i 1 milisaniyeye ölçekle
        if(sonuc <= 50){
            //0 - 50 arası enaz değer olan 10ms
            karaSimsekHizi = 10;
        } else{
            karaSimsekHizi = sonuc / 5;
        }
        /* Bir sonraki dönüştürme ve hesaplama 350ms sonra yapılacak. O zamana
         * dek görev bloklanır.
         */
        grvGECIK_MS(tutucu, &s4, 350);
    }
    
    grvBITIR(tutucu);
}


void main(void) {
    // Sistem osilatörünü ilkle.
    sisOsilatorIlkle();
    
	/* Kesme modülünü ilkle */
	interruptIlkle();
	
	/* sysTikKesmeIsleyici gorev.h içerisinde tanımlıdır. */
	sisTimerIlkle(grvTikKesmeIsleyici); // systimer modülüne kesme işleyiciyi ver.
	
	/* Kesmeleri etkinleştir */
	portKESMELERI_ETKINLESTIR();
    
    /* Çalışacak görevler için görev tutucuları oluştur ve görevleri ilkle */
    gorev_t lc1, lc2, ksmsk, kho;
    
    grvILKLE(&lc1);
    grvILKLE(&lc2);
    grvILKLE(&ksmsk);
    grvILKLE(&kho);
    
    // Görevleri çalıştır.
    while(1) {
        ledCakarlama1(&lc1);
        ledCakarlama2(&lc2);
        karaSimsek(&ksmsk);
        karaSimsekHiziOkuma(&kho);
    }
    
    // Buraya asla ulaşmamalı.
}



