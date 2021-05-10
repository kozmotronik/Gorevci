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

#include <xc.h>
#include <stdint.h>

#include "interrupt.h"
#include "sistimer.h"
#include "devcfg.h"
#include "gorevciypl.h"


#ifdef SISTEM_TIK_SURESI_us
#define SIS_TIMER_SISTEM_TIK_SURESI_uS SISTEM_TIK_SURESI_us
#else
#define SIS_TIMER_SISTEM_TIK_SURESI_uS (1000)
#endif

#ifndef SIS_TIMER_16
#define SIS_TIMER_16 1
#endif

#if(SIS_TIMER_16 == 1)
#define SIS_TIMER_ENCOK (65536)
#else
#define SIS_TIMER_ENCOK (256)
#endif

#define SIS_TIMER_ONOLCEKCI_ENCOK (256)

/* Kullanılan timer donanımının yaratabileceği ençok sürenin us türünden hesabı */
#define SIS_TIMER_ONOLCEKSIZ_ENCOK_SURE (((_XTAL_FREQ / 1000000) / 4) * SIS_TIMER_ENCOK)
#define SIS_TIMER_ONOLCEKLI_ENCOK_SURE (((_XTAL_FREQ / 1000000) / 4) * (SIS_TIMER_ENCOK * SIS_TIMER_ONOLCEKCI_ENCOK))

/* Kesme vektöründen tik sayımını yapmasına kadar çalışan komut sayısı simülatörde
 * 61 komut olarak ölçüldü. Bu komut süresini mikrosaniye türünden hesaplayıp;
 * tik sayımının olabildiğince sistem tiki süresine yakın olması için sistem tik
 * süresinden düşeceğiz. Deneysel deneme gözlemlerden sonra bu gecikme komut
 * çevrimi sayısı 52 olarak belirlendi. Farklı derleme optimizasyonlarında
 * farklı sonuçlar elde edilebilir. Bu değer ile hesaplandığında her tik
 * periyodu simülatörde 1,00075 milisaniyede bir oluşmakta, yani 75 nanosaniyelik
 * gözardı edilebilir bir fark.
 */
#define TIK_KESME_GECIKME_KOMUT_SAYISI  ( 52 )
#define KATSAYI                 ( 100 ) // Komut süresini yuvarlamak için
#define KOMUT_SURESI_uS         ( ( 1 * KATSAYI ) / ( (_XTAL_FREQ / 4 ) / 1000000 ) )
#define TIK_ONCESI_GECIKME_uS   ( ( TIK_KESME_GECIKME_KOMUT_SAYISI * KOMUT_SURESI_uS ) / KATSAYI )
#define GERCEK_TIK_SURESI       ( SIS_TIMER_SISTEM_TIK_SURESI_uS - TIK_ONCESI_GECIKME_uS )

#if GERCEK_TIK_SURESI > SIS_TIMER_ONOLCEKLI_ENCOK_SURE
#error Sistem saati icin istenen sure cok buyuk
#endif

/* Önölçekçi oranının hesaplanması */
#if GERCEK_TIK_SURESI < SIS_TIMER_ONOLCEKSIZ_ENCOK_SURE
#define SIS_TIMER_ONOLCEKCI_ORANI (1)
#else
#define SIS_TIMER_ONOLCEKCI_ORANI (GERCEK_TIK_SURESI / SIS_TIMER_ONOLCEKSIZ_ENCOK_SURE)
#endif

#if SIS_TIMER_ONOLCEKCI_ORANI == 1
#define SIS_TIMER_ONOLCEKCI (1)
#define SIS_TIMER_ONOLCEKCI_DEGER (-1)
#elif SIS_TIMER_ONOLCEKCI_ORANI > 1
#define SIS_TIMER_ONOLCEKCI (2)
#define SIS_TIMER_ONOLCEKCI_DEGER (0)
#elif SIS_TIMER_ONOLCEKCI_ORANI > 2
#define SIS_TIMER_ONOLCEKCI (4)
#define SIS_TIMER_ONOLCEKCI_DEGER (1)
#elif SIS_TIMER_ONOLCEKCI_ORANI > 4
#define SIS_TIMER_ONOLCEKCI (8)
#define SIS_TIMER_ONOLCEKCI_DEGER (2)
#elif SIS_TIMER_ONOLCEKCI_ORANI > 8
#define SIS_TIMER_ONOLCEKCI (16)
#define SIS_TIMER_ONOLCEKCI_DEGER (3)
#elif SIS_TIMER_ONOLCEKCI_ORANI > 16
#define SIS_TIMER_ONOLCEKCI (32)
#define SIS_TIMER_ONOLCEKCI_DEGER (4)
#elif SIS_TIMER_ONOLCEKCI_ORANI > 32
#define SIS_TIMER_ONOLCEKCI (64)
#define SIS_TIMER_ONOLCEKCI_DEGER (5)
#elif SIS_TIMER_ONOLCEKCI_ORANI > 64
#define SIS_TIMER_ONOLCEKCI (128)
#define SIS_TIMER_ONOLCEKCI_DEGER (6)
#elif SIS_TIMER_ONOLCEKCI_ORANI > 128
#define SIS_TIMER_ONOLCEKCI (256)
#define SIS_TIMER_ONOLCEKCI_DEGER (7)
#endif

    /* Timer öndeğerinin hesabı */
#define SIS_TIMER_TMR0_ONDEGER                                  \
(uint16_t)(SIS_TIMER_ENCOK-((GERCEK_TIK_SURESI*    \
(_XTAL_FREQ/1000000))/(4*SIS_TIMER_ONOLCEKCI))-1)

static uint8_t tmr0OnDeger; // 1ms için sabitle
kesmeIsleyici_t sisTimerDisIsleyici;


/* İşleyicinin NULL olmasına önlem olarak bir dummy işleyici */
void sisTimerIsleyiciVarsayilan(void){
    // Hiçbir şey yapma
}

void sisTimerKesmeIsleyici(void){
#if SIS_TIMER_16 == 1    
    TMR0 = (uint16_t) (SIS_TIMER_TMR0_ONDEGER);
#else
    TMR0L = (uint8_t) (SIS_TIMER_TMR0_ONDEGER);
#endif
    (*sisTimerDisIsleyici)();
}

void sisTimerIlkle(kesmeIsleyici_t isleyici){
    T0CON = 0;
#if SIS_TIMER_16 == 1
    T0CONbits.T08BIT = 0; // TMR0 16 bit.
#else
    T0CONbits.T08BIT = 1; // TMR0 8 bit.
#endif
    
    T0CONbits.T0CS = 0; // Saat kaynağı dahili komut saati.
#if SIS_TIMER_ONOLCEKCI_DEGER == -1
    // Önölçekçi sürevciye atanmadı saat sinyali baypas edilir, oran 1:1
    T0CONbits.PSA = 1;
#else
    // Önölçekçi sürevciye atandı, oran ssONOLCEKCI önişlem değişkenindeki değer
    T0CONbits.PSA = 0;
    T0CON &= ~((uint8_t) SIS_TIMER_ONOLCEKCI_DEGER);
    T0CON |= (uint8_t) SIS_TIMER_ONOLCEKCI_DEGER;
#endif
    
#if SIS_TIMER_16 == 1    
    TMR0 = (uint16_t) (SIS_TIMER_TMR0_ONDEGER);
#else
    TMR0L = (uint8_t) (SIS_TIMER_TMR0_ONDEGER);
#endif
    
    /* Timer kesmesinde çağrılacak işleyiciyi kaydet */
    sisTimerDisIsleyici = isleyici == NULL ? sisTimerIsleyiciVarsayilan : isleyici;
    // Timer0 kesme çağrısına kaydol.
    timer0KesmeIsleyiciKur(sisTimerKesmeIsleyici);
    
    // Timer0 kesmesini kur
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    
    // Timer0 ı aç
    T0CONbits.TMR0ON = 1;
}