/**
 * \file
 * \author İsmail Sahillioğlu (Kozmotronik)
 * \copyright \ref lisans "MIT Lisansı"
 * 
 * \defgroup bayrak Bayrak
 * \ingroup senkronizasyon
 * @{
 *
 * Bu modül sayıcı bayrakları görevlerin üzerinde gerçekler. Bayraklar birkaç
 * işlem sağlayan bir senkronizasyon ilkelleridir:
 * \b bekle ve \b imle (wait and signal). Bekleme işlemi bayrak sayıcısını
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

/**
 * Görevlerin veri kaynaklara erişiminin senkronizasyonu için kullanılır.
 *
 * Bir kaynak veya veri birden fazla koşut (paralel) çalışan görev tarafından
 * kullanıldığında veya değiştirildiğinde, veri bütünlüğünü korumak zor bir
 * iştir. Bayrak (semaphore) API'si bu zor işi yüklenir, görevlerin kaynak veya
 * verilere erişimini senkronize eder.
 */
struct Bayrak {
    unsigned char sinir; ///< En çok erişim miktarı.
    unsigned char kalan; ///< Kalan erişim hakkı.
};

/// Kolaylık sağlamak için Bayrak tür tanımlaması.
typedef Bayrak bayrak_t;


/**
 * Bir bayrağı ilkler.
 * 
 * Bayraklar kullanılmadan önce ilklenmelidirler. İlklenmeyen bayraklar
 * belirsiz veya beklenmeyen sonuçlara neden olabilir. Bir bayrak basitçe
 * şöyle ilklenir:
 * \code
 * bayrak_t bayrak;
 * //...
 * // main döngüsünde veya uygun bir yerde ilkleme yapılır.
 * grvBAYRAK_ILKLE(&bayrak, 1);
 * \endcode
 *
 * \param b Bayrağa referans.
 * \param s Sayım, yani bir kaynağa aynı anda erişebilecek görev sayısı.
 * \hideinitializer
 */
#define grvBAYRAK_ILKLE(b, s)   \
    ((bayrak_t*) b)->sinir = s; \
    ((bayrak_t*) b)->kalan = s;

/**
 * Bir bayrağı bekler.
 *
 * Koşut (paralel) çalışan birden fazla görevden bir kaynağa erişirken;
 * bayrak ilklenirken belirlenen erişim sayısına bağlı olarak erişimi
 * ilk alan görev veya görevler kaynağa veya veriye erişim sağlarken, erişim
 * sınırı dolduktan sonra erişen görev veya görevler bloklanacak, ilk erişim
 * alan görevler erişimi grvBAYRAK_IMLE() ile saldıktan sonra bloklanan
 * görevler artan erişim sınırı kadar kaynağa veya veriye erişebilecektir.
 * Kullanımı basitçe şöyledir:
 * \code
 * bayrak_t bayrak;
 * char korunanKaynak;
 * //...
 * // Erişimi almaya çalış; alamazsa bloklanır.
 * grvBAYRAK_BEKLE(gorevTutucu, &bayrak);
 * // Kaynağa erişim sağlandı, artık değiştirilebilir.
 * korunanKaynak = 'A';
 * // İşlem bitince diğer görevlerin erişebilmesi için erişimi sal.
 * grvBAYRAK_IMLE(gorevTutucu, &bayrak);
 * \endcode
 * 
 * \param g Bayrağı alan görevin tutucusu.
 * \param b Bayrağa referans.
 * \hideinitializer
 */
#define grvBAYRAK_BEKLE(g, b)                           \
        grvKOSUL_BEKLE(g, ((bayrak_t*) b)->kalan > 0);  \
        --((bayrak_t*) b)->kalan;

/**
 * Bir bayrağı imler.
 *
 * Birçok işletim sistemindeki signal veya give gibi işlevlere benzer.
 * Bir görev eriştiği kaynakla işlem yapmayı bitirince, başka görevlerin de
 * bu kaynağa erişebilmesi için bu makroyu çalıştırarak erişimi salar.
 * Kullanımı basitçe şöyledir:
 * \code
 * bayrak_t bayrak;
 * char korunanKaynak;
 * //...
 * // Erişimi almaya çalış; alamazsa bloklanır.
 * grvBAYRAK_BEKLE(gorevTutucu, &bayrak);
 * // Kaynağa erişim sağlandı, artık değiştirilebilir.
 * korunanKaynak = 'A';
 * // İşlem bitince diğer görevlerin erişebilmesi için erişimi sal.
 * grvBAYRAK_IMLE(gorevTutucu, &bayrak);
 * \endcode
 * 
 * \param b Bayrağa referans.
 * \hideinitializer
 */
#define grvBAYRAK_IMLE(b)                                   \
    if(((bayrak_t*) b)->kalan < ((bayrak_t*) b)->sinir) {   \
        ++((bayrak_t*) b)->kalan;                           \
    }

#endif
/// @}
