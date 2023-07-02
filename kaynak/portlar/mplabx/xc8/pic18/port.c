/**
 * \file
 * \author İsmail Sahillioğlu (Kozmotronik)
 * \copyright \ref lisans "MIT Lisansı"
 *
 * \ingroup mplab_xc8_pic18
 * @{
 */

#include "gorev.h"
#include <xc.h>

/* MPLAB kütüphanesi başlık dosyaları */
#include "interrupt.h" // Görevci için sistem kesmeleri buradan borulanır
#include "sistimer.h" // Sistem tiki üretmek için ilgili portun timer kütüphanesi

#if grvCALISMA_KIPI == 1
void portGorevciyiBaslat()
{
    /* grvTikKesmeIsleyici gorev.h içerisinde tanımlıdır. */
    sisTimerIlkle(grvTikKesmeIsleyici);

    /* Kesmeleri etkinleştir */
    portKESMELERI_ETKINLESTIR();

    /* Görevciyi başlat, buradan asla çıkmamalı */
    grvGorevciyiBaslat();
}
#endif
/// @}
