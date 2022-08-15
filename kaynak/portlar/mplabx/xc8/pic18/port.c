/**
 * \file
 * \author İsmail Sahillioğlu (Kozmotronik)
 * \copyright \ref lisans "MIT Lisansı"
 * 
 * \ingroup mplab_xc8_pic18
 * @{
 *
 * Görevci tarafından kullanılacak sistem donanımları burada ilklenir. Herbir
 * portun görevcisi kendi port.c dosyasında gerçeklenmiş olan
 * gorevciyiBaslat() işlevini çağırarak başlatılır.
 */


#include <xc.h>
#include "gorev.h"

/* MPLAB kütüphanesi başlık dosyaları */
#include "interrupt.h" // Görevci için sistem kesmeleri buradan borulanır
#include "sistimer.h" // Sistem tiki üretmek için ilgili portun timer kütüphanesi


#if grvCALISMA_KIPI == 1
/*
 * Sistem tikini işlemek için kesme servisi
 */
void portGorevciyiBaslat() {
    /* Kesme modülünü ilkle */
    interruptIlkle();

    /* grvTikKesmeIsleyici gorev.h içerisinde tanımlıdır. */
    sisTimerIlkle(grvTikKesmeIsleyici);

    /* Kesmeleri etkinleştir */
    portKESMELERI_ETKINLESTIR();

    /* Görevciyi başlat, buradan asla çıkmamalı */
    grvGorevciyiBaslat();
}
#endif
/// @}
