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
 * \defgroup mplab_xc8_pic18 MPLAB XC8 PIC18 Portu
 * \ingroup port
 * @{
 *
 * Görevci tarafından kullanılacak sistem donanımları burada ilklenir. Herbir
 * portun görevcisi kendi port.c dosyasında gerçeklenmiş olan
 * portGorevciyiBaslat() işlevini çağırarak başlatılır.
 */


#ifndef PORT_H
#define PORT_H

#include "gorevciypl.h"
#include "gorev.h"

#if grvCALISMA_KIPI == 1
/**
 * Görevlerin çalışacağı sonsuz döngüyü başlatır.
 *
 * Listedeki herbir görev sırasıyla çalıştırılır. Görevler işbirlikçi
 * (cooperative) olarak çalışırlar, çalışan göreve, kendisi kontrolü teslim
 * edinceye dek müdahale edilmez.
 * Her hedef platform (port) kendi portGorevciyiBaslat() işlevini platforma
 * özel bir şekilde gerçeklemiş olduğundan, görevci normal kipte
 * kullanılacaksa, bu işlev ile başlatılmalıdır.
 */
void portGorevciyiBaslat(void);
#endif

/**
 * Sistemin kritik bir bölüme geçmesini sağlar.
 * 
 * Kritik bölüm, genelde atomik yani hiçbir şekilde bölünmemesi gereken bir
 * işlemin hemen öncesinde çağrılmalıdır. Atomik işlemin bitiminde ise bu işlevin
 * eşi olan portKritikBolumCikisi() işlevi çağrılmalıdır. Yoksa kesme bitleri 
 * bu işlev tarafından değiştirildiğinden sistem beklendiği gibi çalışmaz.
 * 
 * \warning Bu işlev çağrıldıktan sonra sistemin tüm kesme kaynakları 
 * etkisizleştirileceğinden, portKritikBolumCikisi() çağrılana dek hiçbir kesme
 * isteği işlenemeyecektir. Bu yüzden kritik bölüm işlemi olabildiğince kısa ve öz
 * olmalıdır. Yoksa zaman kısıtlı olaylar gibi önemli kesme olayları geç işlenir
 * ve bu olaylara bağlı diğer işlemlerin gecikmesine veya aksamasına neden olur.
 * 
 * \sa portKritikBolumCikisi().
 */
void portKritikBolumGirisi(void);

/**
 * Sistemin kritik bir bölümden çıkmasını sağlar.
 * 
 * Kritik bir işlem biter bitmez bu işlev çağrılmalıdır ki sistemin kesmeye bağlı
 * olay kaynakları hemen etkinleştirilsin ve bu olaylar bir an önce işlensin. Bu
 * yüzden kritik bölüm olabildiğince kısa ve öz olmalıdır.
 * 
 * \sa portKritikBolumGirisi().
 */
void portKritikBolumCikisi(void);

#endif

/// @}
