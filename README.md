<h3 align="center">
  <img src="assets/yazili-simge-indigo.png" alt="Görevci logo" width="500">
</h3>

Küçük ölçekli mikrodenetleyiciler için geliştirilen gömülü projelerin 
kurtarıcısı!  
Görevci daha çok küçük ölçekli gömülü projelere hitap eden bir kütüphanedir.
Görevci, zaman ve donanım tabanlı olaylara bağlı çalışan işlemleri CPU'yu
bloklamadan yönetmeyi amaçlar. Böylece görevler olaylara etkili bir biçimde
yanıt verir ve hiçbir olay kaçırılmadan işlenebilir.

## Görevciyi Edinme ve Projenize Ekleme
Görevciyi projelerinizde kullanmadan önce, kullandığınız aygıtın portunun 
desteklendiğinden emin olun. Eğer kullandığınız aygıt için bir port desteği 
yoksa [Issues][1] bölümünde bir konu açarak istek yapabilirsiniz.  

Görevciyi birkaç farklı yolla projelerinize entegre edebilirsiniz.  
Bunlardan ilki repoyu klonlayarak kaynak dosyalarını indirip proje ağacınıza 
eklemektir. Bunu yapmak için, dosyaları projenizin olduğu dizine ekledikten 
sonra geliştirme ortamınızda da (IDE) bu dosyaları eklemeli ardından proje 
özellikleri bölümünden include yollarını IDE'ye tanıtmalısınız. Include yolları 
tanıtılmazsa derleme başarısız olur. Bu aşamadan sonra uygulamanızı 
derleyebilirsiniz.

İkinci yöntem ise Görevciyi projenize bir git alt modülü (submodule) olarak 
eklemektir.  
  
## Lisans
Lütfen lisans metnini dikkatlice okuyun ama lisansa çok takılmayın. Bu lisansın 
özeti şöyle:  
*Burada sağlanan kaynak kodlarını arzuladığınız gibi kullanın. Ancak sağlanan 
üründe için hiçbir teminat verilmemekle birlikte, kullanımdan doğacak hiçbir 
sorumluluk da kabul edilmemektedir.*
* [MIT](LICENSE) - Orijinal metin.
* [MIT-TR](LISANS_tr.md) - Lisans metninin Türkçe sürümü.

## Özellikler
Düşük kaynak kullanır ve farklı platformlara kolayca adapte edilebilir. Diğer 
özellikler için [Genel Özellikler][2] sayfasına bakınız.  
Yaklaşık sistem gereksinimleri için [Minimum Sistem Gereksinimleri][3] 
sayfasına bakınız.

## Wiki ve API Belgelendirmesi
Görevci, kendisi ile ilgili çeşitli bilgiler bulabileceğiniz bir wiki 
bölümüne sahiptir. Göz atmak isterseniz -> [Görevci Wiki][4].  
Görevcinin 2.1.0 sürümünden itibaren yayınlanan herbir sürümü için 
çevrimiçi (html) ve indirilebilir (pdf) olarak iki farklı biçimde API 
belgelendirmesi sunulmaktadır. Bu belgelendirmelere [Görevci-Docs][5] 
sayfasından ulaşabilirsiniz.
 
## Sorun Bildirme
Lütfen sorunları ve eklenmesini istediğiniz özellikleri [Issues][1] 
bölümünden bildirerek bu projenin gelişmesine katkı sağlayın.  
Bir özellik istemeden önce bu projenin küçük ölçekli aygıtlar için 
geliştirildiğini göz önünde bulundurunuz.


[1]: https://github.com/kozmotronik/Gorevci/issues
[2]: https://github.com/kozmotronik/Gorevci/wiki/Genel-%C3%96zellikler
[3]: https://github.com/kozmotronik/Gorevci/wiki/Minimum-Sistem-Gereksinimleri
[4]: https://github.com/kozmotronik/Gorevci/wiki
[5]: https://kozmotronik.github.io/Gorevci-Docs/
