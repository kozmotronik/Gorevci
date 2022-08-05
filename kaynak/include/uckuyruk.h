/**
 * Görevci v1.0.0
 * Copyright (C) 2021 İsmail Sahillioğlu (aka Kozmotronik). Tüm Hakları
 * Saklıdır. Hiçbir ücret talep edilmeden burada işbu* yazılımın bir kopyasını
 * ve belgelendirme dosyalarını (“Yazılım”) elde eden herkese verilen izin;
 * kullanma, kopyalama, değiştirme, birleştirme, yayımlama, dağıtma, alt
 * lisanslama, ve/veya yazılımın kopyalarını satma eylemleri de dahil olmak
 * üzere ve bununla kısıtlama olmaksızın, yazılımın sınırlama olmadan
 * ticaretini yapmak için verilmiş olup, bunları yapmaları için yazılımın
 * sağlandığı kişilere aşağıdakileri yapmak koşuluyla sunulur:
 *
 * Yukarıdaki telif hakkı bildirimi ve işbu izin bildirimi yazılımın tüm
 * kopyalarına veya önemli parçalarına eklenmelidir.
 *
 * YAZILIM “HİÇBİR DEĞİŞİKLİK YAPILMADAN” ESASINA BAĞLI OLARAK, TİCARETE
 * ELVERİŞLİLİK, ÖZEL BİR AMACA UYGUNLUK VE İHLAL OLMAMASI DA DAHİL VE BUNUNLA
 * KISITLI OLMAKSIZIN AÇIKÇA VEYA ÜSTÜ KAPALI OLARAK HİÇBİR TEMİNAT OLMAKSIZIN
 * SUNULMUŞTUR. HİÇBİR KOŞULDA YAZARLAR VEYA TELİF HAKKI SAHİPLERİ HERHANGİ BİR
 * İDDİAYA, HASARA VEYA DİĞER YÜKÜMLÜLÜKLERE KARŞI, YAZILIMLA VEYA KULLANIMLA
 * VEYA YAZILIMIN BAŞKA BAĞLANTILARIYLA İLGİLİ, BUNLARDAN KAYNAKLANAN VE
 * BUNLARIN SONUCU BİR SÖZLEŞME DAVASI, HAKSIZ FİİL VEYA DİĞER EYLEMLERDEN
 * SORUMLU DEĞİLDİR.
 */

/**
 * \defgroup veri Veri Yönetimi
 * \ingroup gorev
 * @{
 *
 * Görevci için bir ardışıl FIFO modelinde bir kuyruk kütüphanesi.
 *
 * uckuyruk unsigned char kuyruk kısaltması olarak kullanılmıştır.
 * Bu modülün amacı statik olarak tanımlanacak unsigned char türünde bir
 * dizinin referansını tutup o dizi üzerinde FIFO (İlk giren ilk çıkar) türünde
 * bir kuyruk yönetimi yapmaktır. Modül, uckuyruk_t türünde bir yapıya sınırlı
 * kapasiteye (en fazla 255) sahip statik bir unsigned char dizisi bağlamak
 * koşuluyla, tek bir uygulama içerisinde birden çok FIFO kuyruğu yönetimi
 * yapabilecek şekilde yazılmıştır. Özellikle seri portlar veya LCD ekranlar
 * gibi görece yavaş çalışan donanımlara birden çok ardı ardına veri yazılması
 * gereken durumlarda bu kuyruğu kullanmak çok avantajlıdır. Bu FIFO kuyruğu
 * kullanıldığı takdirde, bu tür yavaş aygıtların veriyi byte-byte göndermesini
 * beklemek yerine kuyruğa yazıp sonra ilgili donanım göndermeye her müsait
 * olduğunda, kuyruktan alınıp ilgili donanıma verilebilir. Böylece CPU zamanı
 * yavaş donanımların işini bitirmesini beklemek için boşuna harcanmaz.
 * 
 * Kullanımı oldukça basittir:
 * \code
 * // Önce statik bir dizi tanımlanır:
 * #define DIZI_KAPASITE 48
 * unsigned char dizi[DIZI_KAPASITE];
 * 
 * // Dizi tanımlandıktan sonra bir uckuyruk_t tanımlanır:
 * uckuyruk_t kuyruk;
 * 
 * // Tanımlanan kuyruk ilgili parametrelerle ilklenir:
 * uckuyrukIlkle(DIZI_KAPASITE, &kuyruk, dizi);
 * 
 * // Kuyruk bir kez ilklendi mi artık kullanıma hazırdır:
 * // Kuyruklamak için:
 * if(uckuyrukKuyrukla(&kuyruk, 25) == 0) {
 *      // Kuyruklama başarısız; kuyruk dolu.
 *      // Burada farklı yeniden deneme stratejileri uygulanabilir.
 * }
 * else {
 *      // Kuyruklama başarılı.
 * }
 * 
 * // Kuyruktan almak için:
 * if(uckuyrukBos(&kuyruk)) {
 *      // Kuyruk boş, alınacak bir şey yok.
 *      // Burada farklı bekleme stratejileri uygulanabilir.
 * }
 * else {
 *      // Kuyruktan veri var, alınabilir.
 *      unsigned char veri = uckuyrukKuyruktanAl(&kuyruk);
 * }
 * \endcode
 *
 * Eğer kuyrukta char türünde veriler saklanmak istenirse tek yapılması
 * gereken; veriyi kuyruktan alırken char türüne dönüştürmektir (type casting).
 * Önceki örneği char türü için güncellemek istersek yalnızca kuyruktan alma
 * işleminde değişiklik yapmamız gerekir:
 * \code
 * // Kuyruklamak için:
 * if(uckuyrukKuyrukla(&kuyruk, 'A') == 0) {
 *      // Kuyruklama başarısız; kuyruk dolu.
 *      // Burada farklı yeniden deneme stratejileri uygulanabilir.
 * }
 * else {
 *      // Kuyruklama başarılı.
 * }
 * 
 * // Kuyruktan almak için:
 * if(uckuyrukBos(&kuyruk)) {
 *      // Kuyruk boş, alınacak bir şey yok.
 *      // Burada farklı bekleme stratejileri uygulanabilir.
 * }
 * else {
 *      // Char türünde veriyi almak için tek yapılması gereken casting
 *      // işlemidir.
 *      char veri = (char) uckuyrukKuyruktanAl(&kuyruk);
 * }
 * \endcode
 * 
 * Basit tekli kuyruklama ve kuyruktan almaya ek olarak çoklu kuyruklama ve
 * çoklu kuyruktan alma API'leri de vardır. Bunlar:
 * 
 * - uckuyrukCokluKuyrukla()
 * - uckuyrukCokluAl()
 * işlevleridir.
 * 
 * Bu kadarla da sınırlı değil! Çeşitli ardışıllaştırma (serializing)
 * teknikleri kullanarak byte ırmağına dönüştürülmüş daha büyük türde ve yapıda
 * veriler de kuyruklanabilir. Aynı kuyruktan veri eski haline döndürmek için;
 * kuyruklamadan önce uygulanan ardışıllaştırma tekniğinin tersi bir teknik ile
 * istenen tür ve yapıdaki veri byte ırmağı halinde kuyruktan alınıp özgün
 * yapısına dönüştürülebilir.
 */

#ifndef UC_KUYRUK_H
#define UC_KUYRUK_H

#ifndef NULL
#define NULL ( (void*) 0 )
#endif


/**
 * UCKuyruk (unsigned char kuyruk) FIFO (İlk giren ilk çıkar) türünde bir
 * kuyruk yapısıdır. Amacı statik olarak tanımlanmış unsigned char türünde bir
 * diziye referans alıp düşük belleğe sahip aygıtlarda basit bir FIFO kuyruğu
 * yönetmektir. Kuyruk, bas ve sayim adında iki değişkenle yönetilir. bas,
 * Kuyruktan alınmayı bekleyen ilk öğenin konumunu tutar.
 */
struct UCKuyruk {
    /// 1 - 255 arası kapasite. Bu, kuyruğun sonunu bilmek için gereklidir.
    unsigned char kapasite;
    /// Kuyruğa yazılan verilerin sayımını tutar. Bir sonraki yazma konumunu
    /// hesaplamak için de kullanılır.
    unsigned char sayim;
    /// Kuyrukta sıradaki okunmayı / alınmayı bekleyen verinin konumunu tutar.
    unsigned char bas;
    /// Uygulamada oluşturulan unsigned char türünde bir diziye başvuru
    /// adresini tutan imci (pointer).
    unsigned char *tampon;
};
typedef UCKuyruk uckuyruk_t;

/// uckuyruk_t yapısı için başvuru türü (okunaklılığı iyileştirmek için)
typedef uckuyruk_t *puck_t;


/**
 * Bir kuyruğu ilk kullanıma hazırlar. kuyruk veya tampon parametrelerinin biri
 * NULL ise işlem sürdürülmez.
 * 
 * \param[in] kapasite Kuyruğun 1 - 255 aralığında kapasitesi, sizeof ile
 * unsigned char türüne cast edilerek verilebilir.
 * \param[in] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına referans.
 * \param[in] tampon Uygulamada tanımlanan bir unsigned char türünde dizi.
 */
void uckuyrukIlkle(
    const unsigned char kapasite, puck_t kuyruk, unsigned char* tampon);

/**
 * Bir kuyruğa veri ekler.
 *
 * Veri kuyruğun en sonuna eklenir ve kuyruklamanın başarılı olduğunu belirten
 * 1 degeri döndürülür. kuyruk parametresi NULL verilirse veya kuyruk doluysa
 * işlem sürdürülmez ve kuyruklamanın başarısız olduğunu ifade eden bir 0
 * değeri döndürülür.
 * 
 * \param[in,out] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına
 * referans.
 * \param[in] deger Kuyruğa alınacak unsigned char türünde veri.
 * \return Kuyruk NULL veya doluysa \b 0, kuyruklama başarılıysa \b 1.
 * 
 */
char uckuyrukKuyrukla(puck_t kuyruk, const unsigned char deger);

/**
 * Bir kuyruğa belirtilen nicelikte veri ekler.
 * 
 * Veriler kuyruğun en sonuna eklenir ve kuyruklanan verinin niceliği
 * döndürülür. kuyruk veya kaynak parametreleri NULL verilirse veya nicelik
 * 1'den küçükse veya kuyrukta; kuyruklanmak istenen verileri alacak kadar yer
 * yoksa işlem sürdürülmez ve kuyruklamanın başarısız olduğunu ifade eden bir 0
 * değeri döndürülür.
 *
 * \param[in,out] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına
 * referans.
 * \param[in] kaynak Veriyi içeren dizideki verinin başlangıç adresine
 * referans.
 * \param[in] nicelik Başlangıcı verilen veriden kaç tanesinin kuyruğa
 * alınacağı.
 * \return kuyruk veya kaynak NULL ise veya kuyrukta yeteri kadar boş yer yoksa
 * \b 0, kuyruklama başarılıysa \b nicelik.
 */
unsigned char uckuyrukCokluKuyrukla(
    puck_t kuyruk, const unsigned char* kaynak, const unsigned char nicelik);

/**
 * Bir kuyruğun en başındaki veriyi gösterir (peek).
 *
 * Gösterilen veri kuyruktan çıkarılmaz, veriyi kuyruktan çıkarıp almak için
 * uckuyrukKuyruktanAl() işlevini kullanınız. kuyruk parametresi NULL verilirse
 * bu işlev hep 0 değerini döndürecektir. 0 Değeri de unsigned char türünden
 * kuyruklanabilir bir değerdir. Dönen değerin doğrulanması veya ayrımının
 * yapılması programcının sorumluluğudur. Bunu yapmanın en kolay yolu bu işlevi
 * kullanmadan önce kuyruğu; uckuyrukBos() işlevini kullanarak yoklamaktır.
 * 
 * \param[in] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına referans.
 * \return kuyruk NULL ise hep 0, yoksa kuyruğun başındaki veri.
 *
 * \sa uckuyrukKuyruktanAl()
 */
unsigned char uckuyrukBastakiOge(puck_t kuyruk);

/**
 * Bir kuyruğun en başındaki veriyi alır.
 *
 * Kuyruktan alınan veri kuyruktan çıkarılır. Kuyruğun en başındaki değere onu
 * kuyruktan çıkarmadan bakmak için uckuyrukBastakiOge() kullanılır. kuyruk
 * parametresi NULL verilirse bu işlev hep 0 değerini döndürecektir. 0 Değeri
 * de unsigned char türünden kuyruklanabilir bir değerdir. Dönen değerin
 * doğrulanması veya ayrımının yapılması programcının sorumluluğudur. Bunu
 * yapmanın en kolay yolu bu işlevi kullanmadan önce kuyruğu; uckuyrukBos()
 * işlevini kullanarak yoklamaktır.
 * 
 * \param[in] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına referans.
 * \return kuyruk NULL ise hep 0, yoksa kuyruğun başındaki veri.
 *
 * \sa uckuyrukBastakiOge(), uckuyrukBos().
 */
unsigned char uckuyrukKuyruktanAl(puck_t kuyruk);


/**
 * Bir kuyruktan istenen nicelikte veri alır.
 *
 * Kuyruğun başındaki verileri kuyruktaki sırasıyla, hedef belleğe belirtilen,
 * nicelikte alır. Kuyruktan alınan veriler kuyruktan çıkarılır. Kuyruktan
 * alınacak veriyi saklayacak kadar yere sahip bir hedef sağlamak programcının
 * sorumluluğudur. Aksi takdirde veriler kaybolabilir veya gereken kapasiteye
 * sahip bir yer sağlanmamışsa verilerin hedefe sığan bölümü alınırken kalanı
 * kaybolabilir. Hedefe aktarma işlemi başarılıysa aktarılan veri niceliği
 * döndürülür.
 * Diğer yandan; kuyruk veya hedef parametrelerinin biri NULL ise veya nicelik
 * 1'den küçükse veya kuyrukta istenen nicelikte veri yoksa 0 değeri
 * döndürülür.
 * 
 * \param[in] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına referans.
 * \param[out] hedef Kuyruktan alınan verilerin saklanmak istediği konuma
 * referans.
 * \param[in] nicelik Başlangıcı verilen veriden kaç tanesinin kuyruğa alınacağı.
 * \return Kuyrukta istenen nicelikte veri yoksa \b 0, hedefe aktarım
 * başarılıysa aktarılan verilerin niceliği.
 */
unsigned char uckuyrukCokluAl(
    puck_t kuyruk, unsigned char* hedef, const unsigned char nicelik);


/**
 * Bir kuyruğu tamamen boşaltır (flush).
 *
 * Bu işlem önce tamponu 0 değeri ile doldurur ve ardından kuyruk yönetim
 * değişkenlerini sıfırlar.
 * 
 * \param[in] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına referans.
 */
void uckuyrukBosalt(puck_t kuyruk);


/**
 * Bir kuyruğu belirtilen sayıda aynı veri ile doldurur.
 * 
 * İşlem başarılıysa kuyruğa doldurulan nicelik döndürülür. kuyruk NULL ise
 * veya kuyrukta doldurulacak nicelikte yer yoksa veya niceli 1'den küçükse hep
 * 0 değeri döndürülür.
 *
 * \param[in,out] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına
 * referans.
 * \param[in] deger Kuyruğa doldurulacak unsigned char türünde veri.
 * \param[in] nicelik Verilen verinin kuyruğa çoğaltılma sayısı.
 * \return Kuyruk doluysa veya belirtilen nicelikteki verileri alacak kadar boş
 * yer yoksa verileri kuyruğa almaz ve 0, işlem başarılı olursa kuyruklanan
 * verilerin niceliğini döndürür.
 */
unsigned char uckuyrukDoldur(
    puck_t kuyruk, const unsigned char deger, const unsigned char nicelik);

/**
 * Bir kuyrukta bekleyen öğe niceliğini döndürür.
 *
 * \param[in] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına referans.
 * \return Kuyrukta bekleyen öğe niceliği, kuyruk NULL ise her zaman \b 0.
 */
unsigned char uckuyrukKacOgeVar(puck_t kuyruk);

/**
 * Bir kuyruğun dolu olup olmadığını yoklar.
 *
 * \param[in] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına referans.
 * \return Kuyruk NULL veya doluysa \b 1 (true), değilse \b 0 (false) döndürür.
 */
char uckuyrukDolu(puck_t kuyruk);

/**
 * Bir kuyruğun kalan kapasitesini verir.
 *
 * Kuyruğun kapasite niceliğini yani kuyruğun daha kaç öğe alabileceğinin
 * sayısının bilgisini verir.
 * 
 * \param[in] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına referans.
 * \return Kuyruğun kalan kapasitesi, kuyruk NULL ise hep \b 0.
 */
unsigned char uckuyrukKalanKapasite(puck_t kuyruk);

/**
 * Bir kuyruğun boş olup olmadığını yoklar.
 * 
 * \param[in] kuyruk Uygulamada tanımlanan bir uckuyruk_t yapısına referans.
 * \return Kuyruk NULL veya boşsa \b 1 (true), değilse \b 0 (false) döndürür.
 */
char uckuyrukBos(puck_t kuyruk);

#endif /* UC_KUYRUK_H */
/// @}
