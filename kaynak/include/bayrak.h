
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
 * Bu modül sayıcı bayrakları görevlerin üzerine gerçekler.
 * Bayraklar bir iki işlem sağlayan bir senkronizasyon ilkelleridir:
 * "bekle" ve "imle" (wait and signal). Bekleme işlemi bayrak sayıcısını
 * yoklar ve sıfırsa görevi bloklar. İmleme işlemi bayrak sayıcısını
 * arttırır ama görevi bloklamaz. Başka bir görev imlenen bayrağı beklemek
 * için bloklanmışsa, bloklanan görev, yeniden çalışabilir olacaktır.
 * 
 * Bayraklar, izleyiciler, ileti tamponları ve bağlı tamponlar gibi
 * daha yapılandırılmış senkronizasyon ilkellerini gerçeklemek için
 * kullanılabilirler.
 */

#ifndef BAYRAK_H
#define BAYRAK_H

#include "gorev.h"

typedef struct{
	unsigned char sayim;
} bayrak_t;

/**
 * \Ozet	Bir bayrağı ilkler.
 * 
 * \param *bayrak:	Bayrağa referans.
 * \param sayim:	Sayım, yani bir kaynağa aynı anda erişebilecek görev sayısı.
 */
#define	grvBAYRAK_ILKLE(b, s)	(b)->sayim = s

/**
 * \Ozet	Bir bayrağı bekler. RTOS veya birçok işletim sistemindeki
 *			@semaphore @wait veya @take türünde işlevlere eşdeğerdir.
 * 
 * \param *gorev:	Bayrağı alan görev tutucu.
 * \param *bayrak:	Bayrağa referans
 */
#define	grvBAYRAK_BEKLE(g, b)				\
	do{									\
		grvKOSUL_BEKLE(g, (b)->sayim > 0); \
		--(b)->sayim;					\
	} while(0)

/**
 * \Ozet	Bir bayrağı imler. RTOS veya birçok işletim sistemindeki
 *			 @semaphore @signal veya @give türünde işlevlere eşdeğerdir.
 * 
 * \param *bayrak:	Bayrağaa referans.
 * 
 */
#define grvBAYRAK_IMLE(b)	++(b)->sayim;

#endif
