<h3 align="center">
  <a href="https://kozmotronik.github.io/Gorevci-Docs/" target="_blank">
  <img src="https://github.com/kozmotronik/Gorevci-Docs/blob/41ca0fef8178a79f6ecc211b0d6333a0d62f48d0/docs/assets/yazili-simge-indigo.png" alt="Görevci Logo" width="500">
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
ürün için hiçbir teminat verilmemekle birlikte, kullanımdan doğacak hiçbir 
sorumluluk da kabul edilmemektedir.
* [MIT](LICENSE) - Orijinal metin.
* [MIT-TR](LISANS_tr.md) - Lisans metninin Türkçe sürümü.

## Özellikler
Düşük kaynak kullanır ve farklı platformlara kolayca adapte edilebilir. Diğer 
özellikler için [Genel Özellikler][2] sayfasına bakınız.  
Yaklaşık sistem gereksinimleri için [Minimum Sistem Gereksinimleri][3] 
sayfasına bakınız.
 
## Sorun Bildirme
Lütfen sorunları ve eklenmesini istediğiniz özellikleri [Issues][1] 
bölümünden bildirerek bu projenin gelişmesine katkı sağlayın.  
Bir özellik istemeden önce bu projenin küçük ölçekli aygıtlar için 
geliştirildiğini göz önünde bulundurunuz.

## Görevci Wiki
Daha ayrıntılı bilgiler için aşağıdaki kaynaklara başvurabilirsiniz:
* [Görevci Wiki][4]
* [Görevci Belgelendirme][5]
* [Görevci Makaleleri][6]


[1]: https://github.com/kozmotronik/Gorevci/issues
[2]: https://github.com/kozmotronik/Gorevci/wiki/Genel-%C3%96zellikler
[3]: https://github.com/kozmotronik/Gorevci/wiki/Minimum-Sistem-Gereksinimleri
[4]: https://github.com/kozmotronik/Gorevci/wiki/G%C3%B6revci-Wiki
[5]: https://kozmotronik.github.io/Gorevci-Docs/
[6]: https://kozmotronik.com.tr/tags/gorevci/
