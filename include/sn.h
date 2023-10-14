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
 * \internal
 * 
 * Yalnızca dahili kullanım için.
 * 
 * Bu makrolar çağırıldıkları işlevin gerekli noktalarına; işleve bir sonraki
 * girişte işlevin en son kaldığı yerden sürmesi için switch-case ağacını
 * oluşturur.
 */

#ifndef SN_H
#define SN_H


typedef unsigned int sn_t; ///< Sürdürme Noktası. 

/**
 * Sürdürme noktasının değerini ilkler.
 * \param sn sn_t türünde sürdürme noktası değişkeni.
 * \hideinitializer
 */
#define SN_ILKLE(sn)    sn = 0

/**
 * Sürdürme noktasının switch-case yapı ağacını başlatır.
 * \param sn sn_t türünde sürdürme noktası değişkeni.
 * \hideinitializer
 */
#define SN_BASLAT(sn)   switch(sn) { case 0:

/**
 * Çağrıldığı yere bir sürdürme noktası kurar.
 * \param sn sn_t türünde sürdürme noktası değişkeni.
 * \hideinitializer
 */
#define SN_KUR(sn)      sn = __LINE__; case __LINE__:

/**
 * Sürdürme noktası switch-case yapı ağacını sonlandırır.
 * \param sn sn_t türünde sürdürme noktası değişkeni.
 * \hideinitializer
 */
#define SN_BITIR(sn)    }

#endif
