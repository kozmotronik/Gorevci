<h3 align="center">
  <a href="https://github.com/kozmotronik/Gorevci/blob/develop/assets/yazili-simge-indigo.png" target="_blank">
  <img src="https://github.com/kozmotronik/Gorevci/blob/develop/assets/yazili-simge-indigo.png" alt="fastlane Logo" width="500">
  </a>
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
yoksa [Issues][issues-url] bölümünde bir konu açarak istek yapabilirsiniz.  

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
özellikler için [Genel Özellikler][genel-ozellikler-url] sayfasına bakınız.  
Yaklaşık sistem gereksinimleri için [Minimum Sistem Gereksinimleri][MSG-url] 
sayfasına bakınız.
 
## Sorun Bildirme
Lütfen sorunları ve eklenmesini istediğiniz özellikleri [Issues][issues-url] 
bölümünden bildirerek bu projenin gelişmesine katkı sağlayın.  
Bir özellik istemeden önce bu projenin küçük ölçekli aygıtlar için 
geliştirildiğini göz önünde bulundurunuz.


[issues-url]: https://github.com/kozmotronik/Gorevci/issues
[gorevci-wiki-url]: https://github.com/kozmotronik/Gorevci/wiki/G%C3%B6revci-Wiki
[genel-ozellikler-url]: https://github.com/kozmotronik/Gorevci/wiki/Genel-%C3%96zellikler
[MSG-url]: https://github.com/kozmotronik/Gorevci/wiki/Minimum-Sistem-Gereksinimleri
