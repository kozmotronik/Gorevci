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
#include "interrupt.h"

kesmeIsleyici_t timer0KesmeIsleyici, uartTxKesmeIsleyici, uartRxKesmeIsleyici;

void  interruptIlkle (void)
{
    // Kesme öncelik vektörlerini etkisizleştir (16CXXX uyumluluk kipi)
    RCONbits.IPEN = 0;
}


void timer0KesmeIsleyiciKur(kesmeIsleyici_t isleyici) {
    timer0KesmeIsleyici = isleyici;
}

void uartTxKesmeIsleyiciKur(kesmeIsleyici_t isleyici) {
    uartTxKesmeIsleyici = isleyici;
}

void uartRxKesmeIsleyiciKur(kesmeIsleyici_t isleyici) {
    uartRxKesmeIsleyici = isleyici;
}

void __interrupt() interruptYonetici (void){
    // interrupt handler
    if(INTCONbits.TMR0IF == 1 && INTCONbits.TMR0IF == 1){
        TMR0IF = 0;
        (*timer0KesmeIsleyici)();
    }
    else if(INTCONbits.PEIE == 1){
        if(PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1){
            RCIF = 0;
        }
        else if(TXIE == 1 && TXIF == 1){
            TXIF = 0;
        }
        else{
            //Unhandled Interrupt
        }
    }      
    else{
        //Unhandled Interrupt
    }
}


/**
 End of File
*/
