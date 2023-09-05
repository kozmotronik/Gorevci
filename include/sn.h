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
