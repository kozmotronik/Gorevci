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
 * Bu modülün amacı statik olarak tanımlanacak @unsigned char türünde bir
 * dizinin referansını tutup o dizi üzerinde FIFO (İlk giren ilk çıkar) türünde
 * bir kuyruk yönetimi yapmaktır. Modül, @uckuyruk_t türünde bir yapıya sınırlı
 * kapasiteye (en fazla 255) sahip statik bir @unsigned char dizisi bağlamak
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
 * 
 * // Önce statik bir dizi tanımlanır:
 * #define DIZI_KAPASITE 48
 * unsigned char dizi[DIZI_KAPASITE];
 * 
 * // Dizi tanımlandıktan sonra bir @uckuyruk_t tanımlanır:
 * @uckuyruk_t kuyruk;
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
 * 
 * Eğer kuyrukta @char türünde veriler saklanmak istenirse tek yapılması
 * gereken; veriyi kuyruktan alırken @char türüne dönüştürmektir (type casting).
 * Önceki örneği @char türü için güncellemek istersek yalnızca kuyruktan alma
 * işleminde değişiklik yapmamız gerekir:
 * 
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
 *      // Char türünde veriyi almak için tek yapılması gereken casting işlemidir.
 *      char veri = (char) uckuyrukKuyruktanAl(&kuyruk);
 * }
 * 
 * Basit tekli kuyruklama ve kuyruktan almaya ek olarak çoklu kuyruklama ve
 * çoklu kuyruktan alma API'leri de vardır. Bunlar:
 * 
 * - uckuyrukCokluKuyrukla()
 * - uckuyrukCokluAl()
 * 
 * işlevleridir.
 * 
 */

#ifndef UC_KUYRUK_H
#define UC_KUYRUK_H

#ifndef NULL
#define NULL ( (void*) 0 )
#endif


/**
 * \Ozet	@uckuyruk_t (unsigned char kuyruk) FIFO (İlk giren ilk çıkar)
 *			türünde bir kuyruk yapısıdır. Amacı statik olarak tanımlanmış 
 *			@unsigned char türünde bir diziye referans alıp düşük belleğe sahip 
 *			aygıtlarda basit bir FIFO kuyruğu yönetmektir.
 *			Kuyruk, @bas ve @sayim adında iki değişkenle yönetilir. 
 *			@bas, kuyruktan alınmayı bekleyen ilk öğenin konumunu tutar.
 * 
 * \kapasite:	1 - 255 arası kapasite. Bu, kuyruğun sonunu bilmek için 
 * 				gereklidir.
 * \sayim:	Kuyruğa yazılan verilerin sayımını tutar. Bir sonraki yazma konumunu
 *			hesaplamak için de kullanılır.
 * \bas:	Kuyrukta sıradaki okunmayı / alınmayı bekleyen verinin konumunu
 *			tutar.
 * \veri:	Uygulamada oluşturulan @unsigned char türünde bir diziye başvuru
 *			adresini tutan imci (pointer).
 */
typedef struct {
	unsigned char kapasite;
	unsigned char sayim;
	unsigned char bas;
	unsigned char *veri;
} uckuyruk_t;

/**
 * \Ozet	@uckuyruk_t fifo kuyruğunu ilk kullanıma hazırlar.
 * 
 * \param kapasite:	Kuyruğun 1 - 255 aralığında kapasitesi, sizeof ile 
 *					unsigned char türüne cast edilerek verilebilir.
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * \param *veri:	Uygulamada tanımlanan bir unsigned char türünde dizi.
 * 
 */
void uckuyrukIlkle(const unsigned char, uckuyruk_t*, unsigned char*);


/**
 * \Ozet	Kuyruğa veri ekler.
 * 
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * \param veri:		Kuyruğa alınacak @unsigned char türünde veri.
 * 
 * \return donus:	Kuyruk doluysa veriyi kuyruğa almaz ve 0 döndürür.
 *					Veriyi kuyruğa alırsa 1 döndürür.
 * 
 */
char uckuyrukKuyrukla(uckuyruk_t*, const unsigned char);


/**
 * \Ozet	Kuyruğa belirtilen nicelikte veri ekler.
 * 
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * \param *veri:	Veriyi içeren dizideki verinin başlangıç adresine referans.
 * \param kac:		Başlangıcı verilen veriden kaç tanesinin kuyruğa alınacağı.
 * 
 * \return donus:	Kuyruk doluysa veya belirtilen nicelikteki verileri alacak
 *					kadar boş yer yoksa verileri kuyruğa almaz ve 0 döndürür.
 *					Verileri kuyruğa alırsa kuyruklanan verilerin niceliğini
 *					döndürür.
 * 
 */
char uckuyrukCokluKuyrukla(uckuyruk_t*, const unsigned char*, const unsigned char);


/**
 * \Ozet	Kuyrukta alınmayı bekleyen en baştaki öğeyi gösterir (peek eder).
 *			Öğe kuyruktan çıkarılmaz.
 * 
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * 
 * \return donus:	Kuyrukta öğe varsa en baştaki öğe, yoksa 0 döndürür. 
 *					0 değeri de unsigned char türünden kuyruklanabilir bir 
 *					değerdir. Burada programcı dönen değerin ayrımını yapmalıdır.
 * 
 */
unsigned char uckuyrukBastakiOge(uckuyruk_t*);


/**
 * \Ozet	Kuyrukta alınmayı bekleyen en baştaki öğeyi kuyruktan alır.
 *			Kuyruktan alınan öğe kuyruktan çıkarılır.
 * 
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * 
 * \return donus:	Kuyrukta öğe varsa en baştaki öğe, yoksa 0 döndürür. 
 *					0 değeri de unsigned char türünden kuyruklanabilir bir 
 *					değerdir. Burada programcı dönen değerin ayrımını yapmalıdır.
 * 
 */
unsigned char uckuyrukKuyruktanAl(uckuyruk_t*);


/**
 * \Ozet	Kuyrukta alınmayı bekleyen öğelerden belirtilen nicelikte öğeyi
 *			parametre olarak sağlanan diziye alır. Kuyruktan alınan veriler
 *			kuyruktan çıkarılır. Programcı parametre olarak verilen dizinin 
 *			belirtilen nicelikte veriyi alacak kapasitesi olduğundan emin
 *			olmalıdır.
 * 
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * \param *veri:	Kuyruktan alınan verilerin saklanmak istediği konuma referans.
 * \param kac:		Başlangıcı verilen veriden kaç tanesinin kuyruğa alınacağı.
 * 
 * \return donus:	Kuyrukta alınmak istenen nicelik kadar öğe yoksa 0 döndürür.
 *					Kuyruktan başarıyla çoklu alınırsa, alınan verilerin
 *					niceliğini döndürür.
 * 
 */
char uckuyrukCokluAl(uckuyruk_t*, unsigned char*, const unsigned char);


/**
 * \Ozet	Kuyrukta bekleyen öğe niceliğini döndürür.
 * 
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * 
 * \return donus:	Kuyrukta bekleyen öğe niceliği.
 * 
 */
unsigned char uckuyrukKacOgeVar(uckuyruk_t*);


/**
 * \Ozet	Kuyruğun dolu olup olmadığını yoklar.
 * 
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * 
 * \return donus:	Kuyruk doluysa 1 (@true), değilse 0 (@false) döndürür.
 * 
 */
char uckuyrukDolu(uckuyruk_t*);


/**
 * \Ozet	Kuyruğun kapasite niceliğini yani kuyruğun daha kaç öğe
 *			alabileceğinin sayısının bilgisini verir.
 * 
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * 
 * 
 */
unsigned char uckuyrukKalanKapasite(uckuyruk_t*);

/**
 * \Ozet	Kuyruğun boş olup olmadığını yoklar. @uckuyrukDolu() API' sine
 *          alternatiftir.
 * 
 * \param *kuyruk:	Uygulamada tanımlanan bir @uckuyruk_t yapısına referans.
 * 
 * \return donus:	Kuyruk boşsa 1 (@true), değilse 0 (@false) döndürür.
 * 
 */
char uckuyrukBos(uckuyruk_t*);

#endif /* UC_KUYRUK_H */
